#ifndef SRC_CORE_RTAA_HPP_
#define SRC_CORE_RTAA_HPP_

#include "../core/solver.hpp"

class RTAA : Solver
{
public:
    std::vector<MapNode*> get_path
    (
        Map* terrain,
        MapNode* start,
        MapNode* end
    );
};

#endif /* SRC_CORE_RTAA_HPP_ */
