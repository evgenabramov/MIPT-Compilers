#pragma once

#include "../Generators/Temporary.h"

#include <unordered_map>
#include <unordered_set>

namespace irt {

class InterferenceGraph {
 public:
  InterferenceGraph() = default;
  
  void Output(const std::string& filename) const {
    std::ofstream stream(filename);
    
    for (const auto& [temporary, next_temps] : edges_) {
      stream << temporary.ToString() << ": " << std::endl;
      for (const auto& next_temp : next_temps) {
        stream << next_temp.ToString() << " ";
      }
      stream << std::endl << std::endl;
    }
  }
  
  void AddEdge(const Temporary& from, const Temporary& to) {
    if (edges_[from].find(to) != edges_[from].end()) {
      return;
    }
    edges_[from].insert(to);
    edges_[to].insert(from);
  }
 
 private:
  std::unordered_map<Temporary, std::unordered_set<Temporary>> edges_;
};

}