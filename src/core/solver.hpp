#ifndef SOLVER_H
#define SOLVER_H

#include "../common.hpp"

#include "../core/world.hpp"

class Solver
{
public:
    virtual std::vector<Node*> get_path
    (
        World& terrain,
        Node* start,
        Node* end
    ) = 0;
};

#endif
