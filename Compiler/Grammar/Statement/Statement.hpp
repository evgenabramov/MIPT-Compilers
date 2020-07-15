#pragma once

#include "TreeNode.hpp"

namespace ast {

class Statement : public TreeNode {
 public:
  virtual ~Statement() = default;

  void Accept(Visitor* visitor) override {
    visitor->Visit(this);
  }
};

} // namespace ast