#pragma once
#include <fstream>
#include <onnx.pb.h>

#include "Graph.hpp"

// Builds GiantGraph from ONNX graph

namespace GiantGraph {
class GraphBuilder {
public:
  explicit GraphBuilder(const std::string &Path) {
    loadModel(Path);
  }
  ~GraphBuilder() = default;

  // Made public so the user can swap models
  // without initializing another builder.
  void loadModel(const std::string &Path) {

    std::ifstream Input(Path, std::ios::binary);
    if (!Model.ParseFromIstream(&Input)) {
      throw std::runtime_error(
          "Failed to parse YOUR ONNX model, git gut, casul");
    }
  }


  GiantGraph buildGraph();

private:
  onnx::ModelProto Model;
};
} // namespace GiantGraph
