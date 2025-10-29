//
// Created by dewliak on 10/5/25.
//

#include "../../include/sat/ColoringSAT.h"

// ! TEMPORARY
#include "../../include/sat/CadicalSAT.h"


#include <iostream>
#include <ostream>
#include <memory>


ColoringSAT::ColoringSAT(const Graph &g, int color_count): graph(g), numColors(color_count) {
    solver = std::make_unique<CadicalSAT>();
    encodeConstraints();

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
        vector<pair<int,bool>> clause = vector<pair<int,bool>>();
        for (int k = 0; k < numColors; k++) {
            clause.push_back(make_pair(var(i,k),true));
        }
        solver->add_clause(clause);
    }
}

void ColoringSAT::no_multiple_colors_of_node(){
    int vertex_count = graph.getVerticesCount();

    // iterate every node
    for (int i =0; i < vertex_count; i++) {
        // add every color
        for (int k1 = 0; k1 < numColors; k1++) {
            for (int k2 = k1 + 1; k2 < numColors; k2++) {
                vector<pair<int,bool>> clause = vector<pair<int,bool>>();
                clause.push_back(make_pair(var(i,k1),false));
                clause.push_back(make_pair(var(i,k2),false));
                solver->add_clause(clause);
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
                    vector<pair<int,bool>> clause = vector<pair<int,bool>>();
                    clause.push_back(make_pair(var(i,k),false));
                    clause.push_back(make_pair(var(j,k),false));
                    solver->add_clause(clause);

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

    SolveResult result = solver->solve();

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
    vector<int> colors = vector<int>(graph.getVerticesCount(), -1);
    for (int i = 0; i < graph.getVerticesCount(); i++) {
        for (int k = 0; k < numColors;k++) {
            cout << i << " - " << k << ": " << solver->variable_value(var(i,k)) << endl;
            if (solver->variable_value(var(i,k))) {

                colors[i] = k;

            }
        }
    }

    return colors;
}

