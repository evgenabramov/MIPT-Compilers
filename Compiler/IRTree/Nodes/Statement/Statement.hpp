#pragma once

#include "../BaseElement.hpp"

namespace irt {

class Statement : public BaseElement {
 public:
  virtual ~Statement() = default;

  virtual bool IsSeqStatement() const {
    return false;
  }

  virtual bool IsExpStatement() const {
    return false;
  }
  
  virtual bool IsLabelStatement() const {
    return false;
  }
  
  virtual bool IsJumpStatement() const {
    return false;
  }
};

}
