#pragma once

#include "IRTree/Generators/Label.h"
#include "../Nodes/Expression/Expression.hpp"
#include "../Nodes/Statement/Statement.hpp"

namespace irt {

// Base class
class SubtreeWrapper {
 public:
  virtual ~SubtreeWrapper() = default;

  virtual Expression* ToExpression() = 0;

  virtual Statement* ToStatement() = 0;

  virtual Statement* ToConditional(Label true_label, Label false_label) = 0;
};

}