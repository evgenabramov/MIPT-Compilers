#pragma once

#include "Statement.hpp"
#include "../Expression/Expression.hpp"

namespace irt {

class MoveStatement : public Statement {
 public:
  MoveStatement(Expression* target, Expression* source) : target_(target), source_(source) {}

  void Accept(Visitor* visitor) override {
    visitor->Visit(this);
  }

  Expression* source_;
  Expression* target_;
};

};


