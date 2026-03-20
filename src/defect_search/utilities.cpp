//
// Created by dewliak on 3/10/26.
//

#include "defect_search/utilities.h"

#include <iostream>
#include <stack>

int getEdgeColor(const Edge& e, const std::set<Edge>& M1, const std::set<Edge>& M2, const std::set<Edge>& M3) {
    if (M1.contains(e)) return 0;
    if (M2.contains(e)) return 1;
    if (M3.contains(e)) return 2;
    return -1;
}

int getMissingVertexColor(const int vertex, const AdjacencyListGraph& G, const std::set<Edge>& M1, const std::set<Edge>& M2, const std::set<Edge>& M3) {

    // G should be the G - {u,v} so these vertexes have only 2 adjecent edges
    int answer = 0 + 1 + 2;
    for (Edge e : G.getNeighborEdges(vertex)) {
        if (M1.contains(e)) answer -= 0;
        if (M2.contains(e)) answer -= 1;
        if (M3.contains(e)) answer -= 2;
    }

    return answer;
}


vector<Edge> findAlternatingPath(
        const AdjacencyListGraph& G,
        int start,
        int colorA,
        int colorB,
        const set<Edge>& M1,
        const set<Edge>& M2,
        const set<Edge>& M3)
{
    stack<pair<int,int>> st;
    map<int,bool> visited;
    map<int,Edge> parentEdge;

    st.emplace(start, colorA);

    int targetVertex = -1;

    while(!st.empty())
    {
        pair<int,int> p = st.top();
        st.pop();

        int v = p.first;
        int expectedColor = p.second;

        if(visited[v]) continue;
        visited[v] = true;

        for(const Edge& e : G.getNeighborEdges(v))
        {
            int c = getEdgeColor(e, M1, M2, M3);

            // only follow alternating colors
            if(c != expectedColor) continue;

            int u = (e.getFirst() == v) ? e.getSecond() : e.getFirst();

            if(visited[u]) continue;

            parentEdge[u] = e;

            // check if u is exposed in the matching
            Edge incident = findEdgeAtVertex(M1, u);

            if(incident.first == -1)
            {
                targetVertex = u;
                break;
            }

            int nextColor = (expectedColor == colorA) ? colorB : colorA;
            st.emplace(u, nextColor);
        }

        if(targetVertex != -1)
            break;
    }

    vector<Edge> path;

    if(targetVertex == -1)
        return path;

    int current = targetVertex;

    while(current != start)
    {
        Edge e = parentEdge[current];
        path.push_back(e);

        current = (e.getFirst() == current) ? e.getSecond() : e.getFirst();
    }

    reverse(path.begin(), path.end());

    return path;
}

Edge findEdgeAtVertex(const std::set<Edge>& M, int v)
{
    for (const Edge& e : M)
        if (e.first == v || e.second == v)
            return e;

    return Edge(-1,-1); // not found
}

void insertEdgeIntoMatching(std::set<Edge>& M, const Edge& e)
{
    Edge e1 = findEdgeAtVertex(M, e.first);
    Edge e2 = findEdgeAtVertex(M, e.second);

    if (e1.first != -1)
        M.erase(e1);

    if (e2.first != -1 && !(e1 == e2))
        M.erase(e2);

    M.insert(e);
}



void repairMatching(
    const AdjacencyListGraph& G,
    set<Edge>& M1, set<Edge>& M2, set<Edge>& M3,
    int targetColor)   // which matching to repair
{
    set<Edge>& target = (targetColor == 0) ? M1
                      : (targetColor == 1) ? M2 : M3;

    int altA = (targetColor == 0) ? 1 : 0;
    int altB = (targetColor == 0) ? 2 : (targetColor == 1) ? 2 : 1;

    for (int v : G.getVertices()) {
        if (findEdgeAtVertex(target, v).first != -1) continue;

        // Now findAlternatingPath gets real M1,M2,M3 — colors are correct
        vector<Edge> path = findAlternatingPath(
            G, v, targetColor, altA, M1, M2, M3);

        for (const Edge& e : path) {
            int c = getEdgeColor(e, M1, M2, M3);
            set<Edge>& src = (c == 0) ? M1 : (c == 1) ? M2 : M3;
            src.erase(e);
            target.insert(e);
        }
    }
}

vector<Solution> extendMatchings( const AdjacencyListGraph& G, int u, int v, set<Edge> M1, set<Edge> M2, set<Edge> M3) {
    vector<Solution> solutions = {};


    auto uEdges = G.getNeighborEdges(u);
    auto vEdges = G.getNeighborEdges(v);

    vector<Assignment> assignments =
            generateAssignments(uEdges, vEdges);

    for(const Assignment& a : assignments)
    {
        set<Edge> M1copy = M1;
        set<Edge> M2copy = M2;
        set<Edge> M3copy = M3;

        // insert the assignemnt edges
        insertEdgeIntoMatching(M1copy, a.uEdge[0]);
        insertEdgeIntoMatching(M1copy, a.vEdge[0]);

        insertEdgeIntoMatching(M2copy, a.uEdge[1]);
        insertEdgeIntoMatching(M2copy, a.vEdge[1]);

        insertEdgeIntoMatching(M3copy, a.uEdge[2]);
        insertEdgeIntoMatching(M3copy, a.vEdge[2]);

        // repair so it will be a perfect matching - or atleast it tries to be because of the defect
        // TODO: do this proberly because now there is no gurantee
        repairMatching(G, M1copy, M2copy, M3copy, 0);
        repairMatching(G, M1copy, M2copy, M3copy, 1);
        repairMatching(G, M1copy, M2copy, M3copy, 2);



        // calc defect
        int defect = computeDefect(G, M1copy, M2copy, M3copy);

        if(defect == 3)
        {
            Solution solution;
            solution.M1 = M1copy;
            solution.M2 = M2copy;
            solution.M3 = M3copy;
            solutions.push_back(solution);

            std::cout << "Found solution\n";
        }
    }

    return solutions;
}
