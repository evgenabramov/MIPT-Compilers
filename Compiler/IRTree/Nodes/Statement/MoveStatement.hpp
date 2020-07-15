#pragma once

#include "Statement.hpp"
#include "../Expression/Expression.hpp"

namespace irt {

class MoveStatement : public Statement {
 public:
  MoveStatement(Expression* source, Expression* target) : source_(source), target_(target) {}

  void Accept(Visitor* visitor) override {
    visitor->Visit(this);
  }

  Expression* source_;
  Expression* target_;
};

};


