#pragma once

#include "Type.hpp"

namespace ast {

class ArrayType : public Type {
 public:
  explicit ArrayType(const std::string& identifier) {
      this->identifier_ = identifier;
  }

  void Accept(Visitor* visitor) override {
      visitor->Visit(this);
  }

  bool IsSimpleType() const override {
      return false;
  }
};

} // namespace ast
