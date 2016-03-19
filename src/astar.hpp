#include "common.hpp"

#include "solver.hpp"

class AStar : Solver
{
public:
    std::vector<Node*> get_path
    (
        const Terrain& terrain,
        std::pair<int,int> p1,
        std::pair<int,int> p2
    );

protected:
    int heuristic
    (
        std::pair<int,int> p1,
        std::pair<int,int> p2
    );

    std::vector<Node*> reconstruct_path
    (
        std::unordered_map<Node*,Node*> came_from,
        Node* end
    );
};
