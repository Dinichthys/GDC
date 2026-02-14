#include <onnx.pb.h>

#include "graph/GraphBuilder.hpp"

namespace GiantGraph {

void GraphBuilder::buildGraph() {
  const auto &Graph = Model.graph();

  for (const auto &Node : Graph.node()) {
    std::cout << "Op: " << Node.op_type() << "\n";
    // TODO add node on op
    for (const auto &Input : Node.input()) {
      std::cout << "  Input: " << Input << "\n";
    }
    // TODO fill info about node on those ones
    for (const auto &Output : Node.output()) {
      std::cout << "  Output: " << Output << "\n";
    }
  }
}
} // namespace GiantGraph
