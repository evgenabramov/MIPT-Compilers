#pragma once

#include "Instruction.hpp"
#include "../Generators/Label.h"
#include "InterferenceGraph.hpp"

#include <bitset>
#include <unordered_map>
#include <unordered_set>
#include <cmath>
#include <vector>
#include <algorithm>
#include <set>

namespace irt {

static std::unordered_map<Temporary, size_t> temp_to_position;
static std::unordered_map<size_t, Temporary> position_to_temp;

static const size_t kChunkSize = 32;

class ControlFlowNode {
  friend class ControlFlowGraph;
 
 public:
  ControlFlowNode() = default;
  
  ControlFlowNode(Instruction instruction, size_t index)
      : instruction_(std::move(instruction)),
        index_(index) {
  }
  
  void SetNumTemporaries(size_t num_temporaries) {
    size_t num_chunks = ceil(static_cast<double>(num_temporaries) / kChunkSize);
    
    used_.resize(num_chunks);
    defined_.resize(num_chunks);
    in_.resize(num_chunks);
    out_.resize(num_chunks);
    
    ResetTemporaries(used_);
    ResetTemporaries(defined_);
    ResetTemporaries(in_);
    ResetTemporaries(out_);
    
    SetTemporaries(defined_, instruction_.GetTargets());
    SetTemporaries(used_, instruction_.GetSources());
  }
  
  void AddNextNode(ControlFlowNode* next_node) {
    next_.insert(next_node);
  }
  
  void AddPrevNode(ControlFlowNode* prev_node) {
    prev_.insert(prev_node);
  }
  
  std::vector<Temporary> GetDefinedTemporaries() const {
    return std::move(GetTemporaries(defined_));
  }
  
  std::vector<Temporary> GetOutTemporaries() const {
    return std::move(GetTemporaries(out_));
  }
  
  bool operator<(const ControlFlowNode& other) const {
    return index_ < other.index_;
  }
 
 private:
  static std::vector<Temporary> GetTemporaries(const std::vector<std::bitset<kChunkSize>>& chunks) {
    std::vector<Temporary> result;
    for (size_t chunk_index = 0; chunk_index < chunks.size(); ++chunk_index) {
      for (size_t offset = 0; offset < kChunkSize; ++offset) {
        if (chunks[chunk_index][offset]) {
          result.push_back(position_to_temp[chunk_index * kChunkSize + offset]);
        }
      }
    }
    return std::move(result);
  }
  
  static void ResetTemporaries(std::vector<std::bitset<kChunkSize>>& chunks) {
    for (auto& chunk : chunks) {
      chunk.reset();
    }
  }
  
  static void SetTemporaries(std::vector<std::bitset<kChunkSize>>& chunks,
                             const std::vector<Temporary>& temporaries) {
    for (const Temporary& source_temp : temporaries) {
      size_t position = temp_to_position[source_temp];
      size_t chunk_index = position / kChunkSize;
      size_t offset = position % kChunkSize;
      chunks[chunk_index][offset] = true;
    }
  }
  
  Instruction instruction_; // assembly instruction for the node
  
  // For graph navigation
  std::set<ControlFlowNode*> prev_;
  std::set<ControlFlowNode*> next_;
  
  std::vector<std::bitset<kChunkSize>> defined_; // variables defined in the instruction
  std::vector<std::bitset<kChunkSize>> used_; // variables used in the instruction
  
  std::vector<std::bitset<kChunkSize>> in_; // set of variables which are live on any of incoming edges
  std::vector<std::bitset<kChunkSize>> out_; // set of variables which are live on any of outgoing edges
  
  size_t index_;
};

// TODO: add special treatment for mov instructions
// TODO: add special treatment for bl instructions
class ControlFlowGraph {
  using Node = ControlFlowNode;
 
