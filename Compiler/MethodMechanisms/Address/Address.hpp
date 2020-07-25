#pragma once

#include <IRTree/Nodes/Expression/Expression.hpp>

namespace irt {

class Address {
 public:
  virtual ~Address() = default;
  virtual Expression* ToExpression() = 0;
};

}
