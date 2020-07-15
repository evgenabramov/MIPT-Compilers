#pragma once

#include "ConditionalWrapper.hpp"

namespace irt {

class OrConditionalWrapper : public ConditionalWrapper {
 public:
  OrConditionalWrapper(SubtreeWrapper* first, SubtreeWrapper* second) : first_(first), second_(second) {}

  Statement* ToConditional(Label true_label, Label false_label) override {
    Label middle_label;
    return new SeqStatement(
        first_->ToConditional(true_label, middle_label),
        new SeqStatement(
            new LabelStatement(middle_label),
            second_->ToConditional(true_label, false_label)
        )
    );
  }

  SubtreeWrapper* first_;
  SubtreeWrapper* second_;
};

}


