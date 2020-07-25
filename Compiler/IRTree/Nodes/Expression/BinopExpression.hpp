#pragma once

#include "Expression.hpp"
#include "../../Types/BinaryOperatorType.hpp"

namespace irt {

class BinopExpression : public Expression {
 public:
  BinopExpression(BinaryOperatorType type, Expression* first, Expression* second)
      : operator_type_(type), first_(first), second_(second) {}

  void Accept(Visitor* visitor) override {
    visitor->Visit(this);
  }

  BinaryOperatorType operator_type_;
  Expression* first_;
  Expression* second_;
};

}


