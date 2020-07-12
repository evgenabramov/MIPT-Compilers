#pragma once

#include "Expression.hpp"

namespace ast {

class ThisExpression : public Expression {
 public:
  ThisExpression() = default;

  void Accept(Visitor* visitor) override {
    visitor->Visit(this);
  }
};

} // namespace ast