//
// Created by dewliak on 10/5/25.
//

#include "ColoringSAT.h"

#include <iostream>
#include <ostream>
#include <stdexcept>
ColoringSAT::ColoringSAT(const Graph &g, int color_count): graph(g), numColors(color_count) {
}

int ColoringSAT::var(int vertex, int color) {
    // utility for encoding node with color
    return vertex * numColors + color + 1;
}

void ColoringSAT::node_atleast_one_color() {
    int vertex_count = graph.getVerticesCount();

    // iterate every node
    for (int i =0; i < vertex_count; i++) {
        // add every color
        for (int k = 0; k < numColors; k++) {
            solver.add(var(i,k));
        }
        solver.add(0);
    }
}

void ColoringSAT::no_multiple_colors_of_node(){
    int vertex_count = graph.getVerticesCount();

    // iterate every node
    for (int i =0; i < vertex_count; i++) {
        // add every color
        for (int k1 = 0; k1 < numColors; k1++) {
            for (int k2 = k1 + 1; k2 < numColors; k2++) {
                solver.add(-var(i,k1));
                solver.add(-var(i,k2));
                solver.add(0);
            }
        }
    }
}

void ColoringSAT::adjecent_nodes_differnet_colors() {
    int vertex_count = graph.getVerticesCount();

    for (int i =0; i < vertex_count; i++) {
        for (int j = 0; j < i; j++) {
            if (graph.hasEdge(i,j)) {
                for (int k =0; k < numColors; k++) {
                    solver.add(-var(i,k));
                    solver.add(-var(j,k));
                    solver.add(0);
                }
            }

        }
    }
}

void ColoringSAT::encodeConstraints() {
    node_atleast_one_color();
    no_multiple_colors_of_node();
    adjecent_nodes_differnet_colors();
}

bool ColoringSAT::solve() {
    bool sat = solver.solve();
    return sat;
}

std::vector<int> ColoringSAT::getColoring() {
    vector<int> colors = vector<int>(graph.getVerticesCount(), -1);
    for (int i = 0; i < graph.getVerticesCount(); i++) {
        for (int k = 0; k < numColors;k++) {
            cout << i << " - " << k << ": " << solver.val(var(i,k)) << endl;
            if (solver.val(var(i,k)) > 0) {

                colors[i] = k;

            }
        }
    }

    return colors;
}

