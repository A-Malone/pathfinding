#include "astar.hpp"
#include "../pathfinding/rtaa.hpp"

#include "../pathfinding/astar.hpp"

std::vector<MapNode*> RTAA::get_path
(
    Map* terrain,
    MapNode* start,
    MapNode* end
)
{
    AStar a_star;
    std::vector<MapNode*> path = a_star.get_path(terrain, start, end);

    // Failure condition, no path
    if (path.size() == 0)
    {
        return path;
    }

    // Update the closed path
    for (MapNode* node : a_star.m_closed_set)
    {
        // Update
        AStarNodeData* data = node->get_data<AStarNodeData>();
        data->h_score = end->get_data<AStarNodeData>()->g_score \
                + a_star.heuristic(node, end) \
                - data->g_score;
    }

    return path;
}
