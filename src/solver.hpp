#include "common.hpp"
#include "terrain.hpp"

class Solver
{
public:
    virtual std::vector<Node*> get_path
    (
        const Terrain& terrain,
        std::pair<int,int> p1,
        std::pair<int,int> p2
    ) = 0;
};
