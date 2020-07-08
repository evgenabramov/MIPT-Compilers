#pragma once

#include "Object.hpp"
#include "PrimitiveSimpleType.hpp"
#include "ClassStorage.hpp"
#include "Symbol.hpp"
#include "PrimitiveArrayType.hpp"
#include "ArrayObject.hpp"

#include <unordered_map>
#include <memory>

namespace ast {

class SimpleObject : public Object {
 public:
  explicit SimpleObject(PrimitiveSimpleType* type) : type_(type) {
    if (type_->IsClass()) {
      // fields as symbols
      auto fields = ClassStorage::GetInstance().GetField(Symbol(type_->GetTypeName()));
      for (auto& field : fields) {
        if (field.second->IsArray()) {
          fields_[field.first] =
              std::make_shared<ArrayObject>(dynamic_cast<PrimitiveArrayType*>(field.second.get()));
        } else {
          fields_[field.first] =
              std::make_shared<SimpleObject>(dynamic_cast<PrimitiveSimpleType*>(field.second.get()));
        }
      }
    } else {
      value_ = 0;
    }
  }

  SimpleObject(PrimitiveSimpleType* type, int value) : type_(type), value_(value) {}

  void SetValue(std::shared_ptr<Object> object) override {
    if (object->GetType()->GetTypeName() != type_->GetTypeName()) {
      throw std::runtime_error("Different types in object assignment");
    }
    value_ = object->GetValue();
    fields_ = std::move(object->GetFields());
  }

  int GetValue() override {
    return value_;
  }

  PrimitiveType* GetType() override {
    return type_;
  }

  std::unordered_map<Symbol, std::shared_ptr<Object>> GetFields() override {
    return fields_;
  }

 private:
  PrimitiveSimpleType* type_;
  int value_ = 0;
  std::unordered_map<Symbol, std::shared_ptr<Object>> fields_;
};

} // namespace ast