//
// Created by dewliak on 3/20/26.
//

#include "sat/DefectSAT.h"

#include <cassert>
#include <memory>

#include "sat/CadicalSAT.h"

DefectSAT::DefectSAT(const IGraph& g, const EdgeList& modifiedEdgeList, std::map<int,int> baseline, int colorCount): graph(g), numColors(colorCount), edgeList(modifiedEdgeList), colorBaseline(baseline){
    satSolver = std::make_unique<CadicalSAT>();
    numEdges = edgeList.size();

    encodeMatchingConstraints();
    encodeUncoveredVariables();
    encodeExactlyThreeDefect();


    // part of the incremental search
    /*
    encodeChangedVariables(); // only needed if you're using Hamming distance
    for (int i = 0; i < numEdges; i++)
        if (colorBaseline.count(i))
            for (int m = 0; m < 3; m++)
                changedVarsList.push_back(changedVar(i, m));
                */

}




bool DefectSAT::solve() {
    satisfied = (satSolver->solve() == SolveResult::SAT);
    return satisfied;

}
std::vector<int> DefectSAT::getSolution() {

}


int DefectSAT::buildAtMostKGated(const std::vector<int> &vars, int k, int firstAux) {
    int n = vars.size();

    auto s = [&](int i, int j) {
        return firstAux + (i - 1) * (k + 1) + (j - 1);
    };

    vector<pair<int,bool>> clause;

    for (int i = 1; i <= n; i++) {
        int x = vars[i - 1];
        for (int j = 1; j <= k + 1; j++) {

            // s[i][j] → s[i-1][j]
            if (i > 1) {
                clause.emplace_back(s(i,   j), false);
                clause.emplace_back(s(i-1, j), true);
                satSolver->add_clause(clause);
                clause.clear();
            }

            // x ∧ s[i-1][j-1] → s[i][j]
            if (j > 1 && i > 1) {
                clause.emplace_back(x,           false);
                clause.emplace_back(s(i-1, j-1), false);
                clause.emplace_back(s(i,   j),   true);
                satSolver->add_clause(clause);
                clause.clear();
            }

            // x → s[i][1]
            if (j == 1) {
                clause.emplace_back(x,       false);
                clause.emplace_back(s(i, 1), true);
                satSolver->add_clause(clause);
                clause.clear();
            }
        }
    }

    // activation variable — sits just after the counter variables
    int actVar = firstAux + n * (k + 1);

    // gated clause: actVar → ¬s[n][k+1]
    // written as: ¬actVar ∨ ¬s[n][k+1]
    // meaning: IF actVar is assumed true, THEN at-most-k is enforced
    clause.emplace_back(actVar,       false);
    clause.emplace_back(s(n, k + 1), false);
    satSolver->add_clause(clause);
    clause.clear();

    // return actVar so caller can assume() it
    return actVar;
}

int DefectSAT::var(int edge, int color) {
    // utility for encoding node with color
    return edge * numColors + color + 1;
}
int DefectSAT::uncovVar(int i) {
    return numEdges * 3 + i + 1;
}
int DefectSAT::changedVar(int i, int m) {
    return numEdges * 4 + i * 3 + m + 1;
}

int DefectSAT::auxBase() {
    return numEdges * 7 + 1;
}

void DefectSAT::encodeMatchingConstraints() {
    // for each matching m and each vertex v:
    // exactly one neighbor edge of v is in Mm
    map<Edge,int> edgeMap = edgeList.getEdgeMap();
    for (int m = 0; m < 3; m++) {
        for (int v : graph.getVertices()) {
            auto neighbors = graph.getNeighborEdges(v);
            // at least one: v must be covered by Mm
            vector<pair<int,bool>> clause;
            for (Edge e : neighbors)
                clause.emplace_back(var(edgeMap[e], m), true);
            satSolver->add_clause(clause);
            clause.clear();

            // at most one: it's a matching, vertex appears once
            for (int a = 0; a < neighbors.size(); a++) {
                for (int b = a + 1; b < neighbors.size(); b++) {
                    clause.emplace_back(var(edgeMap[neighbors[a]], m), false);
                    clause.emplace_back(var(edgeMap[neighbors[b]], m), false);
                    satSolver->add_clause(clause);
                    clause.clear();
                }
            }
        }
    }
}

