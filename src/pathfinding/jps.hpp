#ifndef JPS_H
#define JPS_H

#include "../core/solver.hpp"

class JPS : Solver
{
public:
    std::vector<MapNode*> get_path
    (
        Map* terrain,
        MapNode* start,
        MapNode* end
    );
};

#endif
