#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>

#include "graph/GraphVizard.hpp"

namespace GraphVizardry {

void dumpGraphViz(const GiantGraph::GiantGraph &GiantGraph) {
  std::string GraphVizString = "digraph g {\n";

  for (auto &Node : GiantGraph.nodes()) {
    auto RawAddr = reinterpret_cast<uintptr_t>(Node.get());
    GraphVizString += "node" + std::to_string(RawAddr) +
                      " [\"label\"=\"{<optype> " + Node->getName();
    auto NumOperands = Node->getNumOperands();
    for (unsigned I = 0; I < NumOperands; ++I) {
      GraphVizString +=
          " | <op" + std::to_string(I) + "> Operand" + std::to_string(I);
    }
    auto NumResults = Node->getNumResults();
    for (unsigned I = 0; I < NumResults; ++I) {
      GraphVizString +=
          " | <res" + std::to_string(I) + "> Result" + std::to_string(I);
    }
    GraphVizString += "}\"\nshape = \"record\"];\n";

    for (unsigned I = 0; I < NumResults; ++I) {
      auto &Res = Node->getResultRef(I);
      for (auto &Use : Res.getUses()) {
        GraphVizString +=
            "node" + std::to_string(RawAddr) + ": <res" + std::to_string(I) +
            "> -> node" +
            std::to_string(reinterpret_cast<uintptr_t>(Use->getOwner())) + '\n';
      }
    }
  }
  GraphVizString += "\n}\n";
  std::fstream GraphVizFile;
  GraphVizFile.open(DOT_LOG_FILE_NAME, std::ios::out);
  if (!GraphVizFile.is_open()) {
    throw(std::runtime_error("Cannot open dot file, casul"));
  }
  GraphVizFile << GraphVizString;
  GraphVizFile.close();
}

} // namespace GraphVizardry
