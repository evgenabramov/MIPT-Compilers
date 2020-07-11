#pragma once

#include "MethodType.hpp"

#include <stack>
#include <vector>

namespace ast {

// Stack is common for all scopes
// But it's size changes for different scopes
class Frame {
 public:
  explicit Frame(size_t num_params) {
    params_.resize(num_params);
    AllocScope();
  }

  // Set params values
  void SetParams(const std::vector<Object*>& values) {
    if (params_.size() != values.size()) {
      throw std::runtime_error("Mismatched number of arguments");
    }
    params_ = values;
  }

  // Returns index at which variable can be stored on stack
  // for current function
  size_t AllocVariable(PrimitiveType* primitive_type) {
    size_t index = variables_.size();
    if (primitive_type->IsSimpleType()) {
      variables_.push_back(new SimpleObject(dynamic_cast<PrimitiveSimpleType*>(primitive_type)));
    } else {
      variables_.push_back(new ArrayObject(dynamic_cast<PrimitiveArrayType*>(primitive_type)));
    }

    return index;
  }

  void AllocScope() {
    offsets_.push(variables_.size());
  }

  void DeallocScope() {
    size_t new_size = offsets_.top();
    offsets_.pop();

    variables_.resize(new_size);
  }

  Object* Get(int index) {
    if (index >= 0) {
      return variables_.at(index);
    }
    return params_.at(-index - 1);
  }

  void Set(int index, Object* value) {
    if (index >= 0) {
      variables_.at(index) = value;
    } else {
      params_.at(-index - 1) = value;
    }
  }

  bool HasParent() const {
    return parent_frame_ != nullptr;
  }

  void SetParentFrame(Frame* parent_frame) {
    parent_frame_ = parent_frame;
  }

  void SetReturnValue(Object* return_value) {
    return_value_ = return_value;
  }

  Object* GetReturnValue() const {
    return return_value_;
  }

 private:
  std::stack<int> offsets_;        // required stack size for each scope
  std::vector<Object*> params_;    // passed arguments values
  std::vector<Object*> variables_; // local variables values

  Object* return_value_ = nullptr;
  Frame* parent_frame_ = nullptr;  // access link for return
};

} // namespace ast