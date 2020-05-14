#include "driver.hh"
#include "parser.hh"

#include "PrintVisitor.hpp"
#include "Interpreter.hpp"

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

void Driver::RunInterpreter(const std::string& filename) {
    ast::Interpreter interpreter(filename);
    interpreter.Visit(program_);
    interpreter.PrintReturnValue();
}