void DefectSAT::encodeUncoveredVariables() {
    // uncov(i) ↔ ¬b[i][0] ∧ ¬b[i][1] ∧ ¬b[i][2]
    // equivalent to two directions:
    //   (1) uncov(i) → ¬b[i][k]  for each k:  ¬uncov(i) ∨ ¬b[i][k]
    //   (2) ¬b[i][0] ∧ ¬b[i][1] ∧ ¬b[i][2] → uncov(i):
    //       b[i][0] ∨ b[i][1] ∨ b[i][2] ∨ uncov(i)
    vector<pair<int,bool>> clause;
    for (int i = 0; i < numEdges; i++) {
        // direction (1)
        for (int k = 0; k < 3; k++) {
            clause.emplace_back(uncovVar(i), false);
            clause.emplace_back(var(i, k),  false);
            satSolver->add_clause(clause);
            clause.clear();
        }
        // direction (2)
        clause.emplace_back(var(i, 0), true);
        clause.emplace_back(var(i, 1), true);
        clause.emplace_back(var(i, 2), true);
        clause.emplace_back(uncovVar(i), true);
        satSolver->add_clause(clause);
        clause.clear();
    }
}

// ── exactly three defect edges ───────────────────────────────────────────────

void DefectSAT::encodeExactlyThreeDefect() {
    vector<int> uncovVars;
    for (int i = 0; i < numEdges; i++)
        uncovVars.push_back(uncovVar(i));

    int n = uncovVars.size();
    vector<pair<int,bool>> clause;

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            for (int k = j + 1; k < n; k++) {
                for (int l = k + 1; l < n; l++) {

                    // ¬xi ∨ ¬xj ∨ ¬xk ∨ ¬xl
                    clause.emplace_back(uncovVars[i], false);
                    clause.emplace_back(uncovVars[j], false);
                    clause.emplace_back(uncovVars[k], false);
                    clause.emplace_back(uncovVars[l], false);

                    satSolver->add_clause(clause);
                    clause.clear();
                }
            }
        }
    }
}

// ── changed variables ────────────────────────────────────────────────────────

void DefectSAT::encodeChangedVariables() {
    vector<pair<int,bool>> clause;

    for (int i = 0; i < numEdges; i++) {
        if (!colorBaseline.count(i)) continue; // u,v edges — skip

        int baselineM = colorBaseline.at(i); // which matching this edge was in

        for (int m = 0; m < 3; m++) {
            int cv = changedVar(i, m);
            int bv = var(i, m);

            if (m == baselineM) {
                // was IN this matching — changed if now NOT in it
                // changed(i,m) ↔ ¬b[i][m]
                clause.emplace_back(cv, false);
                clause.emplace_back(bv, false);
                satSolver->add_clause(clause); clause.clear();

                clause.emplace_back(cv, true);
                clause.emplace_back(bv, true);
                satSolver->add_clause(clause); clause.clear();
            } else {
                // was NOT in this matching — changed if now IN it
                // changed(i,m) ↔ b[i][m]
                clause.emplace_back(cv, false);
                clause.emplace_back(bv, true);
                satSolver->add_clause(clause); clause.clear();

                clause.emplace_back(cv, true);
                clause.emplace_back(bv, false);
                satSolver->add_clause(clause); clause.clear();
            }
        }
    }
}

// ── iterative deepening entry point ─────────────────────────────────────────

bool DefectSAT::solveAtDistance(int k) {
    satSolver = std::make_unique<CadicalSAT>();

    encodeMatchingConstraints();
    encodeUncoveredVariables();
    encodeExactlyThreeDefect();
    encodeChangedVariables();

    // collect all changed(i,m) for G' edges — 3 per edge
    vector<int> changedVars;
    for (int i = 0; i < numEdges; i++) {
        if (!colorBaseline.count(i)) continue;
        for (int m = 0; m < 3; m++)
            changedVars.push_back(changedVar(i, m));
    }

    //int firstAux = auxBase();
    //defectCounterEnd = addAtMostK(changedVars, k, defectCounterEnd);

    satisfied = (satSolver->solve() == SolveResult::SAT);
    return satisfied;
}

bool DefectSAT::solveAtDistanceIncremental(int k) {
    // build a fresh counter for this k using fresh aux variables
    // each call to this method gets its own non-overlapping variable block
    int actVar = buildAtMostKGated(changedVarsList, k, nextFreeVar);

    // advance nextFreeVar past this counter block + the actVar itself
    // counter uses n*(k+1) variables, actVar is one more
    int n = changedVarsList.size();
    nextFreeVar = actVar + 1;

    // assume actVar = true for this solve call only
    // CaDiCaL consumes the assumption after one solve() — it does not persist
    satSolver->assume(actVar);

    satisfied = (satSolver->solve() == SolveResult::SAT);
    return satisfied;
}

// ── solution extraction ──────────────────────────────────────────────────────



