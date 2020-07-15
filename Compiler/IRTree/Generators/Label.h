#pragma once

#include <string>

namespace irt {

class Label {
 public:
  Label();

  explicit Label(std::string label);

  std::string ToString() const;

 private:
  std::string label_;
  static int counter_;
};

}