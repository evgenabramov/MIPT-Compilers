#pragma once

#include "BaseElement.hpp"

#include <vector>

namespace irt {

class Expression;

class ExpressionList : public BaseElement {
 public:
  ExpressionList() = default;

  void Add(Expression* expression) {
    expressions_.push_back(expression);
  }

  void Accept(Visitor* visitor) override {
    visitor->Visit(this);
  }

  std::vector<Expression*> expressions_;
};

}

