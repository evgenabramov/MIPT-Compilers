#include "driver.hh"

#include "PrintVisitor.hpp"
#include "SymbolTreeVisitor.hpp"
#include "MethodCallVisitor.h"
#include "IRTreeBuildVisitor.h"

#include <IRTree/Visitors/PrintVisitor.h>

#include "parser.hh"

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
  using namespace ast;

  SymbolTreeVisitor symbol_tree_visitor(filename);
  symbol_tree_visitor.Visit(program_);

  std::cout << "SymbolTreeVisitor finished work" << std::endl
            << "Tree layers (tab = depth in tree, BFS):" << std::endl;
  // Layers info:
  symbol_tree_visitor.OutputTreeLayers();

  // Classes info:
  symbol_tree_visitor.OutputClassesInfo();

  // For correct method_call_visitor work
  symbol_tree_visitor.FillClassStorage();

  std::shared_ptr<MethodType> main_method = std::dynamic_pointer_cast<MethodType>(
      symbol_tree_visitor.GetScopeLayerTree()->GetRoot()->Get("main"));

  MethodCallVisitor method_call_visitor(
      symbol_tree_visitor.GetScopeLayerTree()->GetScopeLayer("main"), main_method,
      new SimpleObject(new PrimitiveSimpleType(new SimpleType("int")), 0));

  method_call_visitor.SetTree(symbol_tree_visitor.GetScopeLayerTree());

  method_call_visitor.Visit(main_method->GetMethodDeclaration());

  IRTreeBuildVisitor irtree_build_visitor(symbol_tree_visitor.GetScopeLayerTree());
  irtree_build_visitor.Visit(program_);

  IrtMapping methods = irtree_build_visitor.GetMethodTrees();

  for (auto& [method_name, method_statement] : methods) {
    std::string output_filename = method_name + "_IRTreePrintVisitor_output";
    std::ofstream out(output_filename);
    out << "Method tree for " + method_name << ":" << std::endl;

    irt::PrintVisitor print_visitor(output_filename);
    method_statement->Accept(&print_visitor);
  }
}