#ifndef SOLVER_H
#define SOLVER_H

#include "../common.hpp"

#include "terrain.hpp"

class Solver
{
public:
    virtual std::vector<Node*> get_path
    (
        Terrain& terrain,
        std::pair<int,int> p1,
        std::pair<int,int> p2
    ) = 0;
};

#endif
