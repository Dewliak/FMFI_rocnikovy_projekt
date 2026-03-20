//
// Created by dewliak on 3/10/26.
//

#ifndef ROCNIKOVY_PROJEKT_STRUCTURES_H
#define ROCNIKOVY_PROJEKT_STRUCTURES_H

#include "graph/Edge.h"


struct Assignment
{
    Edge uEdge[3];  // edge at u for M1,M2,M3
    Edge vEdge[3];  // edge at v for M1,M2,M3
};

struct Solution { // to store a solution for later calculations - distance from the original
    std::set<Edge> M1;
    std::set<Edge> M2;
    std::set<Edge> M3;
};

#endif //ROCNIKOVY_PROJEKT_STRUCTURES_H