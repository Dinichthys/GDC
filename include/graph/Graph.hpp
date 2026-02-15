#pragma once

#include <memory>

#include "Operation.hpp"

// HEALTH
// STAMINA
// ENDURANCE

namespace GiantGraph {
class GiantGraph {
public:
  GiantGraph(std::vector<std::unique_ptr<Operation>> Nodes_)
      : Nodes{std::move(Nodes_)} {};
  ~GiantGraph() = default;
  std::vector<std::unique_ptr<Operation>> &nodes() {
    return Nodes;
  };
  const std::vector<std::unique_ptr<Operation>> &nodes() const {
    return Nodes;
  };

private:
  std::vector<std::unique_ptr<Operation>> Nodes;
};
} // namespace GiantGraph
