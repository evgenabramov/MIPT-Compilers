#pragma once

#include "Block.hpp"

namespace irt {

class ConditionalBlock : public Block {
 public:
  ConditionalBlock(Label label, Statement* statement, Label true_block_label, Label false_block_label)
      : Block(label, statement), true_block_label_(std::move(true_block_label)),
        false_block_label_(std::move(false_block_label)) {
    std::cout << "new ConditionalBlock label: " << label_.ToString() << std::endl;
  }
  
  bool IsSimple() const override {
    return false;
  }
  
  Block* GetTrueBlock() const {
    return true_block_;
  }
  
  Block* GetFalseBlock() const {
    return false_block_;
  }
  
  void SetNextBlocks(Block* true_block, Block* false_block) {
    true_block_ = true_block;
    false_block_ = false_block;
  }
  
  Label GetTrueBlockLabel() const {
    return true_block_label_;
  }
  
  Label GetFalseBlockLabel() const {
    return false_block_label_;
  }
 
 private:
  Label true_block_label_;
  Label false_block_label_;
  Block* true_block_ = nullptr;
  Block* false_block_ = nullptr;
};

}