#pragma once

#include "Statement.hpp"
#include "Expression.hpp"

namespace ast {

class WhileStatement : public Statement {
 public:
  WhileStatement(Expression* expression, Statement* statement) : expression_(expression), statement_(statement) {}

  void Accept(Visitor* visitor) override {
    visitor->Visit(this);
  }

  Expression* GetExpression() const {
    return expression_;
  }

  void SetExpression(Expression* expression) {
    expression_ = expression;
  }

  Statement* GetStatement() const {
    return statement_;
  }

  void SetStatement(Statement* statement) {
    statement_ = statement;
  }

 private:
  Expression* expression_;
  Statement* statement_;
};

} // namespace ast