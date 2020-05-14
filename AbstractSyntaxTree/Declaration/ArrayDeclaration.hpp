#pragma once

#include "Declaration.hpp"
#include "ArrayType.hpp"

#include <string>

namespace ast {

class ArrayDeclaration : public Declaration {
 public:
  ArrayDeclaration(ArrayType* array_type, std::string array_name)
      : array_type_(array_type), array_name_(std::move(array_name)) {}

  void Accept(Visitor* visitor) override {
      visitor->Visit(this);
  }

  ArrayType* GetArrayType() const {
      return array_type_;
  }

  const std::string& GetArrayName() const {
      return array_name_;
  }

 private:
  ArrayType* array_type_;
  std::string array_name_;
};

} // namespace ast