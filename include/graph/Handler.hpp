#pragma once
#include "Operation.hpp"
#include <functional>
namespace GiantGraph {
// NOTE we use strings as node identifiers, which is slow.
// We probably need to generate a enum or something like that.
// (It's not of high importance so I'll postpone it for now)

// The other question are generated handlers, because in some situations (like)
// debug dump or even code emition, it seems more efficient to generate handlers
class PassHandler {
private:
  using HandlerFn = std::function<void(Operation *)>;
  std::unordered_map<std::string, HandlerFn> HandleTable;
public:
  void registerHandler(const std::string &Name, HandlerFn Fn) {
    HandleTable[Name] = Fn;
  }

  void handleBlock(std::vector<Operation *> &Block) {
    for (auto *Op : Block) {
      auto It = HandleTable.find(Op->getName());
      if (It != HandleTable.end()) {
        It->second(Op);
      } else {
        std::cout << "No handler for op: " << Op->getName() << ", skipping\n";
      }
    }
  }
};

} // namespace GiantGraph
