#pragma once

#include "scanner.h"
#include "parser.hh"

#include <map>
#include <string>
#include <fstream>

#include "Program.hpp"

class Driver {
 public:
  Driver();
  int Parse(const std::string& filename);
  void BeginScan();
  void EndScan();

  std::string filename_; // filename to read data from

  bool trace_parsing_; // debug levels
  bool trace_scanning_;

  int result_; // success or failure code

  friend class Scanner;
  Scanner scanner_;

  yy::parser parser_;
  yy::location location_;

  void PrintTree(const std::string& filename);

  void Evaluate(const std::string& filename);

//    void RunInterpreter(const std::string& filename);

  ast::Program* program_;

 private:
  std::ifstream stream_; // stream for scanner
};
