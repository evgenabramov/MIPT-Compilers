#pragma once

#include "SubtreeWrapper.hpp"

#include <cassert>

namespace irt {

class StatementWrapper : public SubtreeWrapper {
 public:
  explicit StatementWrapper(Statement* statement) : statement_(statement) {}

  Expression* ToExpression() override {
    assert(false);
  }

  Statement* ToStatement() override {
    return statement_;
  }

  Statement* ToConditional(Label true_label, Label false_label) override {
    assert(false);
  }

 private:
  Statement* statement_;
};

}

