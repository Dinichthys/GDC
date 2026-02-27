#pragma once
#include "graph/Operation.hpp"

namespace GDC {

template <typename T> static bool is(GiantGraph::Operation *Op) {
  return T::classof(Op);
}

template <typename T> static T dynCast(GiantGraph::Operation *Op) {
  if (is<T>(Op)) {
    return T(Op);
  }
  return T(nullptr);
}
} // namespace GDC
