#ifndef JPS_H
#define JPS_H

#include "astar.hpp"

class JPS : AStar
{
public:
    std::vector<MapNode*> get_path
    (
        Map* terrain,
        MapNode* start,
        MapNode* end
    ) override;
};

#endif
