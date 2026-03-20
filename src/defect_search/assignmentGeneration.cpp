//
// Created by dewliak on 3/10/26.
//


#include "defect_search/assignmentGeneration.h"

std::vector<Assignment> generateAssignments(
        const std::vector<Edge>& uEdges,
        const std::vector<Edge>& vEdges)
{
    std::vector<Assignment> result;

    for(const Edge& eu0 : uEdges)
        for(const Edge& eu1 : uEdges)
            for(const Edge& eu2 : uEdges)
            {
                if(eu0 == eu1 || eu0 == eu2 || eu1 == eu2)
                    continue;

                for(const Edge& ev0 : vEdges)
                    for(const Edge& ev1 : vEdges)
                        for(const Edge& ev2 : vEdges)
                        {
                            if(ev0 == ev1 || ev0 == ev2 || ev1 == ev2)
                                continue;

                            Assignment a;
                            a.uEdge[0] = eu0;
                            a.uEdge[1] = eu1;
                            a.uEdge[2] = eu2;

                            a.vEdge[0] = ev0;
                            a.vEdge[1] = ev1;
                            a.vEdge[2] = ev2;

                            result.push_back(a);
                        }
            }

    return result;
}