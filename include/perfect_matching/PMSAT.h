//
// Created by dewliak on 10/22/25.
//

#ifndef ROCNIKOVY_PROJEKT_PMSAT_H
#define ROCNIKOVY_PROJEKT_PMSAT_H


#include <set>
#include <vector>
#include <memory>
#include "graph/IGraph.h"
#include "sat/ISAT.h"


template<typename T>
std::set<std::set<T>> all_subsets_size_k(std::vector<T> array, int k);

class PMSAT {
    public:
    PMSAT(const IGraph &g, int color_count=3);
    PMSAT(const PMSAT&) = delete;
    PMSAT& operator=(const PMSAT&) = delete;

    void setSAT(std::unique_ptr<ISAT> sat);
    void encodeConstraints();
    bool solve();
    std::vector<int> getSets() const; // solution mapping vertex -> color
    bool isSatisfied() const;


private:

    EdgeList edge_list_;

    const IGraph& graph;
    bool satisfied = false;
    const int numColors;
    std::unique_ptr<ISAT> satSolver = nullptr;

};


#endif //ROCNIKOVY_PROJEKT_PMSAT_H