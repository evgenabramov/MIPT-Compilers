#pragma once

#include "ClassStorage.hpp"
#include "Object.hpp"
#include "ArrayObject.h"
#include "PrimitiveSimpleType.hpp"
#include "Symbol.hpp"
#include "PrimitiveArrayType.hpp"

#include <unordered_map>
#include <memory>

namespace ast {

class SimpleObject : public Object {
 public:
  explicit SimpleObject(PrimitiveSimpleType* type);

  SimpleObject(PrimitiveSimpleType* type, int value) : type_(type), value_(value) {}

  void SetValue(Object* object) override;

  int GetValue() override {
    return value_;
  }

  PrimitiveType* GetType() override {
    return type_;
  }

  std::unordered_map<Symbol, Object*> GetFields() override {
    return fields_;
  }

 private:
  PrimitiveSimpleType* type_;

  int value_ = 0;
  std::unordered_map<Symbol, Object*> fields_;
};

} // namespace ast