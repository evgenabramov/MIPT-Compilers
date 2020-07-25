#pragma once

#include "Statement.hpp"
#include "../../Generators/Label.h"

namespace irt {

class JumpStatement : public Statement {
 public:
  explicit JumpStatement(Label label) : label_(std::move(label)) {}

  void Accept(Visitor* visitor) override {
    visitor->Visit(this);
  }
  
  bool IsJumpStatement() const override {
    return true;
  }

  Label label_;
};

}

