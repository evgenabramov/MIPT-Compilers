#pragma once

#include "Symbol.hpp"

#include "MemberType.hpp"
#include "ClassType.hpp"
#include "MethodType.hpp"

#include "PrimitiveSimpleType.hpp"
#include "PrimitiveArrayType.hpp"

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <fstream>
#include <iostream>

namespace ast {

class ScopeLayer {
 public:
  ScopeLayer() = default;
  explicit ScopeLayer(ScopeLayer* parent) : parent_(parent) {
    parent_->AddChild(this);
  }

  ~ScopeLayer() {
    for (ScopeLayer* child : children_) {
      delete child;
    }
  }

  std::shared_ptr<PrimitiveSimpleType> DeclareSimpleVariable(Symbol symbol, SimpleType* simple_type) {
    if (Has(symbol)) {
      std::cout << symbol.GetName() << std::endl;
      throw std::runtime_error("Simple variable name already taken");
    }

    offsets_[symbol] = symbols_.size();
    symbols_.push_back(symbol);

    auto simple_variable_type = std::make_shared<PrimitiveSimpleType>(simple_type);
    members_[symbol] = simple_variable_type;
    return simple_variable_type;
  }

  std::shared_ptr<PrimitiveArrayType> DeclareArrayVariable(Symbol symbol, ArrayType* array_type) {
    if (Has(symbol)) {
      throw std::runtime_error("Array name already taken");
    }

    offsets_[symbol] = symbols_.size();
    symbols_.push_back(symbol);

    auto array_variable_type = std::make_shared<PrimitiveArrayType>(array_type);
    members_[symbol] = array_variable_type;
    return array_variable_type;
  }

  std::shared_ptr<MethodType> DeclareMethod(Symbol symbol, MethodDeclaration* method_declaration) {
    if (Has(symbol)) {
      throw std::runtime_error("Method name already taken");
    }

    offsets_[symbol] = symbols_.size();
    symbols_.push_back(symbol);

    auto method_type = std::make_shared<MethodType>(method_declaration);
    members_[symbol] = method_type;
    return method_type;
  }

  std::shared_ptr<ClassType> DeclareClass(Symbol symbol, ClassDeclaration* class_declaration) {
    if (Has(symbol)) {
      throw std::runtime_error("Class name already taken");
    }

    offsets_[symbol] = symbols_.size();
    symbols_.push_back(symbol);

    auto class_type = std::make_shared<ClassType>(class_declaration);
    members_[symbol] = class_type;
    return class_type;
  }

  // Shadow variable type by name
  void Put(Symbol symbol, std::shared_ptr<MemberType> value) {
    ScopeLayer* current_layer = this;
    // go up in tree
    while (!current_layer->Has(symbol) && current_layer->parent_ != nullptr) {
      current_layer = current_layer->parent_;
    }
    // shadow variable type
    if (current_layer->Has(symbol)) {
      current_layer->members_[symbol] = std::move(value);
    } else {
      throw std::runtime_error("Variable not declared");
    }
  }

  // Get variable/method type by symbol
  std::shared_ptr<MemberType> Get(Symbol symbol) {
    ScopeLayer* current_layer = this;
    // go up in tree
    while (!current_layer->Has(symbol) && current_layer->parent_ != nullptr) {
      current_layer = current_layer->parent_;
    }
    if (current_layer->Has(symbol)) {
      return current_layer->members_[symbol];
    } else {
      throw std::runtime_error("variable not declared");
    }
  }

  void PrintLayer(std::ofstream& out, size_t num_tabs = 0) const {
    for (auto& symbol : symbols_) {
      PrintTabs(out, num_tabs);
      out << symbol.GetName() << std::endl;
    }

    for (ScopeLayer* child : children_) {
      child->PrintLayer(out, num_tabs + 1);
    }
  }

  void PrintLayer(size_t num_tabs = 0) const {
    for (auto& symbol : symbols_) {
      PrintTabs(num_tabs);
      std::cout << symbol.GetName() << std::endl;
    }

    PrintTabs(num_tabs);
    std::cout << "Num children of scope: " << children_.size() << std::endl;

    for (ScopeLayer* child : children_) {
      child->PrintLayer(num_tabs + 1);
      std::cout << std::endl;
    }
  }

  void AddChild(ScopeLayer* child) {
    children_.push_back(child);
  }

  ScopeLayer* GetParent() const {
    return parent_;
  }

  // Check access to variable in scope
  bool Has(Symbol symbol) const {
    return (members_.find(symbol) != members_.end());
  }

  ScopeLayer* GetChild(size_t index) const {
    return children_[index];
  }

 private:
  void PrintTabs(std::ofstream& out, size_t num_tabs) const {
    for (size_t i = 0; i < num_tabs; ++i) {
      out << "\t";
    }
  }

  void PrintTabs(size_t num_tabs) const {
    for (size_t i = 0; i < num_tabs; ++i) {
      std::cout << "\t";
    }
  }

  std::unordered_map<Symbol, std::shared_ptr<MemberType>> members_;

  // Save order of symbols
  std::unordered_map<Symbol, size_t> offsets_;
  std::vector<Symbol> symbols_;

  ScopeLayer* parent_ = nullptr;
  std::vector<ScopeLayer*> children_;
};

} // namespace ast