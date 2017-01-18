#include "jps.hpp"

namespace
{
float path_length(MapNode* start, MapNode* end);

// JPS specific functions
std::vector<MapNode*> get_successors(Map* map, MapNode* node, MapNode* goal);
MapNode* jump(Map* map, MapNode* node, point_t vec, MapNode* goal);
bool is_exp_forced(Map* map, MapNode* node, point_t vec);
}

// CLASS FUNCTIONS
std::vector<MapNode*> JPS::get_path
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
        for (MapNode* neighbor : get_successors(map, current, end))
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

namespace
{
float path_length(MapNode* start, MapNode* end)
{
    int dx = std::abs(end->x() - start->x());
    int dy = std::abs(end->y() - start->y());

    return std::min(dx,dy) * std::sqrt(2) + std::abs(dx-dy);
}

std::vector<MapNode*> get_successors(Map* map, MapNode* node, MapNode* goal)
{
    std::vector<MapNode*> successors;
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            if(x != 0 || y != 0)
            {
                point_t dir = point_t(x,y);               
                MapNode* successor = jump(map, node, dir, goal);
                if (successor)
                {
                    successors.push_back(successor);
                }                
            }
        }
    }
    return successors;
}

MapNode* jump(Map* map, MapNode* x, point_t vec, MapNode* goal)
{
    
    point_t next_point = node->point() + vec;
    if(!map->is_valid(next_point))
    {
        return nullptr;
    }

    MapNode* node = map->at(next_point);

    // Check if we've encountered a wall
    if(node->is_wall())
    {
        return nullptr;
    }
    
    // Check to see if this node should be a jump point based on whether it's
    // the goal or an unseen point 
    if(node == goal || !node->is_seen())
    {
        return node;
    }

    // Check to see if this node's expansion is forced
    if(is_exp_forced(map, node, vec))
    {
        return node;
    }

    // Rectilinear expansion for diagonal jumps
    if(vec.get<0>() != 0 && vec.get<1>() != 0)
    {
        point_t x_comp = point_t(vec.get<0>(), 0);
        point_t y_comp = point_t(0, vec.get<1>());

        MapNode* x_node = jump(map, node, x_comp, goal);
        if (x_node)
        {
            return x_node;
        }
        
        MapNode* y_node = jump(map, node, y_comp, goal);
        if (y_node)
        {
            return y_node;
        }
    }

    return jump(map, node, vec, goal);
}

bool is_exp_forced(Map* map, MapNode* node, point_t vec)
{
    bool forced = false;

    // Rectilinear expansion
    if(vec.get<0>() == 0 || vec.get<1>() == 0)
    {
        // Check nodes perpendicular to motion for walls
        point_t inverse = point_t(vec.get<1>(), vec.get<0>());
        point_t p;

        p = node->point() + inverse;
        forced = forced || (map->is_valid(p) && map->at(p)->is_wall());

        p = node->point() - inverse;
        forced = forced || (map->is_valid(p) && map->at(p)->is_wall());
    }
    else    // Diagonal expansion
    {
        // Check inverse components of motion for walls
        point_t p;

        p = node->point() - point_t(0, vec.get<1>());
        forced = forced || (map->is_valid(p) && map->at(p)->is_wall());

        p = node->point() - point_t(vec.get<0>(), 0);
        forced = forced || (map->is_valid(p) && map->at(p)->is_wall());
    }

    return forced;
}

}

