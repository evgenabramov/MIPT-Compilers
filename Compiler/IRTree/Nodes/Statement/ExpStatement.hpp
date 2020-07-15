#pragma once

#include "Statement.hpp"
#include "../Expression/Expression.hpp"

namespace irt {

class ExpStatement : public Statement {
 public:
  explicit ExpStatement(Expression* expression) : expression_(expression) {}

  Expression* GetExpression() {
    return expression_;
  }

  ~ExpStatement() override = default;

  void Accept(Visitor* visitor) override {
    visitor->Visit(this);
  }

 private:
  Expression* expression_;
};

}

