#ifndef SOLVER_H
#define SOLVER_H

#include "../common.hpp"

#include "../core/map.hpp"

class Solver
{
public:
    virtual std::vector<MapNode*> get_path
    (
        Map* terrain,
        MapNode* start,
        MapNode* end
    ) = 0;
};

#endif
