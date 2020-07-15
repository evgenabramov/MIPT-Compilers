#pragma once

#include "PrimitiveType.hpp"
#include "SimpleType.hpp"
#include "ClassStorage.hpp"

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

  ~PrimitiveSimpleType() final {
    delete simple_type_;
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

  size_t GetSize() const override {
    if (!is_class_) {
      return 4;
    }
    std::string class_name = simple_type_->GetIdentifier();
    auto fields = ClassStorage::GetInstance().GetClassFields(class_name);
    size_t size = 4; // Class without fields still require space
    for (const auto& [field_name, field_type]: fields) {
      size += field_type->GetSize();
    }
    return size;
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