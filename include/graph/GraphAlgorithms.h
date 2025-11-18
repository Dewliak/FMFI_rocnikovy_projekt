//
// Created by dewliak on 10/29/25.
//

#ifndef ROCNIKOVY_PROJEKT_GRAPHALGORITHMS_H
#define ROCNIKOVY_PROJEKT_GRAPHALGORITHMS_H

#include "IGraph.h"
#include "Edge.h"

namespace GraphAlgorithms{

    int shortest_cycle(const IGraph& graph);
    Matrix graph6format_to_matrix(std::string graph6format);
}

#endif //ROCNIKOVY_PROJEKT_GRAPHALGORITHMS_H