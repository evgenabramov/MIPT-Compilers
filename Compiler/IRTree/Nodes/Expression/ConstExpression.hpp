#pragma once

#include "Expression.hpp"

namespace irt {

class ConstExpression : public Expression {
 public:
  explicit ConstExpression(int value) : value_(value) {}

  ~ConstExpression() final = default;

  int Value() const {
    return value_;
  }

  void Accept(Visitor* visitor) override {
    visitor->Visit(this);
  }

 private:
  int value_;
};

}

