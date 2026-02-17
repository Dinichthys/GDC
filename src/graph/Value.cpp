#include "graph/Value.hpp"

namespace GiantGraph {
void OpResult::addUse(OpOperand *NewUse) { Uses.push_back(NewUse); }

void OpResult::removeUse(OpOperand *NewUse) {
  // FIXME that's abosulte shit and we need to come up with
  // better removal function.
  // Also since Uses change will probably be extremely common in optimizations
  // we should use double-linked list for uses, but for now we'll stick with
  // default vector and nullptr checks.
  for (size_t I = 0; I < Uses.size(); ++I) {
    // Should use std::find_if here, but it's just draft version, so
    // it's not important yet.
    if (Uses[I] == NewUse) {
      Uses[I] = nullptr;
    }
  }
}
// And yes, in current state it will just infinitely grow each time you
// need to change a Use, but again, it doesn't really matter right now.

} // namespace GiantGraph
