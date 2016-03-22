#ifndef ASTAR_H
#define ASTAR_H

#include "../common.hpp"

#include "solver.hpp"
#include "priority_queue.hpp"


class AStarNodeData : public NodeData
{
public:
	AStarNodeData(float g, int h) : g_score(g), h_score(h) {};

	float f_score() {return g_score + h_score;};

	int g_score;	// Real cost of getting from the start node to that node.
	float h_score;	// Heuristic cost of getting from the start node to that node.
};

struct node_cmp
{
    bool operator()(Node* a, Node* b)
    {
    	return static_cast<AStarNodeData*>(a->data)->f_score() > static_cast<AStarNodeData*>(b->data)->f_score();
    }
};

class AStar : Solver
{
public:
    std::vector<Node*> get_path
    (
        Terrain& terrain,
        std::pair<int,int> p1,
        std::pair<int,int> p2
    );

    std::unordered_set<Node*> m_closed_set;
	PriorityQueue<Node*, node_cmp> m_open_set;

    float heuristic
    (
        Node* a,
		Node* b
    );

    std::vector<Node*> reconstruct_path
    (
        std::unordered_map<Node*,Node*> came_from,
        Node* end
    );
};

#endif
