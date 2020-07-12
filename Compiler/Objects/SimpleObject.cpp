#include "SimpleObject.h"

namespace ast {

SimpleObject::SimpleObject(PrimitiveSimpleType* type) : type_(type) {
  if (type_->IsClass()) {
    // fields as symbols
    auto fields = ClassStorage::GetInstance().GetClassFields(Symbol(type_->GetTypeName()));
    for (auto& [field_name, field_type] : fields) {
      if (field_type->IsArray()) {
        fields_[field_name] = new ArrayObject(dynamic_cast<PrimitiveArrayType*>(field_type.get()));
      } else {
        fields_[field_name] = new SimpleObject(dynamic_cast<PrimitiveSimpleType*>(field_type.get()));
      }
    }
  } else {
    value_ = 0;
  }
}

void SimpleObject::SetValue(Object* object) {
  if (object->GetType()->GetTypeName() != type_->GetTypeName()) {
    throw std::runtime_error("Different types in object assignment");
  }
  value_ = object->GetValue();
  // TODO: Check if it is safe
  fields_ = std::move(object->GetFields());
}

} // namespace ast
