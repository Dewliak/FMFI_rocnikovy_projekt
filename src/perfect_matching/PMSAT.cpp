//
// Created by dewliak on 10/28/25.
//

#include "perfect_matching/PMSAT.h"

#include <algorithm>
#include <set>
#include <vector>

#include "graph/EdgeList.h"


template<typename T>
std::set<std::set<T>> all_subsets_size_k(std::vector<T> array, int k) {
    // return all subsets of size k
    std::vector<bool> used(array.size(), false);
    for (int i =0; i < k; i++) {
        used[i] = true;
    }

    std::set<std::set<T>> subsets;

    do {
        std::set<T> subset;
        for (int i =0; i < array.size(); i++) {
            if (used[i]) {
                subsets.add(array[i]);
            }
        }
        subsets.add(subset);

    }while (std::next_permutation(used.begin(), used.end()));

    return subsets;
}


void cnf_k(EdgeList edge_list, int k) {

    return;

}



/*
 * THE PLAN:
 *  1. generate perfect matching
 *  2. get defect
 */
PMSAT::PMSAT(const IGraph &g, int color_count) : edge_list_(g.getEdgeList()), graph(g), numColors(color_count) {
}

void PMSAT::setSAT(std::unique_ptr<ISAT> sat) {
    satSolver = std::move(sat);
}

void PMSAT::encodeConstraints() {
    if (satSolver == nullptr) {
        throw std::runtime_error("A solver hasn't been set");
    }

    map<Edge,int> edge_map = edge_list_.getEdgeMap();

    for (int vertex: graph.getVertices()) {

        // atleast one edge
        vector<pair<int,bool>> clause = vector<pair<int,bool>>();
        vector<Edge> neighbors = graph.getNeighborEdges(vertex);
        for (Edge e: neighbors) {
            clause.emplace_back(edge_map[e],true);
        }
        satSolver->add_clause(clause);
        clause.clear();

        for (int i =0; i < neighbors.size(); i++) {
            for (int j = i + 1; j < neighbors.size();j++) {
                clause.emplace_back(edge_map[neighbors[i]],false);
                clause.emplace_back(edge_map[neighbors[j]],false);
                satSolver->add_clause(clause);
                clause.clear();
            }
        }
    }

}

bool PMSAT::solve() {
    SolveResult result = satSolver->solve();

    if (result == SolveResult::SAT) {
        satisfied = true;
        return true;
    }
    return false;
}

std::vector<int> PMSAT::getSets() const {
}

bool PMSAT::isSatisfied() const {
    return satisfied;
}



