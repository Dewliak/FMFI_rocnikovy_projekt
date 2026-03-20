//
// Created by dewliak on 3/10/26.
//

#ifndef ROCNIKOVY_PROJEKT_DEFECT_COMPUTING_H
#define ROCNIKOVY_PROJEKT_DEFECT_COMPUTING_H

#include "graph/Edge.h"
#include "graph/AdjacencyListGraph.h"

int computeDefect( const AdjacencyListGraph& G, const set<Edge>& M1, const set<Edge>& M2, const set<Edge>& M3);

#endif //ROCNIKOVY_PROJEKT_DEFECT_COMPUTING_H