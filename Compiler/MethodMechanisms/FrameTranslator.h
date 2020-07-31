#pragma once

#include "Address.hpp"

#include <string>
#include <unordered_map>
#include <stack>
#include <vector>

namespace irt {

class FrameTranslator {
 public:
  explicit FrameTranslator(const std::string& function_name);
  void SetupScope();
  void TearDownScope();

  void AddVariable(const std::string& name);
  void AddLocalVariable(const std::string& name);
  void AddReturnAddress();

  void AddArgumentAddress(const std::string& name);

  Address* FramePointer();
  Address* GetAddress(const std::string& name);
  Address* GetReturnValueAddress();

 private:
  // Get frame size on stack and increase it
  int GetOffset();

  std::string function_name_;

  // reserved variable names
  const std::string return_address_ = "::return";
  const std::string frame_pointer_address_ = "::fp";
  const std::string return_value_address_ = "::return_value";

  // variable -> stack of Addresses (register/stack) in scopes
  std::unordered_map<std::string, std::stack<Address*>> addresses_;
  // stack of variables in order they are declared
  std::stack<std::string> symbols_;
  int word_size_ = 4;

  int max_offset_ = 0;
};

}