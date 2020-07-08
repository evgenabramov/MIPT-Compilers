#pragma once

#include "Object.hpp"
#include "PrimitiveArrayType.hpp"
#include "SimpleType.hpp"
#include "SimpleObject.hpp"

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

  void SetValue(std::shared_ptr<Object> object) override {
    auto array_object = dynamic_cast<ArrayObject*>(object.get());
    this->Resize(array_object->GetSize());
    for (size_t index = 0; index < buffer_.size(); ++index) {
      buffer_[index]->SetValue(array_object->buffer_[index]);
    }
  }

  int GetValue() override {
    throw std::runtime_error("Try to get int value from array");
  }

  std::unordered_map<Symbol, std::shared_ptr<Object>> GetFields() override {
    throw std::runtime_error("Try to get fields from Array object");
  }

  void SetAtIndex(size_t index, std::shared_ptr<Object> value) {
    if (value->GetType()->GetTypeName() != element_type_->GetTypeName()) {
      throw std::runtime_error("Bad array element type");
    } else if (index >= buffer_.size()) {
      throw std::runtime_error("Bad array index");
    }
    buffer_[index] = value;
  }

  std::shared_ptr<Object> GetAtIndex(size_t index) {
    if (index >= buffer_.size()) {
      throw std::runtime_error("Bad array index");
    }
    return buffer_[index];
  }

  size_t GetSize() const {
    return buffer_.size();
  }

  void Resize(size_t size) {
    buffer_.clear();
    buffer_ = std::vector<std::shared_ptr<Object>>(size, std::make_shared<SimpleObject>(element_type_));
  }

 private:
  PrimitiveArrayType* array_type_;
  PrimitiveSimpleType* element_type_;
  std::vector<std::shared_ptr<Object>> buffer_;
};

} // namespace ast