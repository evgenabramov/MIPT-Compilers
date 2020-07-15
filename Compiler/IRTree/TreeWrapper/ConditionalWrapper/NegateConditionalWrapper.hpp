#pragma once

#include "ConditionalWrapper.hpp"

namespace irt {

class NegateConditionalWrapper : public ConditionalWrapper {
 public:
  explicit NegateConditionalWrapper(SubtreeWrapper* wrapper) : wrapper_(wrapper) {}

  Statement* ToConditional(Label true_label, Label false_label) override {
    return wrapper_->ToConditional(false_label, true_label);
  }

 private:
  SubtreeWrapper* wrapper_;
};

}


