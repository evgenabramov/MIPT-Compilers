#pragma once

#include "../Generators/Temporary.h"

#include <fstream>
#include <string>
#include <vector>

namespace irt {

class Instruction {
 public:
  Instruction(std::string str, std::vector<Temporary> targets, std::vector<Temporary> sources)
    : str_(std::move(str)), targets_(std::move(targets)), sources_(std::move(sources)) {}
  
  std::string GetStr() const {
    return str_;
  }
  
  void Print(std::ofstream& out) const {
    std::string result = str_;
    for (size_t i = 0; i < targets_.size(); ++i) {
      std::string match = "t" + std::to_string(i);
      size_t pos = result.find(match);
      while (pos != std::string::npos) {
        result.replace(pos, match.length(), targets_[i].ToString());
        pos = result.find(match);
      }
    }
    for (size_t i = 0; i < sources_.size(); ++i) {
      std::string match = "s" + std::to_string(i);
      size_t pos = result.find(match);
      while (pos != std::string::npos) {
        result.replace(pos, match.length(), sources_[i].ToString());
        pos = result.find(match);
      }
    }
    out << result << std::endl;
  }
 
 private:
  std::string str_;
  std::vector<Temporary> targets_;
  std::vector<Temporary> sources_;
};

}