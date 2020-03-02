#include "driver.hh"
#include "parser.hh"

Driver::Driver() :
    trace_parsing_(false),
    trace_scanning_(false),
    scanner_(*this), parser_(scanner_, *this) {
    variables_["one"] = 1; // convert tokens to values
    variables_["two"] = 2;
}

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