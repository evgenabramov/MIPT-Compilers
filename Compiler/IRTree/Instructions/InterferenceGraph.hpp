#pragma once

#include "../Generators/Temporary.h"
#include "ControlFlowGraph.hpp"

#include <unordered_map>
#include <unordered_set>
#include <set>
#include <stack>

namespace irt {

class ControlFlowGraph;

class InterferenceGraph {
 public:
  explicit InterferenceGraph(size_t num_temporaries,
                             std::unordered_map<Temporary, size_t> temp_to_position,
                             std::unordered_map<size_t, Temporary> position_to_temp,
                             std::vector<Instruction> instructions, size_t frame_size)
      : num_temporaries_(num_temporaries),
        temp_to_position_(std::move(temp_to_position)),
        position_to_temp_(std::move(position_to_temp)),
        instructions_(std::move(instructions)),
        frame_size_(frame_size) {
    nodes_.resize(num_temporaries);
    adjacency_matrix_.resize(num_temporaries, std::vector<bool>(num_temporaries, false));
    
    for (size_t index = 0; index + 1 < significant_degree_; ++index) {
      size_t position = temp_to_position_.at(Temporary("r" + std::to_string(index)));
      precolored_indices_.insert(position);
      nodes_[position].type_ = NodeType::Precolored;
      nodes_[position].color_ = Temporary("r" + std::to_string(index));
    }
    size_t position = temp_to_position_.at(Temporary("fp"));
    precolored_indices_.insert(position);
    nodes_[position].type_ = NodeType::Precolored;
    nodes_[position].color_ = Temporary("fp");
    
    for (size_t index = 0; index < num_temporaries; ++index) {
      if (precolored_indices_.find(index) == precolored_indices_.end()) {
        initial_indices_.insert(index);
        nodes_[index].type_ = NodeType::Initial;
      }
    }
  }
  
  void Output(const std::string& filename) const {
    std::ofstream stream(filename);
    
    for (size_t index = 0; index < nodes_.size(); ++index) {
      stream << position_to_temp_.at(index).ToString() << ": " << std::endl;
      for (size_t next_index : nodes_[index].neighbours_) {
        stream << position_to_temp_.at(next_index).ToString() << " ";
      }
      stream << std::endl << std::endl;
    }
  }
  
  void AddEdge(const Temporary& from, const Temporary& to) {
    size_t from_position = temp_to_position_[from];
    size_t to_position = temp_to_position_[to];
    
    nodes_[from_position].neighbours_.insert(to_position);
    nodes_[to_position].neighbours_.insert(from_position);
    ++nodes_[from_position].degree_;
    ++nodes_[to_position].degree_;
    
    adjacency_matrix_[from_position][to_position] = true;
    adjacency_matrix_[to_position][from_position] = true;
  }
  
  bool ColorNodes() {
    bool should_retry_pipeline = false;
    MakeWorklist();
    do {
      if (!simplify_worklist_.empty()) {
        Simplify();
      } else if (!spill_worklist_.empty()) {
        SelectSpill();
      }
    } while (!(simplify_worklist_.empty() && spill_worklist_.empty()));
    AssignColors();
    if (!spilled_indices_.empty()) {
      RewriteProgram();
      should_retry_pipeline = true;
    } else {
      UpdateInstructions();
    }
    return should_retry_pipeline;
  }
  
  size_t GetFrameSize() const {
    return frame_size_;
  }
  
  std::vector<Instruction> GetInstructions() const {
    return instructions_;
  }
 
 private:
  void MakeWorklist() {
    for (size_t index : initial_indices_) {
      if (nodes_[index].degree_ >= significant_degree_) {
        spill_worklist_.insert(index);
      } else {
        simplify_worklist_.insert(index);
      }
    }
    initial_indices_.clear();
  }
  
  void Simplify() {
    size_t simplify_index = *simplify_worklist_.begin();
    select_stack_.push(position_to_temp_.at(simplify_index));
    for (size_t neighbour_index : nodes_[simplify_index].neighbours_) {
      --nodes_[neighbour_index].degree_;
    }
    simplify_worklist_.erase(simplify_index);
  }
  
  void SelectSpill() {
    size_t spill_index = *spill_worklist_.begin();
    simplify_worklist_.insert(spill_index);
    spill_worklist_.erase(spill_index);
  }
  
  void AssignColors() {
    while (!select_stack_.empty()) {
      size_t top_index = temp_to_position_.at(select_stack_.top());
      select_stack_.pop();
      
      std::unordered_set<Temporary> used_colors;
      for (size_t neighbour_index : nodes_[top_index].neighbours_) {
        if (precolored_indices_.find(neighbour_index) != precolored_indices_.end()
            || colored_indices_.find(neighbour_index) != colored_indices_.end()) {
          used_colors.insert(nodes_[neighbour_index].color_);
        }
      }
      for (size_t index = 0; index < significant_degree_; ++index) {
        Temporary color("r" + std::to_string(index));
        if (index + 1 == significant_degree_) {
          color = Temporary("fp");
        }
        if (used_colors.find(color) == used_colors.end()) {
          colored_indices_.insert(top_index);
          nodes_.at(top_index).color_ = color;
          break;
        }
      }
      if (colored_indices_.find(top_index) == colored_indices_.end()) {
        spilled_indices_.insert(top_index);
      }
    }
  }
  
