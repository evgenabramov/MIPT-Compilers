#pragma once

#include <memory>

namespace ast {

class IntValue : public Value {
 public:
  explicit IntValue(int value = 0) : value_(value) {}
  IntValue(const IntValue& other) = default;
  IntValue& operator=(const IntValue& other) = default;

  int toInt() const override {
    return value_;
  }

  bool toBool() const override {
      return static_cast<bool>(value_);
  }

  std::string GetTypeIdentifier() const override {
      return "int";
  }

 private:
  int value_ = 0;
};

} // namespace ast