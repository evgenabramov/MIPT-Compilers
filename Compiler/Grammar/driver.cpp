#include "Grammar/driver.hh"
#include "parser.hh"

#include "PrintVisitor.hpp"
#include "SymbolTreeVisitor.hpp"

#include <fstream>

Driver::Driver() :
    trace_parsing_(false),
    trace_scanning_(false),
    scanner_(*this), parser_(scanner_, *this) {}

int Driver::Parse(const std::string& filename) {
    filename_ = filename;
    location_.initialize(&filename_);
    BeginScan();
    parser_.set_debug_level(trace_parsing_);
    int res = parser_();
    EndScan();
    return res;
}

void Driver::BeginScan() {
  scanner_.set_debug(trace_scanning_);
  
  if (filename_.empty() || filename_ == "-") {
    std::cout << "Empty filename" << std::endl;
  } else {
    stream_.open(filename_);
    std::cout << filename_ << std::endl;
    scanner_.yyrestart(&stream_);
  }
}

void Driver::EndScan() {
    stream_.close();
}

void Driver::PrintTree(const std::string& filename) {
    ast::PrintVisitor visitor(filename);
    visitor.Visit(program_);
}

//void Driver::RunInterpreter(const std::string& filename) {
//    ast::Interpreter interpreter(filename);
//    interpreter.Visit(program_);
//    interpreter.PrintReturnValue();
//}

void Driver::Evaluate(const std::string& filename) {
    SymbolTreeVisitor visitor(filename);
    visitor.Visit(program_);

    std::cout << "SymbolTreeVisitor finished work" << std::endl
              << "Tree layers (tab = depth in tree, BFS):" << std::endl;
    visitor.OutputTreeLayers();

    std::ofstream out;
    out.open(filename, std::ios_base::app);

    std::unordered_map<Symbol, std::shared_ptr<ClassType>> class_types = visitor.GetClassTypes();
    for (auto& class_type : class_types) {
        out << "Class name: " << class_type.first.GetName() << std::endl;
        out << "Class methods:" << std::endl;
        for (auto& method_type : class_type.second->GetMethodTypes()) {
            out << "method name: " << method_type.first.GetName()
                << " return type: " << method_type.second->GetReturnValueType()->GetIdentifier() << std::endl;
        }
        out << "Class fields:" << std::endl;
        for (auto& method_type : class_type.second->GetFieldTypes()) {
            out << "field name: " << method_type.first.GetName()
                << " type: ";
            if (method_type.second->IsSimpleType()) {
                out << method_type.second->GetTypeName() << std::endl;
            } else {
                out << method_type.second->GetTypeName() << "[]" << std::endl;
            }
        }
    }

    out.close();
}