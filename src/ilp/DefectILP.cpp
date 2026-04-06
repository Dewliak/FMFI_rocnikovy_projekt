//
// Created by dewliak on 4/2/26.
//

#include "ilp/DefectILP.h"

#include "ilp/DefectILP.h"
#include "Highs.h"
#include <cassert>
#include <cmath>
#include <iostream>

DefectILP::DefectILP(const IGraph &g,
                     const EdgeList &el,
                     const std::map<int, int> &bl)
    : graph(g), edgeList(el), baseline(bl) {
    edges = edgeList.getEdgeList();
    edgeMap = edgeList.getEdgeMap();
    numEdges = edges.size();
}

ILPResult DefectILP::solve() {
    Highs highs;
    highs.setOptionValue("output_flag", false);

    // tried some parameters, because there were some errors with the solution, since this isnt an integer solve by base
    highs.setOptionValue("mip_rel_gap", 0.0);
    highs.setOptionValue("mip_abs_gap", 0.0);
    highs.setOptionValue("primal_feasibility_tolerance", 1e-9);
    highs.setOptionValue("mip_feasibility_tolerance", 1e-9);

    int N = totalCols();

    // building the matrice
    // In exactly one matching
    for (int i = 0; i < numEdges; i++)
        for (int m = 0; m < 3; m++)
            highs.addVar(0.0, 1.0);

    // defect
    for (int i = 0; i < numEdges; i++)
        highs.addVar(0.0, 1.0);

    // does it differ from the original coloring

    for (int i = 0; i < numEdges; i++)
        for (int m = 0; m < 3; m++)
            highs.addVar(0.0, 1.0);

    // mark all as integer
    for (int col = 0; col < N; col++)
        highs.changeColIntegrality(col, HighsVarType::kInteger);

    // GOAL: minimize sum of difference [i][m] for G' edges only
    for (int i = 0; i < numEdges; i++)
        if (baseline.count(i))
            for (int m = 0; m < 3; m++)
                highs.changeColCost(cCol(i, m), 1.0);

    highs.changeObjectiveSense(ObjSense::kMinimize);


    for (int m = 0; m < 3; m++) {
        for (int v: graph.getVertices()) {
            auto neighbors = graph.getNeighborEdges(v);

            std::vector<int> idx;
            std::vector<double> val;
            for (const Edge &e: neighbors) {
                idx.push_back(bCol(edgeMap.at(e), m));
                val.push_back(1.0);
            }

            // at least one neighbor edge in matching m (= 1 for cubic graph)
            highs.addRow(1.0, kHighsInf, idx.size(), idx.data(), val.data());

            // for pair exactly one
            for (int a = 0; a < (int) neighbors.size(); a++) {
                for (int b = a + 1; b < (int) neighbors.size(); b++) {
                    int cols[2] = {
                        bCol(edgeMap.at(neighbors[a]), m),
                        bCol(edgeMap.at(neighbors[b]), m)
                    };
                    double vals[2] = {1.0, 1.0};
                    highs.addRow(-kHighsInf, 1.0, 2, cols, vals);
                }
            }
        }
    }

    //defect definition

    for (int i = 0; i < numEdges; i++) {
        // u[i] + b[i][m] <= 1  for each m  (if in any matching, not defect)
        for (int m = 0; m < 3; m++) {
            int cols[2] = {uCol(i), bCol(i, m)};
            double vals[2] = {1.0, 1.0};
            highs.addRow(-kHighsInf, 1.0, 2, cols, vals);
        }

        // u[i] + m[i][0] + m[i][1] + m[i][2] >= 1
        // if all m are 0, u must be 1  so it isnt in any matching
        int cols[4] = {uCol(i), bCol(i, 0), bCol(i, 1), bCol(i, 2)};
        double vals[4] = {1.0, 1.0, 1.0, 1.0};
        highs.addRow(1.0, kHighsInf, 4, cols, vals);
    }

    //exactly 3 defect edges

    {
        std::vector<int> cols;
        std::vector<double> vals;
        for (int i = 0; i < numEdges; i++) {
            cols.push_back(uCol(i));
            vals.push_back(1.0);
        }
        highs.addRow(3.0, 3.0, cols.size(), cols.data(), vals.data());
    }

    //difference [i][m] = 1 if they are not equal

    for (int i = 0; i < numEdges; i++) {
        if (!baseline.count(i)) continue; // u,v edges — skip

        int baseM = baseline.at(i);

        for (int m = 0; m < 3; m++) {
            double base = (m == baseM) ? 1.0 : 0.0;
            int b = bCol(i, m);
            int c = cCol(i, m);

            // c >= b - base  →  c - b >= -base
            {
                int cols[2] = {c, b};
                double vals[2] = {1.0, -1.0};
                highs.addRow(-base, kHighsInf, 2, cols, vals);
            }
            // c >= base - b  →  c + b >= base
            {
                int cols[2] = {c, b};
                double vals[2] = {1.0, 1.0};
                highs.addRow(base, kHighsInf, 2, cols, vals);
            }
            // c <= b + base  →  c - b <= base
            {
                int cols[2] = {c, b};
                double vals[2] = {1.0, -1.0};
                highs.addRow(-kHighsInf, base, 2, cols, vals);
            }
            // c <= 2 - b - base  →  c + b <= 2 - base
            {
                int cols[2] = {c, b};
                double vals[2] = {1.0, 1.0};
                highs.addRow(-kHighsInf, 2.0 - base, 2, cols, vals);
            }
        }
    }

    // solve

    highs.run();

    ILPResult result;
    result.found = false;
    result.hammingDistance = -1;

    auto status = highs.getModelStatus();

    // accept kOptimal only
    if (status != HighsModelStatus::kOptimal) {
        std::cout << "ILP: status = " << (int) status << " — not proven optimal\n";
        return result;
    }

    result.found = true;
    double value;
    highs.getInfoValue("objective_function_value", value);
    result.hammingDistance = (int) std::round(value);

    const std::vector<double> &sol = highs.getSolution().col_value;

    for (int i = 0; i < numEdges; i++) {
        Edge e = edges[i];
        if (std::round(sol[bCol(i, 0)]) == 1.0) result.solution.M1.insert(e);
        if (std::round(sol[bCol(i, 1)]) == 1.0) result.solution.M2.insert(e);
        if (std::round(sol[bCol(i, 2)]) == 1.0) result.solution.M3.insert(e);
        if (std::round(sol[uCol(i)]) == 1.0) result.defectEdges.insert(e);
    }

    int actualDefect = result.defectEdges.size();
    if (actualDefect != 3) {
        std::cerr << "ILP WARNING: defect = " << actualDefect << " (expected 3)\n";
    }

    // verify matching constraints hold
    auto edgeMapLocal = edgeList.getEdgeMap();
    for (int m = 0; m < 3; m++) {
        const std::set<Edge> &M = (m == 0)
                                      ? result.solution.M1
                                      : (m == 1)
                                            ? result.solution.M2
                                            : result.solution.M3;
        for (int v: graph.getVertices()) {
            int cover = 0;
            for (const Edge &e: graph.getNeighborEdges(v))
                if (M.count(e)) cover++;
            if (cover != 1) {
                std::cerr << "ILP WARNING: vertex " << v << " covered " << cover << " times in M" << (m + 1) << "\n";
            }
        }
    }

    return result;
}