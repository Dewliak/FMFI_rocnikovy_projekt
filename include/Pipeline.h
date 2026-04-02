//
// Created by dewliak on 4/2/26.
//

#ifndef ROCNIKOVY_PROJEKT_PIPELINE_H
#define ROCNIKOVY_PROJEKT_PIPELINE_H
#include <string>

#include "defect_search/structures.h"
#include "graph/EdgeList.h"

struct GraphColoringData {
    std::string originalGraphFormat;
    EdgeList modifiedGraphEdgeList;
    Solution originalSolution;
    map<int,int> baseline;
    EdgeList originalGraphEdgeList;


    GraphColoringData(string graphFormat, std::map<int,int>b, EdgeList mEdgeList, Solution solution, EdgeList oEdgeList){
        baseline = b;
        originalGraphFormat = graphFormat;
        modifiedGraphEdgeList = mEdgeList;
        originalSolution = solution;
        originalGraphEdgeList = oEdgeList;
    }
};

void printMatchings(const std::set<Edge>& M1,
                    const std::set<Edge>& M2,
                    const std::set<Edge>& M3);

GraphColoringData generateColoring(string graph6format, int vertex1, int vertex2);
vector<GraphColoringData> generateAllColoring(string graph6format, int vertex1, int vertex2);

Solution findClosestWithDefectThree(
    const string originalGraphFormat,
    const EdgeList& modifiedGraphEdgeList,
    const Solution& originalSolution,
    const map<int,int>& baseline,
    SearchStrategy strategy = SearchStrategy::BruteForce);


#endif //ROCNIKOVY_PROJEKT_PIPELINE_H