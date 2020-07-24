#pragma once

#include "Visitor.h"
#include "../Nodes/BaseElement.hpp"
#include "VisitorStruct.h"

namespace irt {

template<typename T>
class TemplateVisitor : public Visitor {
 public:
  T Accept(BaseElement* element) {
    element->Accept(this);
    return tos_value_;
  }

 protected:
  T tos_value_;
};

template int TemplateVisitor<int>::Accept(BaseElement* element);

template IrtStorage TemplateVisitor<IrtStorage>::Accept(irt::BaseElement* element);

}

