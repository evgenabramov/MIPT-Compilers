#pragma once

#include <Visitors/Visitor.hpp>
#include "Statement.hpp"
#include "Expression.hpp"

namespace ast {

class AssertStatement : public Statement {
 public:
  explicit AssertStatement(Expression* expression) : expression_(expression) {}

  void Accept(Visitor* visitor) override {
    visitor->Visit(this);
  }

  void SetExpression(Expression* expression) {
    expression_ = expression;
  }

  Expression* GetExpression() const {
    return expression_;
  }

 private:
  Expression* expression_;
};

} // namespace ast;