#pragma once

#include "Address.hpp"

#include <IRTree/Nodes/Expression/BinopExpression.hpp>
#include <IRTree/Nodes/Expression/ConstExpression.hpp>
#include <IRTree/Nodes/Expression/MemExpression.hpp>
#include <IRTree/Types/BinaryOperatorType.hpp>

namespace irt {

class AddressInFrame : public Address {
 public:
  AddressInFrame(Address* frame_address, int offset) : frame_address_(frame_address), offset_(offset) {}

  ~AddressInFrame() = default;

  Expression* ToExpression() override {
    Expression* offset_expression;
    if (offset_ != 0) {
      offset_expression = new BinopExpression(
          BinaryOperatorType::PLUS,
          frame_address_->ToExpression(),
          new ConstExpression(offset_)
      );
    } else {
      offset_expression = frame_address_->ToExpression();
    }
    return new MemExpression(offset_expression);
  }

 private:
  Address* frame_address_;
  int offset_;
};

}
