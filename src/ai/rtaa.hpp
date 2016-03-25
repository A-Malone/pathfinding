#ifndef SRC_CORE_RTAA_HPP_
#define SRC_CORE_RTAA_HPP_

#include "../core/solver.hpp"

class RTAA : Solver
{
public:
    std::vector<Node*> get_path
    (
        World& terrain,
        Node* start,
        Node* end
    );
};

#endif /* SRC_CORE_RTAA_HPP_ */
