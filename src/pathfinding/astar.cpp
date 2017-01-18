#include "../pathfinding/astar.hpp"

// ANONYMOUS NAMESPACE

namespace
{
float path_length(MapNode* start, MapNode* end);
}

// CLASS FUNCTIONS

std::vector<MapNode*> AStar::get_path
(
    Map* map,
    MapNode* start,
    MapNode* end
)
{
    start->set_data<AStarNodeData>(new AStarNodeData(0, heuristic(start,end)));
    m_open_set.insert(start);
    m_open_queue.push(start);

    AStarNodeData* s_data = start->get_data<AStarNodeData>();

    std::unordered_map<MapNode*,MapNode*> came_from;

    // The return path
    std::vector<MapNode*> path;

    while (m_open_queue.size() > 0)
    {
        // Get the first item in the min-heap
        MapNode* current = m_open_queue.top();
        m_open_set.erase(current);
        m_open_queue.pop();

        AStarNodeData* c_data = current->get_data<AStarNodeData>();

        // Short-circuit for the end
        if (!current->is_seen() && c_data->f_score() < s_data->f_score())
        {
            int dist = path_length(current, end);
            if (AStarNodeData* data = end->get_data<AStarNodeData>())
            {
                data->g_score = data->g_score + dist;
                data->h_score = 0;
            }
            else
            {
                end->set_data<AStarNodeData>(
                    new AStarNodeData(c_data->g_score + dist, 0)
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
        for (MapNode* neighbor : map->neighbours(current))
        {
            bool is_in_open_set = m_open_set.find(neighbor) != m_open_set.end();
            bool is_in_closed_set = m_closed_set.find(neighbor) != m_closed_set.end();
            
            // If it's in the closed set skip
            if (is_in_closed_set)
            {
                continue;       // Ignore the neighbor which is already evaluated.
            }

            AStarNodeData* n_data = neighbor->get_data<AStarNodeData>();

            // The distance from start to goal passing through current and the neighbor.
            int tentative_g_score = c_data->g_score + path_length(current,neighbor);
            if (is_in_open_set && tentative_g_score >= n_data->g_score)
            {
                continue;       // This is not a better path.
            }

            // This path is the best until now. Record it!
            came_from[neighbor] = current;

            if (n_data)
            {
                n_data->g_score = tentative_g_score;
                n_data->h_score = heuristic(neighbor, end);
            }
            else
            {
                neighbor->set_data<AStarNodeData>(
                        new AStarNodeData(
                            tentative_g_score,
                            heuristic(neighbor, end)
                        )
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
    MapNode* a,
    MapNode* b
)
{
    return sqrt(std::pow(a->x() - b->x(), 2) + std::pow(a->y() - b->y(), 2));
}

std::vector<MapNode*> AStar::reconstruct_path
(
    const std::unordered_map<MapNode*,MapNode*>& came_from,
    MapNode* end
)
{
    MapNode* current = end;
    std::vector<MapNode*> path;

    while (came_from.find(current) != came_from.end())
    {
        path.push_back(current);
        current = came_from.at(current);
    }
    path.push_back(current);

    return path;
}


namespace
{
float path_length(MapNode* start, MapNode* end)
{
    int dx = std::abs(end->x() - start->x());
    int dy = std::abs(end->y() - start->y());

    return std::min(dx,dy) * std::sqrt(2) + std::abs(dx-dy);
}
}