 public:
  ControlFlowGraph(size_t frame_size, const std::vector<Instruction>& instructions)
      : frame_size_(frame_size), instructions_(instructions) {
    std::unordered_set<Temporary> saved_temporaries;
    std::unordered_map<Label, Node*> node_by_label;
    size_t position = 0;
    
    temp_to_position.clear();
    position_to_temp.clear();
    nodes_.resize(instructions.size());
    
    SaveMachineRegisters(saved_temporaries, position);
    
    for (size_t index = 0; index < instructions.size(); ++index) {
      const Instruction& instruction = instructions[index];
      nodes_[index] = std::move(Node(instruction, index));
      SaveTemporaries(saved_temporaries, instruction.GetTargets(), position);
      SaveTemporaries(saved_temporaries, instruction.GetSources(), position);
      
      std::string instruction_str = instruction.GetStr();
      
      if (instruction_str.back() == ':') {
        Label label(instruction_str.substr(0, instruction_str.length() - 1));
        node_by_label[label] = &nodes_[index];
      }
    }
    
    // Order edges
    for (size_t index = 0; index + 1 < instructions.size(); ++index) {
      std::string instruction_str = instructions[index].GetStr();
      if (instruction_str.substr(0, 2) != "b ") {
        nodes_[index].AddNextNode(&nodes_[index + 1]);
        nodes_[index + 1].AddPrevNode(&nodes_[index]);
      }
    }
    
    num_temporaries_ = saved_temporaries.size();
    
    // Label edges
    for (size_t index = 0; index < instructions.size(); ++index) {
      nodes_[index].SetNumTemporaries(num_temporaries_);
      std::string instruction_str = instructions[index].GetStr();
      
      if (instruction_str.front() == 'b' && instruction_str.substr(0, 3) != "bl "
         && instruction_str.substr(0, 3) != "bx ") {
        size_t label_position = instruction_str.find(' ') + 1;
        if (label_position == std::string::npos) {
          throw std::logic_error("Cannot find label in instruction: " + instruction_str);
        }
        
        Label label(instruction_str.substr(label_position));
        
        nodes_[index].AddNextNode(node_by_label[label]);
        node_by_label[label]->AddPrevNode(&nodes_[index]);
      }
    }
  }
  
  void OutputGraph(const std::string& filename) {
    std::ofstream stream(filename);
    for (size_t index = 0; index < nodes_.size(); ++index) {
      stream << "index: " << index << std::endl;
      nodes_[index].instruction_.Print(stream);
      
      stream << "Next nodes indices: ";
      for (Node* next_node : nodes_[index].next_) {
        stream << next_node->index_ << " ";
      }
      stream << std::endl;
      
      stream << "Previous nodes indices: ";
      for (Node* prev_node : nodes_[index].prev_) {
        stream << prev_node->index_ << " ";
      }
      stream << std::endl;
      
      std::vector<Temporary> used = nodes_[index].GetTemporaries(nodes_[index].used_);
      stream << "Used temporaries: ";
      for (const auto& temp : used) {
        stream << temp.ToString() << " ";
      }
      stream << std::endl;
      
      std::vector<Temporary> defined = nodes_[index].GetTemporaries(nodes_[index].defined_);
      stream << "Defined temporaries: ";
      for (const auto& temp : defined) {
        stream << temp.ToString() << " ";
      }
      stream << std::endl;
      
      std::vector<Temporary> in = nodes_[index].GetTemporaries(nodes_[index].in_);
      stream << "Live-in temporaries: ";
      for (const auto& temp : in) {
        stream << temp.ToString() << " ";
      }
      stream << std::endl;
      
      std::vector<Temporary> out = nodes_[index].GetTemporaries(nodes_[index].out_);
      stream << "Live-out temporaries: ";
      for (const auto& temp : out) {
        stream << temp.ToString() << " ";
      }
      
      stream << std::endl << std::endl;
    }
  }
  
