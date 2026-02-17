#pragma once

#include "Operation.hpp"
namespace GiantGraph {

template <typename ExactOp>

class Op {
protected:
  Operation *Oper;

public:
  explicit Op(Operation *Operation) : Oper(Operation) {}

  Operation *getOperation() const { return Oper; }

  const std::string &getName() const { return Oper->getName(); }

  static bool classof(Operation *operation) {
    return operation->getName() == ExactOp::getOperationName();
  }
};
} // namespace GiantGraph
