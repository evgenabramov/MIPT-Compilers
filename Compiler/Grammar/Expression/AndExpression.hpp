#pragma once

#include "BoolExpression.hpp"

namespace ast {

class AndExpression : public Expression {
 public:
  AndExpression(Expression* left, Expression* right)
      : left_(left), right_(right) {}

  void Accept(Visitor* visitor) override {
      visitor->Visit(this);
  }

  Expression* GetLeftExpression() const {
      return left_;
  }

  Expression* GetRightExpression() const {
      return right_;
  }

  Expression* left_;
  Expression* right_;
};

} // namespace ast