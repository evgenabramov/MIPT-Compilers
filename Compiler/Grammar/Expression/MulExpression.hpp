#pragma once

#include "Expression.hpp"

namespace ast {

class MulExpression : public Expression {
 public:
  MulExpression(Expression* left, Expression* right) : left_(left), right_(right) {}

  void Accept(Visitor* visitor) override {
    visitor->Visit(this);
  }

  Expression* GetLeftExpression() const {
    return left_;
  }

  void SetLeftExpression(Expression* left) {
    left_ = left;
  }

  Expression* GetRightExpression() const {
    return right_;
  }

  void SetRightExpression(Expression* right) {
    right_ = right;
  }

 private:
  Expression* left_;
  Expression* right_;
};

} // namespace ast