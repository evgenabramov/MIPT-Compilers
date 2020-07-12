#pragma once

#include "TreeNode.hpp"

namespace ast {

class Declaration : public TreeNode {
 public:
  virtual ~Declaration() = default;
  void Accept(Visitor* visitor) override {
    visitor->Visit(this);
  }
};

} // namespace ast