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

  ~PrimitiveArrayType() final {
    delete array_type_;
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

  size_t GetSize() const override {
    // Array size is a runtime property which can change
    // So we can't store dynamic array on stack anyway
    // TODO: find correct solution for problem
    return 4;
  }

 private:
  ArrayType* array_type_ = nullptr;
};

} // namespace ast