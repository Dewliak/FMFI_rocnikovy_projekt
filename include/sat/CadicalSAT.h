//
// Created by dewliak on 10/13/25.
//

#ifndef ROCNIKOVY_PROJEKT_CADICALSAT_H
#define ROCNIKOVY_PROJEKT_CADICALSAT_H


#include "ISAT.h"
#include <cadical.hpp>

#include <vector>
#include <utility>
#include <memory>
class CadicalSAT: public ISAT {
public:
    CadicalSAT();
    CadicalSAT(const CadicalSAT&) = delete;
    CadicalSAT& operator=(const CadicalSAT&) = delete;
    ~CadicalSAT() override = default;
    void add_clause(std::vector<std::pair<int,bool>> literal ) override;
    //virtual add(unsigned var, boolean value);
    SolveResult solve() override;
    bool variable_value(int var) override;
    std::vector<bool> getAnswer();
private:
    std::unique_ptr<CaDiCaL::Solver> solver;
    int max_lit = 0;
};



#endif //ROCNIKOVY_PROJEKT_CADICALSAT_H

