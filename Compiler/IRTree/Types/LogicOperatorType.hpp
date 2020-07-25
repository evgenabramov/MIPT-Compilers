#pragma once

#include <string>

namespace irt {

enum class LogicOperatorType : char {
  LT,
  LE,
  GT,
  GE,
  EQ,
  NE,
};

namespace {

std::string ToString(LogicOperatorType type) {
  switch (type) {
    case LogicOperatorType::EQ:return "EQ";
    case LogicOperatorType::NE:return "NE";
    case LogicOperatorType::LT:return "LT";
    case LogicOperatorType::LE:return "LE";
    case LogicOperatorType::GT:return "GT";
    default:return "GE";
  }
}

}

}
