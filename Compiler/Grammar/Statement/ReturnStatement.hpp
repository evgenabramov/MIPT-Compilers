#pragma once

#include "Statement.hpp"
#include "Expression.hpp"

namespace ast {

class ReturnStatement : public Statement {
 public:
  explicit ReturnStatement(Expression* expression) : expression_(expression ) {}

  void Accept(Visitor* visitor) override {
      visitor->Visit(this);
  }

  Expression* GetExpression() const {
      return expression_;
  }

  void SetExpression(Expression* expression) {
      expression_ = expression;
  }

 private:
  Expression* expression_;
};

} // namespace ast