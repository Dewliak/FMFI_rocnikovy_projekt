//
// Created by dewliak on 3/10/26.
//

#ifndef ROCNIKOVY_PROJEKT_UTILITIES_H
#define ROCNIKOVY_PROJEKT_UTILITIES_H

#include "graph/Edge.h"
#include "graph/AdjacencyListGraph.h"
#include "defect_search/assignmentGeneration.h"
#include "defect_search/defectComputing.h"

// TODO: might be a good idea to put M1, M2 M3 into a solution structure (or just to rename that structure to be more general)
int getEdgeColor(const Edge& e, const std::set<Edge>& M1, const std::set<Edge>& M2, const std::set<Edge>& M3);

int getMissingVertexColor(int vertex, const AdjacencyListGraph& G, const std::set<Edge>& M1, const std::set<Edge>& M2, const std::set<Edge>& M3);

vector<Edge> findAlternatingPath(const AdjacencyListGraph& G, int start, int colorA, int colorB, const set<Edge>& M1, const set<Edge>& M2, const set<Edge>& M3);

Edge findEdgeAtVertex(const std::set<Edge>& M, int v);

void insertEdgeIntoMatching(std::set<Edge>& M, const Edge& e);

void repairMatching(
    const AdjacencyListGraph& G,
    set<Edge>& M1, set<Edge>& M2, set<Edge>& M3,
    int targetColor);

vector<Solution> extendMatchings(const AdjacencyListGraph& G, int u, int v, set<Edge> M1, set<Edge> M2, set<Edge> M3);

#endif //ROCNIKOVY_PROJEKT_UTILITIES_H