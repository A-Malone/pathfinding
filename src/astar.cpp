#include "astar.hpp"

#include "priority_queue.hpp"


std::vector<Node*> AStar::get_path
(
    const Terrain& terrain,
    std::pair<int,int> p1,
    std::pair<int,int> p2
)
{
    // Get the starting and ending nodes
    Node* start = terrain.at(p1);
    Node* end = terrain.at(p2);

    // For each node, the cost of getting from the start node to that node.
    std::unordered_map<Node*, int> g_score;
    g_score[start] = 0;

    // For each node, the total cost of getting from the start node to the goal
    // by passing by that node. That value is partly known, partly heuristic.
    CompareMap<Node*, int, std::less<int>> f_score;
    f_score[start] = heuristic(p1,p2);

    // Setup the open and closed sets
    std::unordered_set<Node*> closed_set;

    // Create the backing container for heap
    PriorityQueue<Node*, CompareMap<Node*, int, std::less<int>>> open_set(f_score);
    open_set.push(start);

    std::unordered_map<Node*,Node*> came_from;

    // The return path
    std::vector<Node*> path;

    while (open_set.size() > 0)
    {
        // Get the first item in the min-heap        
        Node* current = open_set.top();
        open_set.pop();

        std::cout << "Current : (" << current->x << "," << current->y << ")" << std::endl;
        
        if (current == end)
        {
            path = reconstruct_path(came_from, end);
        }

        closed_set.insert(current);
        for (Node* neighbor : terrain.neighbours(current))
        {
            std::cout << "Neighbour : (" << neighbor->x << "," << neighbor->y << ")" << std::endl;

            bool is_in_open_set = open_set.find(neighbor) != open_set.end();
            bool is_in_closed_set = closed_set.find(neighbor) != closed_set.end();
            
            std::cout << "in_open:" << is_in_open_set << " in_closed:" << is_in_closed_set << std::endl;

            // If it's in the closed set skip
            if (is_in_closed_set)
            {
                continue;       // Ignore the neighbor which is already evaluated.
            }

            // The distance from start to goal passing through current and the neighbor.
            int tentative_g_score = g_score[current] + 1;
            if (is_in_open_set && tentative_g_score >= g_score[neighbor])
            {
                continue;       // This is not a better path.
            }

            // This path is the best until now. Record it!
            came_from[neighbor] = current;
            g_score[neighbor] = tentative_g_score;
            f_score[neighbor] = g_score[neighbor] + heuristic(std::make_pair(neighbor->x, neighbor->y), std::make_pair(end->x, end->y));
            
            if (!is_in_open_set)
            {
                open_set.push(neighbor);
            }
        }
    }
    return path;
}

int AStar::heuristic
(
    std::pair<int,int> p1,
    std::pair<int,int> p2
)
{
    return sqrt(std::pow(p2.first - p1.first, 2) + std::pow(p2.second - p1.second, 2));
}

std::vector<Node*> AStar::reconstruct_path
(
    std::unordered_map<Node*,Node*> came_from,
    Node* end
)
{
    Node* current = end;
    std::vector<Node*> path;

    while (came_from.find(current) == came_from.end())
    {
        path.push_back(current);
        current = came_from[current];
    }
    path.push_back(current);

    return path;
}
