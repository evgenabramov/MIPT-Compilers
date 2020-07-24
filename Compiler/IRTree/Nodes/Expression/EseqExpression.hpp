#pragma once

#include "../Statement/Statement.hpp"
#include "Expression.hpp"

namespace irt {

class EseqExpression : public Expression {
 public:
  EseqExpression(Statement* statement, Expression* expression)
      : statement_(statement), expression_(expression) {}

  void Accept(Visitor* visitor) override {
    visitor->Visit(this);
  }

  bool IsEseqExpression() const override {
    return true;
  }

  Statement* statement_;
  Expression* expression_;
};

}


