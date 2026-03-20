//
// Created by dewliak on 3/10/26.
//

#include "defect_search/defectComputing.h"

int computeDefect(const AdjacencyListGraph& G, const set<Edge>& M1, const set<Edge>& M2, const set<Edge>& M3) {
    int defect = 0;

    for(const Edge& e : G.getEdges())
    {
        int cover = 0;

        if(M1.contains(e)) {
            cover++;
        }
        if(M2.contains(e)) {
            cover++;
        }
        if(M3.contains(e)) {
            cover++;
        }

        if(cover == 0) {
            defect++;
        }
    }

    return defect;
}

