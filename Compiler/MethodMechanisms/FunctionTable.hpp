#pragma once

#include "Symbol.hpp"
#include "Object.hpp"

#include <stack>
#include <iostream>

namespace ast {

// There is a common stack of values for all variables.
// This class allows to keep variables' indices on that stack for current scope.
class FunctionTable {
 public:
  // Set new index for variable
  void Put(Symbol symbol, int index) {
    std::cout << "\'" << symbol.GetName() << "\' index on stack is " << index << std::endl;
    if (values_.find(symbol) == values_.end()) {
      throw std::runtime_error("\'" + symbol.GetName() + "\': variable not declared");
    }
    values_[symbol].pop();
    values_[symbol].push(index);
    symbols_.push(symbol);
  }

  // Get index for variable by name
  int Get(Symbol symbol) {
    if (values_.find(symbol) == values_.end()) {
      throw std::runtime_error("\'" + symbol.GetName() + "\': variable not declared");
    }
    return values_[symbol].top();
  }

  void CreateVariable(Symbol symbol) {
    std::cerr << "Creating variable \'" << symbol.GetName() << "\'" << std::endl;
    if (values_.find(symbol) == values_.end()) {
      values_[symbol] = std::stack<int>();
    }
    values_[symbol].push(0); // default initial index
  }

  void BeginScope() {
    symbols_.push(Symbol("{"));
  }

  void EndScope() {
    while (symbols_.top() != Symbol("{")) {
      Symbol symbol = symbols_.top();

      std::cerr << "Popping \'" << symbol.GetName()
        << "\' index on stack while exiting scope..." << std::endl;

      values_[symbol].pop();
      if (values_[symbol].empty()) {
        values_.erase(symbol);
      }
      symbols_.pop();
    }
    symbols_.pop();
  }

 private:
  // variable -> stack of indices in scopes
  std::unordered_map<Symbol, std::stack<int>> values_;
  // stack of variables in order they are declared
  std::stack<Symbol> symbols_;
};

} // namespace ast