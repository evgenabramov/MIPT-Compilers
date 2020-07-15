#pragma once

#include "Statement.hpp"
#include "../../Generators/Label.h"

namespace irt {

class LabelStatement : public Statement {
 public:
  explicit LabelStatement(Label label) : label_(std::move(label)) {}

  void Accept(Visitor* visitor) override {
    visitor->Visit(this);
  }

  Label label_;
};

}

