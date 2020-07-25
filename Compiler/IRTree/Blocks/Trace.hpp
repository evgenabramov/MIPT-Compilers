#pragma once

#include "Block.hpp"

#include <vector>
#include <fstream>

namespace irt {

class Trace {
 public:
  Trace() = default;
  
  explicit Trace(std::vector<Block*> blocks) : blocks_(std::move(blocks)) {}
  
  void AddBlock(Block* block) {
    blocks_.push_back(block);
  }
  
  void Output(std::ofstream& stream) const {
    stream << "--------------------------------------------" << std::endl;
    for (const auto block : blocks_) {
      stream << block->GetLabel().ToString() << std::endl;
    }
    stream << "--------------------------------------------" << std::endl;
  }
  
 private:
  std::vector<Block*> blocks_;
};

}