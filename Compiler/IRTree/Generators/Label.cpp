#include "Label.h"

#include <string>

namespace irt {

Label::Label() : label_("L" + std::to_string(counter_++)) {}

Label::Label(const Label& other) : label_(other.label_) {}

Label::Label(std::string label) : label_(std::move(label)) {}

std::string Label::ToString() const {
  return label_;
}

bool Label::operator==(const Label& other) const {
  return (label_ == other.label_);
}

int Label::counter_ = 0;

}