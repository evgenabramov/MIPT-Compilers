#pragma once

#include "../BaseElement.hpp"

namespace irt {

class Expression : public BaseElement {
 public:
  virtual ~Expression() = default;

  virtual bool IsEseqExpression() const {
    return false;
  }

  virtual bool IsConstExpression() const {
    return false;
  }

  virtual bool IsNameExpression() const {
    return false;
  }
  
  virtual bool IsBinopExpression() const {
    return false;
  }
  
  virtual bool IsMemExpression() const {
    return false;
  }
  
  virtual bool IsTempExpression() const {
    return false;
  }
  
  virtual bool IsCallExpression() const {
    return false;
  }
};

}
