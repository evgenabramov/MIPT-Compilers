#pragma once

#include "TreeNode.hpp"

namespace ast {

class Expression : public TreeNode {
 public:
  virtual ~Expression() = default;
  void Accept(Visitor* visitor) override {
    visitor->Visit(this);
  }
};

} // namespace ast