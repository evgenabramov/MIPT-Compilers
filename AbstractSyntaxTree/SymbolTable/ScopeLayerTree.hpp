#pragma once

#include "ScopeLayer.hpp"

#include <unordered_map>

namespace ast {

// Logic of both Table and ScopeLayerTree from:
// https://github.com/akhtyamovpavel/CompilersCourse/blob/30c03655943e234f59b951429dc16bbc228a0041/05-variable-scopes/symbol_table/Table.h#L11
class ScopeLayerTree {
 public:
  ScopeLayerTree() = default;
  explicit ScopeLayerTree(ScopeLayer* root) : root_(root) {}

  std::shared_ptr<ObjectType> Get(Symbol symbol) {
      return root_->Get(symbol);
  }

  // Map function or class scope with label
  void LabelScope(Symbol label, ScopeLayer* scope_layer) {
      if (scopes_.find(label) != scopes_.end()) {
          throw std::runtime_error("Label for scope already taken");
      }

      scopes_[label] = scope_layer;
  }

  ScopeLayer* GetScopeLayer(Symbol label) {
      if (scopes_.find(label) == scopes_.end()) {
          throw std::runtime_error("Label for scope not found");
      }

      return scopes_[label];
  }

  ScopeLayer* GetRoot() const {
      return root_;
  }

 private:
  ScopeLayer* root_ = nullptr;
  std::unordered_map<Symbol, ScopeLayer*> scopes_;
};

}