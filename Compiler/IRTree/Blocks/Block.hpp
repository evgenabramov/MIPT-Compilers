#pragma once

#include "../Statement/Statement.hpp"
#include "../Generators/Label.h"

namespace irt {

// Base class for IRTree blocks
class Block {
 public:
  Block(Label label, Statement* statement) : label_(std::move(label)), statement_(statement) {}
  
  virtual bool IsSimple() const = 0;
  
  bool CheckHasTrace() const {
    return has_trace_;
  }
  
  void SetHasTrace() {
    has_trace_ = true;
  }
  
  Label GetLabel() const {
    return label_;
  }
  
 protected:
  Label label_;
  Statement* statement_;
  
  bool has_trace_ = false;
};

}