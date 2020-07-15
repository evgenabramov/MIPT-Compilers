#pragma once

#include "Expression.hpp"
#include "../../Generators/Label.h"

namespace irt {

class NameExpression : public Expression {
 public:
  explicit NameExpression(Label label) : label_(std::move(label)) {}

  void Accept(Visitor* visitor) override {
    visitor->Visit(this);
  }

  Label label_;
};

}


