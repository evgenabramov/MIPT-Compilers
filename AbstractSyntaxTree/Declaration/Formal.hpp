#pragma once

#include "TreeNode.hpp"
#include "Type.hpp"

#include <string>
#include <memory>

namespace ast {

class Formal : public TreeNode {
 public:
  Formal(Type* type, std::string identifier) : type_(type), identifier_(std::move(identifier)) {}

  void Accept(Visitor* visitor) override {
      visitor->Visit(this);
  }

  Type* GetType() const {
      return type_;
  }

  const std::string& GetIdentifier() const {
      return identifier_;
  }

 private:
  Type* type_;
  std::string identifier_;
};

} // namespace ast