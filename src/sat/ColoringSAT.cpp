//
// Created by dewliak on 10/5/25.
//

#include "../../include/sat/ColoringSAT.h"

// ! TEMPORARY
#include "../../include/sat/CadicalSAT.h"


#include <iostream>
#include <ostream>
#include <memory>


ColoringSAT::ColoringSAT(const IGraph& g, int color_count): graph(g), numColors(color_count) {
    satSolver = std::make_unique<CadicalSAT>();
    encodeConstraints();

}

int ColoringSAT::var(int vertex, int color) {
    // utility for encoding node with color
    return vertex * numColors + color + 1;
}

void ColoringSAT::encodeConstraints() {
    node_atleast_one_color();
    no_multiple_colors_of_node();
    adjecent_nodes_differnet_colors();

    // every edge has atleast one color, and not more than one color
    EdgeList edge_list = graph.getEdgeList();
    vector<Edge> edges =  edge_list.getEdgeList();
    map<Edge,int> edge_map = edge_list.getEdgeMap();
    vector<pair<int,bool>> clause = vector<pair<int,bool>>();
    for (int i =0; i <edges.size(); i++) {


        //atleast one color
        for (int k = 0; k < numColors; k++) {
            clause.emplace_back(var(i,k),true);
        }
        satSolver->add_clause(clause);
        clause.clear();

        // not more than one color
        for (int k1 = 0; k1 < numColors; k1++) {
            for (int k2 = k1 + 1; k2 < numColors; k2++) {
                clause.emplace_back(var(i,k1),false);
                clause.emplace_back(var(i,k2),false);

                satSolver->add_clause(clause);
                clause.clear();
            }
        }
    }

    // adjecent edges have different color
    clause.clear();
    for (int vertex: graph.getVertices()) {
        for (Edge e1: graph.getNeighborEdges(vertex)) {
            for (Edge e2: graph.getNeighborEdges(vertex)) {
                if (e1 == e2) {
                    continue;
                }

                for (int k =0; k < numColors; k++) {
                    clause.emplace_back(var(edge_map[e1],k),false);
                    clause.emplace_back(var(edge_map[e2],k),false);
                    satSolver->add_clause(clause);
                    clause.clear();
                }
            }
        }
    }


}

bool ColoringSAT::solve() {

    SolveResult result = satSolver->solve();

    if (result == SolveResult::SAT) {
        satisfied = true;
        return true;
    }
    return false;

}

std::vector<int> ColoringSAT::getColoring() {
    if (!satisfied) {
        return {};
    }
    vector<int> colors = vector<int>(graph.getVertexCount(), -1);
    for (int i = 0; i < graph.getVertexCount(); i++) {
        for (int k = 0; k < numColors;k++) {
            cout << i << " - " << k << ": " << satSolver->variable_value(var(i,k)) << endl;
            if (satSolver->variable_value(var(i,k))) {

                colors[i] = k;

            }
        }
    }

    return colors;
}