ExtractionResult DefectSAT::extractSolution2() {
    assert(satisfied);
    vector<Edge> edges = edgeList.getEdgeList();
    ExtractionResult result;
    result.hammingDistance = 0;

    for (int i = 0; i < numEdges; i++) {
        Edge e = edges[i];

        // find which color (if any) this edge got
        int assignedColor = -1;
        for (int k = 0; k < 3; k++) {
            if (satSolver->variable_value(var(i, k))) {
                assignedColor = k;
                break;
            }
        }

        // place edge into the right matching or defect set
        switch (assignedColor) {
            case 0:  result.solution.M1.insert(e); break;
            case 1:  result.solution.M2.insert(e); break;
            case 2:  result.solution.M3.insert(e); break;
            default: result.defectEdges.insert(e); break;
        }

        // compute Hamming distance — only for G' edges
        if (colorBaseline.count(i)) {
            int originalColor = colorBaseline.at(i);
            if (assignedColor != originalColor) {
                result.hammingDistance++;
                result.changedEdges.push_back({e, assignedColor});
            }
        }
    }

    return result;
}
Solution DefectSAT::extractSolution() {
    Solution sol;
    vector<Edge> edges = edgeList.getEdgeList();
    for (int i = 0; i < numEdges; i++) {
        if (satSolver->variable_value(var(i, 0))) sol.M1.insert(edges[i]);
        if (satSolver->variable_value(var(i, 1))) sol.M2.insert(edges[i]);
        if (satSolver->variable_value(var(i, 2))) sol.M3.insert(edges[i]);
        // note: no break — edge can be in multiple matchings
    }
    return sol;
}

// ── sequential counter (at-most-k) ──────────────────────────────────────────

int DefectSAT::addAtMostK(const vector<int>& vars, int k, int firstAuxVar) {
    int n = vars.size();

    // s(i,j) means "among the first i variables, at least j are true"
    // laid out linearly: s(i,j) = firstAuxVar + (i-1)*(k+1) + (j-1)
    auto s = [&](int i, int j) {
        return firstAuxVar + (i - 1) * (k + 1) + (j - 1);
    };

    vector<pair<int,bool>> clause;

    for (int i = 1; i <= n; i++) {
        int x = vars[i - 1];

        for (int j = 1; j <= k + 1; j++) {

            // s[i][j] → s[i-1][j]
            // if at least j of first i are true, then at least j of first i-1 are true
            if (i > 1) {
                clause.emplace_back(s(i,   j), false);
                clause.emplace_back(s(i-1, j), true);
                satSolver->add_clause(clause);
                clause.clear();
            }

            // x ∧ s[i-1][j-1] → s[i][j]
            // if x is true AND at least j-1 of first i-1 are true,
            // then at least j of first i are true
            if (j > 1 && i > 1) {
                clause.emplace_back(x,           false);
                clause.emplace_back(s(i-1, j-1), false);
                clause.emplace_back(s(i,   j),   true);
                satSolver->add_clause(clause);
                clause.clear();
            }

            // x → s[i][1]
            // if x is true then at least 1 of first i are true
            if (j == 1) {
                clause.emplace_back(x,       false);
                clause.emplace_back(s(i, 1), true);
                satSolver->add_clause(clause);
                clause.clear();
            }
        }
    }

    // the enforcement clause: s[n][k+1] must be false
    // meaning: it is NOT the case that k+1 or more variables are true
    // this is the actual at-most-k constraint
    clause.emplace_back(s(n, k + 1), false);
    satSolver->add_clause(clause);
    clause.clear();

    // return the first variable id after this counter block
    // so the caller knows where free variables start
    return firstAuxVar + n * (k + 1);
}



void DefectSAT::addBlockingClause(const Solution& sol) {
    // forbid this exact membership assignment for every edge in every matching
    // clause: at least one of the current true variables must flip
    vector<pair<int,bool>> clause;
    vector<Edge> edges = edgeList.getEdgeList();

    for (int i = 0; i < numEdges; i++) {
        Edge e = edges[i];
        // for each matching, negate whatever is currently true
        bool inM1 = sol.M1.contains(e);
        bool inM2 = sol.M2.contains(e);
        bool inM3 = sol.M3.contains(e);

        // if b[i][m] is true in this solution, add ¬b[i][m] to the blocking clause
        // if b[i][m] is false in this solution, add b[i][m] to the blocking clause
        // together this forces at least one variable to differ from current assignment
        clause.emplace_back(var(i, 0), !inM1);
        clause.emplace_back(var(i, 1), !inM2);
        clause.emplace_back(var(i, 2), !inM3);
    }

    satSolver->add_clause(clause);
}

std::vector<Solution> DefectSAT::getAllSolutions() {
    std::vector<Solution> results;

    while (true) {
        satisfied = (satSolver->solve() == SolveResult::SAT);
        if (!satisfied) break;

        Solution sol = extractSolution();
        results.push_back(sol);

        //std::cout << "Found solution " << results.size() << "\n";

        addBlockingClause(sol);
    }

    return results;
}