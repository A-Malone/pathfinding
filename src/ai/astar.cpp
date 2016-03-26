#include "astar.hpp"

std::vector<Node*> AStar::get_path
(
    Map* terrain,
    MapNode* start,
    MapNode* end
)
{
    AStarNode* a_start = new AStarNode(start, 0, heuristic(start,end));
    m_open_set.insert(a_start);
    m_open_queue.push(a_start);

    std::unordered_map<AStarNode*,AStarNode*> came_from;

    // The return path
    std::vector<Node*> path;

    while (m_open_queue.size() > 0)
    {
        // Get the first item in the min-heap
        AStarNode* current = m_open_queue.top();
        m_open_set.erase(current);
        m_open_queue.pop();

        // Short-circuit for the end
        if (!current->m_node->is_seen())
        {
            int dist = std::abs(current->x() - end->x) + std::abs(current->y() - end->y);
            if (AStarNode* data = static_cast<AStarNodeData*>(end->data))
            {
                data->g_score = static_cast<AStarNodeData*>(current->data)->g_score + dist;
                data->h_score = 0;
            }
            else
            {
                end->data = new AStarNodeData(
                        static_cast<AStarNodeData*>(current->data)->g_score + dist,
                        0
                );
            }
            path = reconstruct_path(came_from, current);
            break;
        }
        if (current == end)
        {
            path = reconstruct_path(came_from, end);
            break;
        }

        m_closed_set.insert(current);
        for (Node* neighbor : terrain.neighbours(current))
        {
            bool is_in_open_set = m_open_set.find(neighbor) != m_open_set.end();
            bool is_in_closed_set = m_closed_set.find(neighbor) != m_closed_set.end();
            
            // If it's in the closed set skip
            if (is_in_closed_set)
            {
                continue;       // Ignore the neighbor which is already evaluated.
            }

            // The distance from start to goal passing through current and the neighbor.
            int tentative_g_score = static_cast<AStarNodeData*>(current->data)->g_score + 1;
            if (is_in_open_set && tentative_g_score >= static_cast<AStarNodeData*>(neighbor->data)->g_score)
            {
                continue;       // This is not a better path.
            }

            // This path is the best until now. Record it!
            came_from[neighbor] = current;

            if (AStarNodeData* data = static_cast<AStarNodeData*>(neighbor->data))
            {
                data->g_score = tentative_g_score;
                data->h_score = heuristic(neighbor, end);
            }
            else
            {
                neighbor->data = new AStarNodeData(
                        tentative_g_score,
                        heuristic(neighbor, end)
                );
            }

            if (!is_in_open_set)
            {
                m_open_set.insert(neighbor);
                m_open_queue.push(neighbor);
            }
        }
    }

    return path;
}

float AStar::heuristic
(
    Node* a,
    Node* b
)
{
    return sqrt(std::pow(a->x - b->x, 2) + std::pow(a->y - b->y, 2));
}

std::vector<Node*> AStar::reconstruct_path
(
    const std::unordered_map<Node*,Node*>& came_from,
    Node* end
)
{
    Node* current = end;
    std::vector<Node*> path;

    while (came_from.find(current) != came_from.end())
    {
        path.push_back(current);
        current = came_from.at(current);
    }
    path.push_back(current);

    return path;
}
