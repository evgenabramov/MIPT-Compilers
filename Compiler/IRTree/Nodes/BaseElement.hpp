#pragma once

#include "../Visitors/Visitor.h"

namespace irt {

class BaseElement {
 public:
  virtual ~BaseElement() = default;
  virtual void Accept(Visitor* visitor) = 0;
};

}
