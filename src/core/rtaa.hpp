#ifndef SRC_CORE_RTAA_HPP_
#define SRC_CORE_RTAA_HPP_

#include "solver.hpp"

class RTAA : Solver
{
public:
    std::vector<Node*> get_path
    (
        Terrain& terrain,
        std::pair<int,int> p1,
        std::pair<int,int> p2
    );
};

#endif /* SRC_CORE_RTAA_HPP_ */
