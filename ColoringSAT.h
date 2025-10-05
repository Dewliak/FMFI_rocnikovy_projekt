//
// Created by dewliak on 10/5/25.
//

#ifndef ROCNIKOVY_PROJEKT_COLORINGSAT_H
#define ROCNIKOVY_PROJEKT_COLORINGSAT_H

#include "Graph.h"
#include "cadical.hpp"

class ColoringSAT {
public:
    ColoringSAT(const Graph &g, int color_count=3);

    void encodeConstraints();
    bool solve();
    std::vector<int> getColoring(); // solution mapping vertex -> color
private:
    void node_atleast_one_color(); // constraint
    void no_multiple_colors_of_node(); // constraint
    void adjecent_nodes_differnet_colors();

    Graph graph;
    int numColors;
    int var(int vertex, int color);
    CaDiCaL::Solver solver;
};


#endif //ROCNIKOVY_PROJEKT_COLORINGSAT_H