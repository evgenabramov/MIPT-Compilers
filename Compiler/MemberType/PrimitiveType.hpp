#pragma once

#include "MemberType.hpp"

#include <string>

namespace ast {

class PrimitiveType : public MemberType {
 public:
  virtual bool IsArray() const = 0;
  virtual bool IsSimpleType() const = 0;
  virtual std::string GetTypeName() const = 0;
};

} // namespace ast