#pragma once

#include "PrimitiveArrayType.hpp"
#include "SimpleType.hpp"
#include "SimpleObject.h"

#include <vector>
#include <exception>

namespace ast {

class ArrayObject : public Object {
 public:
  explicit ArrayObject(PrimitiveArrayType* array_type)
      : array_type_(array_type), element_type_(new PrimitiveSimpleType(array_type->GetTypeName())) {}

  PrimitiveType* GetType() override {
    return element_type_;
  }

  void SetValue(Object* object) override;

  int GetValue() override {
    throw std::runtime_error("Try to get int value from array");
  }

  std::unordered_map<Symbol, Object*> GetFields() override {
    throw std::runtime_error("Try to get fields from Array object");
  }

  void SetAtIndex(size_t index, Object* value);

  Object* GetAtIndex(size_t index);

  size_t GetSize() const {
    return buffer_.size();
  }

  void Resize(size_t size);

 private:
  PrimitiveArrayType* array_type_;
  PrimitiveSimpleType* element_type_;

  std::vector<Object*> buffer_;
};

} // namespace ast