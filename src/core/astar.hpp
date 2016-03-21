#include "../common.hpp"

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
    float heuristic
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

class AStarNodeData : public NodeData
{
public:
	AStarNodeData(float f, int g) : f_score(f), g_score(g) {};


	float f_score;

	// For each node, the cost of getting from the start node to that node.
	int g_score;
};

struct node_cmp
{
    bool operator()(Node* a, Node* b)
    {
    	return static_cast<AStarNodeData*>(a->data)->f_score > static_cast<AStarNodeData*>(b->data)->f_score;
    }
};
