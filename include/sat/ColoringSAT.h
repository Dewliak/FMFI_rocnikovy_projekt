//
// Created by dewliak on 10/5/25.
//

#ifndef ROCNIKOVY_PROJEKT_COLORINGSAT_H
#define ROCNIKOVY_PROJEKT_COLORINGSAT_H

#include "../graph/Graph.h"
#include <memory>

#include "sat/ISAT.h"
#include "graph/IGraph.h"


class ColoringSAT {
public:
    ColoringSAT(const IGraph& g, int color_count=3);
    ColoringSAT(const ColoringSAT&) = delete;
    ColoringSAT& operator=(const ColoringSAT&) = delete;

    void encodeConstraints();
    bool solve();
    std::vector<int> getColoring(); // solution mapping vertex -> color
private:
    void node_atleast_one_color(); // constraint
    void no_multiple_colors_of_node(); // constraint
    void adjecent_nodes_differnet_colors();

    const IGraph& graph;
    bool satisfied = false;
    int numColors;
    int var(int vertex, int color);
    std::unique_ptr<ISAT> satSolver;
};


#endif //ROCNIKOVY_PROJEKT_COLORINGSAT_H