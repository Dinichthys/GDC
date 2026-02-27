#pragma once

#include <string>

#include "graph/Graph.hpp"

namespace GraphVizardry {

static const std::string DOT_LOG_FILE_NAME = "log/dot.dot";

void dumpGraphViz(const GiantGraph::GiantGraph &GiantGraph);

} // namespace GraphVizardry
