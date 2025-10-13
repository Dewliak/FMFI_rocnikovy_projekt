//
// Created by dewliak on 10/13/25.
//

#ifndef ROCNIKOVY_PROJEKT_SATINTERFACE_H
#define ROCNIKOVY_PROJEKT_SATINTERFACE_H

#include <vector>
#include <utility>

enum class SolveResult {SAT, UNSAT, UNKNOWN};

class SATInterface {
    public:
    virtual ~SATInterface() = default;


    virtual void add_clause(std::vector<std::pair<int,bool>> literal );
    //virtual add(unsigned var, boolean value);
    virtual SolveResult solve();
    virtual bool variable_value(int var);

    private:
};

#endif //ROCNIKOVY_PROJEKT_SATINTERFACE_H