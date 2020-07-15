#pragma once

#include "PrimitiveType.hpp"
#include "Symbol.hpp"

#include <unordered_map>
#include <memory>

namespace ast {

class Object {
 public:
  virtual void SetValue(Object* object) = 0;
  virtual int GetValue() = 0;
  virtual PrimitiveType* GetType() = 0;
  virtual std::unordered_map<Symbol, Object*> GetFields() = 0;
};

} // namespace ast