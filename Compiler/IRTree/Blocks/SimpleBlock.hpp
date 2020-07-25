#pragma once

#include "Block.hpp"

#include <iostream>

namespace irt {

class SimpleBlock : public Block {
 public:
  SimpleBlock(Label label, Statement* statement, Label next_block_label)
      : Block(label, statement), next_block_label_(std::move(next_block_label)) {
    std::cout << "new SimpleBlock label: " << label_.ToString() << std::endl;
  }
  
  bool IsSimple() const override {
    return true;
  }
  
  Block* GetNextBlock() const {
    return next_block_;
  }
  
  void SetNextBlock(Block* next_block) {
    next_block_ = next_block;
  }
  
  Label GetNextBlockLabel() const {
    return next_block_label_;
  }
 
 private:
  Label next_block_label_;
  Block* next_block_ = nullptr;
};

}