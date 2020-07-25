#pragma once

#include <string>

class MemberType {
 public:
  virtual ~MemberType() = default;
  virtual std::string GetTypeName() const = 0;
};