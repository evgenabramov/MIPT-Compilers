#pragma once

#include "Statement.hpp"
#include "Expression.hpp"

namespace ast {

class IfElseStatement : public Statement {
 public:
  IfElseStatement(Expression* expression, Statement* first, Statement* second) :
      expression_(expression), first_(first), second_(second) {}

  void Accept(Visitor* visitor) override {
      visitor->Visit(this);
  }

  Expression* GetExpression() const {
      return expression_;
  }
  void SetExpression(Expression* expression) {
      expression_ = expression;
  }

  Statement* GetFirstStatement() const {
      return first_;
  }

  void SetFirstStatement(Statement* first) {
      first_ = first;
  }

  Statement* GetSecondStatement() const {
      return second_;
  }

  void SetSecondStatement(Statement* second) {
      second_ = second;
  }

 private:
  Expression* expression_;
  Statement* first_;
  Statement* second_;
};

} // namespace ast