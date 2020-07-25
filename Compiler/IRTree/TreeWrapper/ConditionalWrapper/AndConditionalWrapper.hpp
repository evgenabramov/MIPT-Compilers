#pragma once

#include "ConditionalWrapper.hpp"

namespace irt {

class AndConditionalWrapper : public ConditionalWrapper {
 public:
  AndConditionalWrapper(SubtreeWrapper* first, SubtreeWrapper* second) : first_(first), second_(second) {}

  Statement* ToConditional(Label true_label, Label false_label) override {
    Label middle_label;

    return new SeqStatement(
        first_->ToConditional(middle_label, false_label),
        new SeqStatement(
            new LabelStatement(middle_label),
            second_->ToConditional(true_label, false_label)
        )
    );
  }

 private:
  SubtreeWrapper* first_;
  SubtreeWrapper* second_;
};

}