  void RewriteProgram() {
    size_t num_spilled_temps = spilled_indices_.size();
    // Frame size should be increased
    for (auto& instruction : instructions_) {
      std::string instruction_str = instruction.GetStr();
      
      if (instruction_str.find("sub sp, sp, #") != std::string::npos) {
        size_t new_frame_size = frame_size_ + num_spilled_temps * 4;
        instruction = Instruction("sub sp, sp, #" + std::to_string(new_frame_size), {}, {});
      }
    }
    
    for (size_t spilled_index : spilled_indices_) {
      std::vector<Instruction> new_instructions;
      Temporary spilled_temp = position_to_temp_[spilled_index];
      
      for (const Instruction& instruction : instructions_) {
        std::vector<Temporary> targets = instruction.GetTargets();
        std::vector<Temporary> sources = instruction.GetSources();
        
        for (const Temporary& source_temp : sources) {
          if (spilled_temp == source_temp) {
            new_instructions.push_back(
                Instruction(
                    "ldr t0, s0, #" + std::to_string(-(frame_size_ + spilled_index * 4)),
                    {spilled_temp},
                    {Temporary("fp")}
                )
            );
            new_instructions.push_back(instruction);
            continue;
          }
        }
        for (const Temporary& target_temp : targets) {
          if (spilled_temp == target_temp) {
            new_instructions.push_back(instruction);
            new_instructions.push_back(
                Instruction(
                    "str s0, [s1, #" + std::to_string(-(frame_size_ + spilled_index * 4)),
                    {},
                    {spilled_temp, Temporary("fp")}
                )
            );
            continue;
          }
        }
        new_instructions.push_back(instruction);
      }
      instructions_ = new_instructions;
    }
    frame_size_ = frame_size_ + num_spilled_temps * 4;
    spilled_indices_.clear();
  }
  
  void UpdateInstructions() {
    std::vector<Instruction> new_instructions;
    std::unordered_set<Temporary> used_registers;
    for (Instruction& instruction : instructions_) {
      std::vector<Temporary> targets = std::move(instruction.GetTargets());
      std::vector<Temporary> sources = std::move(instruction.GetSources());
    
      // Replace temporaries with machine registers
      for (Temporary& target : targets) {
        target = nodes_[temp_to_position_.at(target)].color_;
        used_registers.insert(target);
      }
      for (Temporary& source : sources) {
        source = nodes_[temp_to_position_.at(source)].color_;
        used_registers.insert(source);
      }
    
      // Remove unnecessary moves
      if (instruction.GetStr().substr(0, 4) == "mov " && targets.size() == 1
          && sources.size() == 1 && targets[0] == sources[0]) {
        continue;
      }
      new_instructions.emplace_back(instruction.GetStr(), targets, sources);
    }
  
    instructions_ = std::move(new_instructions);
  
    std::string registers_list;
  
    for (size_t index = 4; index + 1 < significant_degree_; ++index) {
      Temporary register_temp("r" + std::to_string(index));
      if (used_registers.find(register_temp) != used_registers.end()) {
        if (registers_list.empty()) {
          registers_list = register_temp.ToString();
        } else {
          registers_list += ", " + register_temp.ToString();
        }
      }
    }
  
    registers_list = (registers_list.empty()) ? "lr" : registers_list + ", lr";
  
    // Save callee-save registers
    for (Instruction& instruction : instructions_) {
      if (instruction.GetStr().substr(0, 5) == "push ") {
        instruction = Instruction("push {" + registers_list + "}", {}, {});
        break;
      }
    }
    // Restore callee-save registers
    for (int index = static_cast<int>(instructions_.size()) - 1; index >= 0; --index) {
      Instruction& instruction = instructions_[index];
      if (instruction.GetStr().substr(0, 4) == "pop ") {
        instruction = Instruction("pop {" + registers_list + "}", {}, {});
        break;
      }
    }
  }
  
  enum class NodeType {
    Initial, Precolored, Simplify, Spill, Spilled, Colored
  };
  
  struct Node {
    NodeType type_;
    Temporary color_;
    
    size_t degree_;
    std::set<size_t> neighbours_;
  };
  
  size_t frame_size_;
  std::vector<Instruction> instructions_;
  
  size_t num_temporaries_;
  
  const size_t significant_degree_ = 12; // number of machine registers
  
  std::vector<Node> nodes_;
  std::vector<std::vector<bool>> adjacency_matrix_;
  
  std::unordered_map<Temporary, size_t> temp_to_position_;
  std::unordered_map<size_t, Temporary> position_to_temp_;

// Next sets are disjoint
  std::unordered_set<size_t> precolored_indices_;
  std::set<size_t> initial_indices_;
  std::set<size_t> simplify_worklist_;
// TODO: freeze_worklist_, coalesced_nodes_
  std::set<size_t> spill_worklist_;
  std::set<size_t> spilled_indices_;
  std::set<size_t> colored_indices_;
  
  std::stack<Temporary> select_stack_;
};

}