#pragma once

#include "Expression.hpp"
#include "Statement.hpp"
#include "NamedVariable.hpp"

namespace ast {

class AssignmentStatement : public Statement {
 public:
  AssignmentStatement(NamedVariable* named_variable, Expression* expression)
      : named_variable_(named_variable), expression_(expression) {}

  void Accept(Visitor* visitor) override {
      visitor->Visit(this);
  }

  NamedVariable* GetNamedVariable() const {
      return named_variable_;
  }

  void SetNamedVariable(NamedVariable* named_variable) {
      named_variable_ = named_variable;
  }

  Expression* GetExpression() const {
      return expression_;
  }

  void SetExpression(Expression* expression) {
      expression_ = expression;
  }

 private:
    NamedVariable* named_variable_;
    Expression* expression_;
};

} // namespace ast