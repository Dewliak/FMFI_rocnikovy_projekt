//
// Created by dewliak on 3/10/26.
//

#ifndef ROCNIKOVY_PROJEKT_ASSIGNMENT_GENERATION_H
#define ROCNIKOVY_PROJEKT_ASSIGNMENT_GENERATION_H

#include <vector>
#include "defect_search/structures.h"

// generates each case that needs to be checked
std::vector<Assignment> generateAssignments( const std::vector<Edge>& uEdges, const std::vector<Edge>& vEdges);

#endif //ROCNIKOVY_PROJEKT_ASSIGNMENT_GENERATION_H