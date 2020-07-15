#pragma once

#include "Statement.hpp"

namespace irt {

class SeqStatement : public Statement {
 public:
  SeqStatement(Statement* first, Statement* second)
      : first_statement_(first), second_statement_(second) {}

  void Accept(Visitor* visitor) override {
    visitor->Visit(this);
  }

  Statement* first_statement_;
  Statement* second_statement_;
};

}


