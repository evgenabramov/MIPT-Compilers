#pragma once

#include "Expression.hpp"

namespace irt {

class MemExpression : public Expression {
 public:
  explicit MemExpression(Expression* expression) : expression_(expression) {}

  void Accept(Visitor* visitor) override {
    visitor->Visit(this);
  }
  
  bool IsMemExpression() const override {
    return true;
  }

  Expression* expression_;
};

}


