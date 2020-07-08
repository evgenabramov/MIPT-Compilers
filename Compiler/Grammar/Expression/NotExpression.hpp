#pragma once

#include "Expression.hpp"

namespace ast {

class NotExpression : public Expression {
 public:
  explicit NotExpression(Expression* expression) : expression_(expression) {}

  void Accept(Visitor* visitor) override {
    visitor->Visit(this);
  }

  Expression* GetExpression() const {
    return expression_;
  }

  Expression* expression_;
};

} // namespace ast