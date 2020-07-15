#pragma once

#include "ConditionalWrapper.hpp"
#include "../../Types/LogicOperatorType.hpp"
#include "../../Nodes/Statement/JumpConditionalStatement.hpp"

namespace irt {

class RelativeConditionalWrapper : public ConditionalWrapper {
 public:
  RelativeConditionalWrapper(LogicOperatorType type, Expression* lhs, Expression* rhs)
      : operator_type_(type), lhs_(lhs), rhs_(rhs) {}

  Statement* ToConditional(Label true_label, Label false_label) override {
    return new JumpConditionalStatement(operator_type_, lhs_, rhs_, true_label, false_label);
  }

  LogicOperatorType operator_type_;
  Expression* lhs_;
  Expression* rhs_;
};

}


