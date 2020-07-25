#pragma once

#include "SubtreeWrapper.hpp"
#include "../Nodes/Statement/ExpStatement.hpp"
#include "../Nodes/Statement/JumpConditionalStatement.hpp"
#include "../Nodes/Expression/ConstExpression.hpp"

namespace irt {

class ExpressionWrapper : public SubtreeWrapper {
 public:
  explicit ExpressionWrapper(Expression* expression) : expression_(expression) {}

  virtual ~ExpressionWrapper() = default;

  Expression* ToExpression() override {
    return expression_;
  }

  Statement* ToStatement() override {
    return new ExpStatement(expression_);
  }

  Statement* ToConditional(Label true_label, Label false_label) override {
    return new JumpConditionalStatement(
        LogicOperatorType::NE,
        expression_,
        new ConstExpression(0),
        true_label,
        false_label
    );
  }

 private:
  Expression* expression_;
};

}