  InterferenceGraph BuildInterferenceGraph() {
    InterferenceGraph interference_graph(
        num_temporaries_,
        temp_to_position,
        position_to_temp,
        instructions_,
        frame_size_
    );
    
    PreformLivenessAnalysis();
    
    for (const Node& node : nodes_) {
      const std::vector<Temporary> defined_temps = std::move(node.GetDefinedTemporaries());
      const std::vector<Temporary> out_temps = std::move(node.GetOutTemporaries());
      
      for (const Temporary& from : defined_temps) {
        for (const Temporary& to : out_temps) {
          if (from != to) {
            interference_graph.AddEdge(from, to);
          }
        }
      }
    }
    
    for (size_t index_from = 0; index_from + 1 < kNumMachineRegisters; ++index_from) {
      Temporary from_temp = Temporary("r" + std::to_string(index_from));
      for (size_t index_to = index_from + 1; index_to + 1 < kNumMachineRegisters; ++index_to) {
        Temporary to_temp = Temporary("r" + std::to_string(index_to));
        interference_graph.AddEdge(from_temp, to_temp);
      }
      interference_graph.AddEdge(from_temp, Temporary("fp"));
    }
    
    return std::move(interference_graph);
  }
 
 private:
  void PreformLivenessAnalysis() {
    std::vector<size_t> order = GetReversedTopologicalOrder();
    
    bool is_same;
    size_t num_iterations = 0;
    do {
      ++num_iterations;
      is_same = true;
      for (size_t index : order) {
        Node& node = nodes_[index];
        
        std::vector<std::bitset<kChunkSize>> old_out = node.out_;
        std::vector<std::bitset<kChunkSize>> old_in = node.in_;
        
        // Dataflow equation for node.out[]
        for (Node* next_node : node.next_) {
          for (size_t chunk_index = 0; chunk_index < node.out_.size(); ++chunk_index) {
            node.out_[chunk_index] |= next_node->in_[chunk_index];
          }
        }
        
        // Dataflow equation for node.in[]
        for (size_t chunk_index = 0; chunk_index < node.in_.size(); ++chunk_index) {
          node.in_[chunk_index] = node.used_[chunk_index]
              | (node.out_[chunk_index] & (~node.defined_[chunk_index]));
        }
        
        if (old_in != node.in_ || old_out != node.out_) {
          is_same = false;
        }
      }
    } while (!is_same);
    std::cout << "Num iterations: " << num_iterations << std::endl;
  }
  
  std::vector<size_t> GetReversedTopologicalOrder() const {
    std::vector<size_t> order;
    std::vector<bool> is_used(nodes_.size(), false);
    
    for (size_t index = 0; index < nodes_.size(); ++index) {
      if (!is_used[index]) {
        ReverseDFS(index, order, is_used);
      }
    }
    
    std::reverse(order.begin(), order.end());
    return order;
  }
  
  void ReverseDFS(size_t index,
                  std::vector<size_t>& order,
                  std::vector<bool>& is_used) const {
    is_used[index] = true;
    for (Node* prev_node: nodes_[index].prev_) {
      if (!is_used[prev_node->index_]) {
        ReverseDFS(prev_node->index_, order, is_used);
      }
    }
    order.push_back(index);
  }
  
  static void SaveTemporaries(std::unordered_set<Temporary>& saved_temporaries,
                              const std::vector<Temporary>& temporaries,
                              size_t& position) {
    for (const Temporary& temporary : temporaries) {
      if (saved_temporaries.find(temporary) == saved_temporaries.end()) {
        temp_to_position[temporary] = position;
        position_to_temp[position] = temporary;
        ++position;
        saved_temporaries.insert(temporary);
      }
    }
  }
  
  static void SaveMachineRegisters(std::unordered_set<Temporary>& saved_temporaries, size_t& position) {
    std::vector<Temporary> machine_registers;
    for (size_t index = 0; index + 1 < kNumMachineRegisters; ++index) {
      machine_registers.emplace_back("r" + std::to_string(index));
    }
    SaveTemporaries(saved_temporaries, machine_registers, position);
  }
  
  size_t num_temporaries_;
  static const size_t kNumMachineRegisters = 12;
  
  std::vector<Node> nodes_;
  
  size_t frame_size_;
  std::vector<Instruction> instructions_;
};

}