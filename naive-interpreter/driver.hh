#pragma once

#include <map>
#include <string>
#include <fstream>
#include "scanner.h"
#include "parser.hh"


class Driver {
 public:
    Driver();
    int Parse(const std::string& filename);
    void BeginScan();
    void EndScan();

    std::map<std::string, int> variables_;
    std::map<std::string, bool> bool_variables_;
    std::string filename_; // filename to read data from

    bool trace_parsing_; // debug levels
    bool trace_scanning_;

    int result_; // success or failure code

    friend class Scanner;
    Scanner scanner_;
    
    yy::parser parser_;
    yy::location location_;
 
 private:
    std::ifstream stream_; // stream for scanner
};
