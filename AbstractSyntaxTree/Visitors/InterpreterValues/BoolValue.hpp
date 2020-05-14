#pragma once

namespace ast {

class BoolValue : public Value {
 public:
  explicit BoolValue(bool value = false) : value_(value) {}
  BoolValue(const BoolValue& other) = default;
  BoolValue& operator=(const BoolValue& other) = default;

  int toInt() const override {
      return static_cast<int>(value_);
  }

  bool toBool() const override {
      return value_;
  }

  std::string GetTypeIdentifier() const override {
      return "bool";
  }

 private:
  bool value_ = 0;
};

} // namespace ast