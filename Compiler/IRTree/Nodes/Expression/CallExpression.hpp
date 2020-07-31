#pragma once

#include "Expression.hpp"

namespace irt {

class CallExpression : public Expression {
 public:
  CallExpression(Expression* expression, ExpressionList* args)
      : function_name_(expression), args_(args) {}

  void Accept(Visitor* visitor) override {
    visitor->Visit(this);
  }
  
  bool IsCallExpression() const override {
    return true;
  }

  Expression* function_name_;
  ExpressionList* args_;
};

}


