#include "Temporary.h"

namespace irt {

Temporary::Temporary() : name_("%" + std::to_string(counter_++)) {}

Temporary::Temporary(const std::string& name) : name_(name) {}

std::string Temporary::ToString() const {
  return name_;
}

int Temporary::counter_ = 0;

}