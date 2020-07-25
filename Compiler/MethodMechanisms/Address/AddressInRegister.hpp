#pragma once

#include "Address.hpp"

#include <IRTree/Generators/Temporary.h>
#include <IRTree/Nodes/Expression/TempExpression.hpp>

namespace irt {

class AddressInRegister : public Address {
 public:
  explicit AddressInRegister(const Temporary& temp) : temp_(temp) {}

  Expression* ToExpression() override {
    return new TempExpression(temp_);
  }

 private:
  Temporary temp_;
};

}

