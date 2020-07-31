#pragma once

#include "Expression.hpp"
#include "../../Generators/Temporary.h"

namespace irt {

class TempExpression : public Expression {
 public:
  explicit TempExpression(const irt::Temporary& temporary) : temporary_(temporary) {}

  void Accept(Visitor* visitor) override {
    visitor->Visit(this);
  }
  
  bool IsTempExpression() const override {
    return true;
  }

  Temporary temporary_;
};

}


