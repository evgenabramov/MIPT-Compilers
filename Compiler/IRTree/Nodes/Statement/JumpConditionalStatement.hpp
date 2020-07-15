#pragma once

#include "Statement.hpp"
#include "../../Types/LogicOperatorType.hpp"
#include "../Expression/Expression.hpp"
#include "../../Generators/Label.h"

namespace irt {

class JumpConditionalStatement : public Statement {
 public:
  JumpConditionalStatement(
      LogicOperatorType type,
      Expression* left,
      Expression* right,
      Label label_true,
      Label label_false
  ) : operator_type_(type),
      left_operand_(left),
      right_operand_(right),
      label_true_(label_true),
      label_false_(label_false) {}

  void Accept(Visitor* visitor) override {
    visitor->Visit(this);
  }

  LogicOperatorType operator_type_;
  Expression* left_operand_;
  Expression* right_operand_;
  Label label_true_;
  Label label_false_;
};

}


