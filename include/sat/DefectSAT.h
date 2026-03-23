//
// Created by dewliak on 3/20/26.
//

#ifndef ROCNIKOVY_PROJEKT_DEFECTSAT_H
#define ROCNIKOVY_PROJEKT_DEFECTSAT_H
#include <memory>

#include "ISAT.h"
#include "defect_search/structures.h"
#include "graph/IGraph.h"

struct ExtractionResult {
    Solution solution;           // M1, M2, M3
    std::set<Edge> defectEdges;  // the 3 uncovered edges
    int hammingDistance;         // how many G' edges changed color vs baseline
    std::vector<std::pair<Edge,int>> changedEdges; // which ones changed, and to what
};

class DefectSAT {
public:
    DefectSAT(const IGraph& g, const EdgeList& edgeList, std::map<int,int> baseline, int colorCount=3);
    DefectSAT(const DefectSAT&) = delete;
    DefectSAT& operator=(const DefectSAT&) = delete;

    bool solveAtDistance(int k);
    Solution extractSolution();

    void encodeConstraints();
    ExtractionResult extractSolution2();
    bool solve();
    std::vector<int> getSolution();
    std::vector<Solution> getAllSolutions();
private:
    EdgeList edgeList;
    map<int,int> colorBaseline;
    int  numColors;
    int numEdges;
    bool satisfied = false;;
    const IGraph& graph;
    int var(int edge, int color);

    int uncovVar(int i);

    int changedVar(int i);

    void encodeMatchingConstraints();

    void encodeUncoveredVariables();

    void encodeExactlyThreeDefect();

    void encodeChangedVariables();

    int addAtMostK(const vector<int> &vars, int k, int firstAuxVar);

    void addBlockingClause(const Solution& sol);

    unique_ptr<ISAT> satSolver;
};


#endif //ROCNIKOVY_PROJEKT_DEFECTSAT_H