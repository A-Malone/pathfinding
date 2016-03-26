#include "rtaa.hpp"

#include "astar.hpp"

std::vector<Node*> RTAA::get_path
(
    Map* terrain,
    MapNode* start,
    MapNode* end
)
{
    AStar a_star;
    std::vector<Node*> path = a_star.get_path(terrain, start, end);

    // Failure condition, no path
    if (path.size() == 0)
    {
        return path;
    }

    // Update the closed path
    for (Node* node : a_star.m_closed_set)
    {
        // Update
        AStarNodeData* data = static_cast<AStarNodeData*>(node->data);
        data->h_score = static_cast<AStarNodeData*>(end->data)->g_score \
                + a_star.heuristic(node, end) \
                - data->g_score;
    }

    return path;
}
