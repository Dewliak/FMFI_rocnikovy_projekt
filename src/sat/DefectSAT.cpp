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
    //encodeChangedVariables(); // only needed if you're using Hamming distance
}




bool DefectSAT::solve() {
    satisfied = (satSolver->solve() == SolveResult::SAT);
    return satisfied;

}
std::vector<int> DefectSAT::getSolution() {

}


int DefectSAT::var(int edge, int color) {
    // utility for encoding node with color
    return edge * numColors + color + 1;
}
int DefectSAT::uncovVar(int i) {
    return numEdges * 3 + i + 1;
}
int DefectSAT::changedVar(int i) {
    return numEdges * 4 + i + 1;
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
    // collect all uncov variables
    vector<int> uncovVars;
    for (int i = 0; i < numEdges; i++)
        uncovVars.push_back(uncovVar(i));

    // first aux var starts after all named vars
    int firstAux = numEdges * 5 + 1;

    // at-most-3: sequential counter
    int nextAux = addAtMostK(uncovVars, 3, firstAux);

    // at-least-3: at-most-(n-3) of the negations
    // equivalently: add a unit clause that the 3rd counter register is true
    // simplest: just add at-most-3 + force sum >= 3 via a dedicated counter
    // The cleanest way: build a second counter for at-least-3.
    // We encode: NOT (at-most-2), i.e., the counter's slot 3 must be set.
    // Since addAtMostK already enforces ≤3, we add one more clause:
    // the sequential counter's s[n][3] variable must be true.
    // We track that variable here:
    int n = uncovVars.size();
    // s[n][3] is the variable that says "at least 3 of uncovVars are true"
    // In our addAtMostK layout: s[i][j] = firstAux + (i-1)*(k+1) + (j-1)
    // where k=3, so s[n][3] = firstAux + (n-1)*4 + 2
    int atLeastThreeVar = firstAux + (n - 1) * 4 + 2;

    vector<pair<int,bool>> clause;
    clause.emplace_back(atLeastThreeVar, true);
    satSolver->add_clause(clause);
}

// ── changed variables ────────────────────────────────────────────────────────

void DefectSAT::encodeChangedVariables() {
    // changed(i) ↔ ¬b[i][baseline(i)]
    // only for G' edges that appear in the baseline
    vector<pair<int,bool>> clause;
    for (int i = 0; i < numEdges; i++) {
        if (!colorBaseline.count(i)) continue; // u,v edges — no baseline, skip

        int bColor = colorBaseline.at(i);
        int cv = changedVar(i);
        int bv = var(i, bColor);

        // changed(i) ↔ ¬b[i][bColor]
        // (1) ¬changed(i) ∨ ¬b[i][bColor]
        clause.emplace_back(cv, false);
        clause.emplace_back(bv, false);
        satSolver->add_clause(clause);
        clause.clear();
        // (2) changed(i) ∨ b[i][bColor]
        clause.emplace_back(cv, true);
        clause.emplace_back(bv, true);
        satSolver->add_clause(clause);
        clause.clear();
    }
}

// ── iterative deepening entry point ─────────────────────────────────────────

bool DefectSAT::solveAtDistance(int k) {
    // collect changed vars for G' edges only
    vector<int> changedVars;
    for (int i = 0; i < numEdges; i++)
        if (colorBaseline.count(i))
            changedVars.push_back(changedVar(i));

    // aux vars start after changed vars block
    int firstAux = numEdges * 5 + numEdges + 1;
    addAtMostK(changedVars, k, firstAux);

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
    // s[i][j]: "among first i vars, at least j are true"  (1-indexed both)
    // laid out as: s[i][j] = firstAuxVar + (i-1)*(k+1) + (j-1)
    auto s = [&](int i, int j) {
        return firstAuxVar + (i - 1) * (k + 1) + (j - 1);
    };

    vector<pair<int,bool>> clause;

    for (int i = 1; i <= n; i++) {
        int x = vars[i - 1];
        for (int j = 1; j <= k + 1; j++) {

            // s[i][j] → s[i-1][j]  (counter is monotone)
            if (i > 1) {
                clause.emplace_back(s(i, j),     false);
                clause.emplace_back(s(i - 1, j), true);
                satSolver->add_clause(clause);
                clause.clear();
            }

            // x ∧ s[i-1][j-1] → s[i][j]
            if (j > 1 && i > 1) {
                clause.emplace_back(x,                false);
                clause.emplace_back(s(i - 1, j - 1), false);
                clause.emplace_back(s(i, j),          true);
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

    // the core at-most-k clause: s[n][k+1] must be false
    clause.emplace_back(s(n, k + 1), false);
    satSolver->add_clause(clause);

    return firstAuxVar + n * (k + 1); // next free variable after this block
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