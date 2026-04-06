//
// Created by dewliak on 4/2/26.
//

#ifndef ROCNIKOVY_PROJEKT_DEFECTILP_H
#define ROCNIKOVY_PROJEKT_DEFECTILP_H

#include "graph/IGraph.h"
#include "graph/EdgeList.h"
#include "defect_search/structures.h"
#include <map>
#include <set>
#include <vector>

struct ILPResult {
    Solution solution;
    std::set<Edge> defectEdges;
    int hammingDistance;
    bool found;
};

class DefectILP {
public:
    DefectILP(const IGraph &g,
              const EdgeList &edgeList,
              const std::map<int, int> &baseline);

    ILPResult solve();

private:
    const IGraph &graph;
    const EdgeList &edgeList;
    const std::map<int, int> &baseline;

    int numEdges;
    std::vector<Edge> edges;
    std::map<Edge, int> edgeMap;

    // column indices in HiGHS model
    // b[i][m]: edge i in matching m
    int bCol(int i, int m) { return i * 3 + m; }
    // u[i]: edge i is uncovered (defect)
    int uCol(int i) { return numEdges * 3 + i; }
    // c[i][m]: edge i changed membership in matching m vs baseline
    int cCol(int i, int m) { return numEdges * 4 + i * 3 + m; }

    int totalCols() { return numEdges * 7; }
};

#endif //ROCNIKOVY_PROJEKT_DEFECTILP_H
