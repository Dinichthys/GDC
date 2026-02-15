#include <onnx.pb.h>
#include <stdexcept>

#include "graph/Graph.hpp"
#include "graph/GraphBuilder.hpp"

namespace GiantGraph {

GiantGraph GraphBuilder::buildGraph() {
  const auto &Graph = Model.graph();

  std::unordered_map<std::string, OpResult *> DefUseMap;
  unsigned ResCounter{0};
  std::vector<std::unique_ptr<Operation>> Nodes;

  for (const auto &Node : Graph.node()) {
    std::cout << Node.op_type() << '\n';
    Operation::OperationName Name{Node.op_type()};

    std::vector<OpOperand> Operands;
    Operands.reserve(Node.input_size());
    for (auto I = 0; I < Node.input_size(); ++I) {
      Operands.emplace_back(); // might have use in future
    }

    std::vector<OpResult> Results;
    Results.reserve(Node.output_size());
    for (auto I = 0; I < Node.output_size(); ++I) {
      Results.emplace_back(ResCounter);
      ++ResCounter;
    }

    Nodes.push_back(std::make_unique<Operation>(Name, std::move(Operands),
                                                std::move(Results)));
    auto &AddedNode = Nodes.back();

    std::cout << "  Operands:\n";
    for (auto I = 0; I < Node.input_size(); ++I) {
      auto &Input = Node.input().at(I);
      std::cout << "    " << Input << '\n';
      auto &Operand = AddedNode->getOperandRef(I);
      if (DefUseMap.find(Input) != DefUseMap.end()) {
        Operand.setValue(DefUseMap[Input]);
        DefUseMap[Input]->addUse(
            &Operand); // I might have brain damage (depricated)
      } else {
        // TODO fetch info from value data or smth
      }
    }

    std::cout << "  Results:\n";
    for (auto I = 0; I < Node.output_size(); ++I) {
      auto &Output = Node.output().at(I);
      std::cout << "    " << Output << '\n';
      if (DefUseMap.find(Output) != DefUseMap.end()) {
        throw(std::runtime_error("Multiple result definition: " + Output +
                                 "\ngit gud, casul"));
      }

      DefUseMap.emplace(std::make_pair(Output, &AddedNode->getResultRef(I)));
    }
  }
  return GiantGraph{std::move(Nodes)};
}
} // namespace GiantGraph
