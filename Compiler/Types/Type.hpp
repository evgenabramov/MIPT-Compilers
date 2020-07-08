#pragma once

#include "TreeNode.hpp"

#include <string>

namespace ast {

class Type : public TreeNode {
 public:
  Type() = default;
  explicit Type(std::string identifier) = delete;

  void Accept(Visitor* visitor) override {
      visitor->Visit(this);
  }

  virtual bool IsSimpleType() const = 0;

  const std::string& GetIdentifier() const {
      return identifier_;
  }

 protected:
  std::string identifier_;
};

} // namespace ast