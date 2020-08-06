#pragma once

#include <string>

namespace irt {

class Label {
 public:
  Label();
  Label(const Label& other);
  explicit Label(std::string label);

  std::string ToString() const;
  
  bool operator==(const Label& other) const;
  
 private:
  std::string label_;
  static int counter_;
};

}

template<> struct std::hash<irt::Label> {
  std::size_t operator()(irt::Label const& label) const noexcept {
    return std::hash<std::string>{}(label.ToString());
  }
};