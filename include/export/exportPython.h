//
// Created by dewliak on 3/11/26.
//

#ifndef ROCNIKOVY_PROJEKT_EXPORTPYTHON_H
#define ROCNIKOVY_PROJEKT_EXPORTPYTHON_H

#include "defect_search/structures.h"
#include <string>

#include "graph/EdgeList.h"

void exportPython(Solution solution, std::string filename, vector<Edge> edge_list ={}, bool deleted_edges = false);


#endif //ROCNIKOVY_PROJEKT_EXPORTPYTHON_H