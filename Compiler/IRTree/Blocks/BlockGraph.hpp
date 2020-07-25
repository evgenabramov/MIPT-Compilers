#pragma once

#include "SimpleBlock.hpp"
#include "ConditionalBlock.hpp"
#include "Trace.hpp"

#include <vector>

#include <fstream>
#include <unordered_map>

namespace irt {

class BlockGraph {
 public:
  explicit BlockGraph(Block* root) : root_(root) {}
  
  Block* GetRoot() const {
    return root_;
  }
  
  std::vector<Trace*> GetTraces() {
    TraverseDFS(root_);
    return traces_;
  }
  
  void TraverseDFS(Block* block, Trace* current_trace = nullptr) {
    if (block == nullptr || visited_[block]) {
      return;
    } else if (current_trace == nullptr) {
      traces_.push_back(new Trace());
      current_trace = traces_.back();
    }
    
    visited_[block] = true;
    current_trace->AddBlock(block);
    block->SetHasTrace();
    
    if (block->IsSimple()) {
      auto simple_block = dynamic_cast<SimpleBlock*>(block);
      TraverseDFS(simple_block->GetNextBlock(), current_trace);
    } else {
      auto conditional_block = dynamic_cast<ConditionalBlock*>(block);
      TraverseDFS(conditional_block->GetFalseBlock(), current_trace);
      TraverseDFS(conditional_block->GetTrueBlock());
    }
  }
  
  void TraverseOutput(Block* block) {
    if (block == nullptr || visited_[block]) {
      return;
    }
    visited_[block] = true;
    stream_ << block->GetLabel().ToString();
    
    if (block->IsSimple()) {
      auto simple_block = dynamic_cast<SimpleBlock*>(block);
      stream_ << " -> [next]: " << ((simple_block->GetNextBlock() != nullptr) ?
                                    simple_block->GetNextBlock()->GetLabel().ToString() : "null")
              << std::endl;
      TraverseOutput(simple_block->GetNextBlock());
    } else {
      auto conditional_block = dynamic_cast<ConditionalBlock*>(block);
      stream_ << " -> [false]: " << ((conditional_block->GetFalseBlock() != nullptr) ?
                                     conditional_block->GetFalseBlock()->GetLabel().ToString() : "null")
              << " [true]: " << ((conditional_block->GetTrueBlock() != nullptr) ?
                                 conditional_block->GetTrueBlock()->GetLabel().ToString() : "null")
              << std::endl;
      TraverseOutput(conditional_block->GetFalseBlock());
      TraverseOutput(conditional_block->GetTrueBlock());
    }
  }
  
  void OutputGraph(const std::string& method_name, const std::string& suffix) {
    stream_ = std::move(std::ofstream(method_name + suffix));
    stream_ << "Blocks for method \'" << method_name << "\':" << std::endl;
    visited_.clear();
    TraverseOutput(root_);
  }
  
  void OutputTraces(const std::string& method_name, const std::string suffix) {
    stream_ = std::move(std::ofstream(method_name + suffix));
    stream_ << "Traces for method \'" << method_name << "\':" << std::endl;
    visited_.clear();
    traces_ = GetTraces();
    for (const auto& trace : traces_) {
      trace->Output(stream_);
    }
  }
 
 private:
  Block* root_;
  std::vector<Trace*> traces_;
  
  std::ofstream stream_;
  std::unordered_map<Block*, bool> visited_;
};

}