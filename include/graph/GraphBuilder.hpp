#pragma once
#include <fstream>
#include <onnx.pb.h>

#include "Graph.hpp"

// Builds GiantGraph from ONNX graph

namespace GiantGraph {
class GraphBuilder {
public:
  explicit GraphBuilder(const std::string &Path, GiantGraph &Graph)
      : OutputGraph(Graph) {
    loadModel(Path);
  }
  ~GraphBuilder() = default;

  // Made public so the user can swap models
  // without initializing another builder.
  void loadModel(const std::string &Path) {

    std::ifstream Input(Path, std::ios::binary);
    if (!Model.ParseFromIstream(&Input)) {
      throw std::runtime_error("Failed to parse ONNX model");
    }
  }

  // same reasoning as above
  void setOutputGraph(GiantGraph &Graph) { OutputGraph = Graph; };

  void buildGraph();

private:
  onnx::ModelProto Model;
  GiantGraph &OutputGraph;
};
} // namespace GiantGraph
