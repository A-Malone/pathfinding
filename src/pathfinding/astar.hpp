#ifndef ASTAR_H
#define ASTAR_H

#include "../common.hpp"
#include "../core/solver.hpp"
#include "../core/map.hpp"

#include "../pathfinding/priority_queue.hpp"


class AStarNodeData : public NodeData
{
public:
    AStarNodeData(float g, float h) : g_score(g), h_score(h) {};

    float f_score() {return g_score + h_score;};

    float g_score;    // Real cost of getting from the start node to that node.
    float h_score;    // Heuristic cost of getting from the start node to that node.
};

struct node_cmp
{
    bool operator()(MapNode* a, MapNode* b)
    {
        return a->get_data<AStarNodeData>()->f_score() > b->get_data<AStarNodeData>()->f_score();
    }
};

class AStar : Solver
{
public:
    std::vector<MapNode*> get_path
    (
        Map* terrain,
        MapNode* start,
        MapNode* end
    );

    std::unordered_set<MapNode*> m_closed_set;

    PriorityQueue<MapNode*, node_cmp> m_open_queue;
    std::unordered_set<MapNode*> m_open_set;


    float heuristic
    (
        MapNode* a,
        MapNode* b
    );

    std::vector<MapNode*> reconstruct_path
    (
        const std::unordered_map<MapNode*,MapNode*>& came_from,
        MapNode* end
    );
};

#endif
