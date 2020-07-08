#pragma once

#include "PrimitiveType.hpp"
#include "ArrayType.hpp"
#include "PrimitiveSimpleType.hpp"

namespace ast {

class PrimitiveArrayType : public PrimitiveType {
 public:
  explicit PrimitiveArrayType(ArrayType* array_type) : array_type_(array_type) {}

  explicit PrimitiveArrayType(PrimitiveSimpleType* primitive_simple_type)
    : array_type_(new ArrayType(primitive_simple_type->GetTypeName())) {}

  PrimitiveSimpleType* GetPrimitiveSimpleType() {
    return new PrimitiveSimpleType(array_type_->GetIdentifier());
  }

  bool IsArray() const override {
    return true;
  }

  std::string GetTypeName() const override {
    return array_type_->GetIdentifier();
  }

  bool IsSimpleType() const override {
    return false;
  }

 private:
  ArrayType* array_type_ = nullptr;
};

} // namespace ast