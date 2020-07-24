#pragma once

#include "Expression.hpp"
#include "Statement.hpp"

namespace irt {

struct IrtStorage {
  Expression* expression_ = nullptr;
  Statement* statement_ = nullptr;
  ExpressionList* expression_list_ = nullptr;
};

}