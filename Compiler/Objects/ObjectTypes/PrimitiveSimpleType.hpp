#pragma once

#include "PrimitiveType.hpp"
#include "SimpleType.hpp"

#include <string>

namespace ast {

class PrimitiveSimpleType : public PrimitiveType {
 public:
  explicit PrimitiveSimpleType(SimpleType* simple_type)
    : simple_type_(simple_type) {
      CheckClass();
  }

  explicit PrimitiveSimpleType(const std::string& identifier) {
      simple_type_ = new SimpleType(identifier);
      CheckClass();
  }

  bool IsArray() const override {
      return false;
  }

  bool IsClass() const {
    return is_class_;
  }

  std::string GetTypeName() const override {
      return simple_type_->GetIdentifier();
  }

  bool IsSimpleType() const override {
    return true;
  }

 private:
  void CheckClass() {
      const std::string& identifier = simple_type_->GetIdentifier();
      is_class_ = ((identifier != "int") && (identifier != "boolean"));
  }

  SimpleType* simple_type_ = nullptr;
  bool is_class_ = false;
};

} // namespace ast