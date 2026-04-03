//
// Created by dewliak on 3/10/26.
//

#include "defect_search/defectComputing.h"
#include "defect_search/structures.h"

int computeDefect(const AdjacencyListGraph &G, const Solution& solution) {
    int defect = 0;

    for (const Edge &e: G.getEdges()) {
        int cover = 0;

        if (solution.M1.contains(e)) {
            cover++;
        }
        if (solution.M2.contains(e)) {
            cover++;
        }
        if (solution.M3.contains(e)) {
            cover++;
        }

        if (cover == 0) {
            defect++;
        }
    }

    return defect;
}

