#include "Label.h"

namespace irt {

Label::Label() : label_("L" + std::to_string(counter_++)) {}

Label::Label(std::string label) : label_(std::move(label)) {}

std::string Label::ToString() const {
  return label_;
}

int Label::counter_ = 0;

}