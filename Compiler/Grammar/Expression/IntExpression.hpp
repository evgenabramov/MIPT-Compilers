#pragma once

#include "Expression.hpp"

namespace ast {

class IntExpression : public Expression {
 public:
  explicit IntExpression(const int value) : value_(value) {}

  void Accept(Visitor* visitor) override {
    visitor->Visit(this);
  }

  int GetValue() const {
    return value_;
  }

 private:
  int value_;
};

} // namespace ast