#include "ArrayObject.h"

namespace ast {

void ArrayObject::SetValue(Object* object) {
  auto array_object = dynamic_cast<ArrayObject*>(object);
  this->Resize(array_object->GetSize());
  for (size_t index = 0; index < buffer_.size(); ++index) {
    buffer_[index]->SetValue(array_object->buffer_[index]);
  }
}

void ArrayObject::SetAtIndex(size_t index, Object* value) {
  if (value->GetType()->GetTypeName() != element_type_->GetTypeName()) {
    throw std::runtime_error("Bad array element type");
  } else if (index >= buffer_.size()) {
    throw std::runtime_error("Bad array index");
  }
  buffer_[index] = value;
}

Object* ArrayObject::GetAtIndex(size_t index) {
  if (index >= buffer_.size()) {
    throw std::runtime_error("Bad array index");
  }
  return buffer_[index];
}

void ArrayObject::Resize(size_t size) {
  buffer_.clear();
  for (size_t i = 0; i < size; ++i) {
    buffer_.emplace_back(new SimpleObject(element_type_));
  }
}

} // namespace ast
