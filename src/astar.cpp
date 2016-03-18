#include "astar.hpp"

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
    std::unordered_map<Node*, int> f_score;
    f_score[start] = heuristic(p1,p2);

    // Setup the open and closed sets
    std::unordered_set<Node*> closed_set;

    // Create the backing container for heap
    auto node_comp = [&](Node* left, Node* right) { return f_score[left] > f_score[right];};
    std::vector<Node*> open_set;

    open_set.push_back(start);
    std::make_heap (open_set.begin(),open_set.end(), node_comp);

    std::unordered_map<Node*,Node*> came_from;

    // The return path
    std::vector<Node*> path;

    while (open_set.size() > 0)
    {
        // Get the first item in the min-heap
        Node* current = open_set.front();
        std::pop_heap(open_set.begin(), open_set.end(), node_comp);
        open_set.pop_back();

        if(current == end)
        {
            path = reconstruct_path(came_from, end);
        }

        closed_set.insert(current);
        for (Node* neighbor : terrain.neighbours(current))
        {
            if (closed_set.find(neighbor) != closed_set.end())
            {
                continue;		// Ignore the neighbor which is already evaluated.
            }
            // The distance from start to goal passing through current and the neighbor.
            int tentative_g_score = g_score[current] + 1;

            if (std::find(open_set.begin(), open_set.end(), neighbor) == open_set.end())
            {
                open_set.push_back(neighbor);
                std::push_heap(open_set.begin(),open_set.end(), node_comp);
            }
            else if (tentative_g_score >= g_score[neighbor])
            {
                continue;		// This is not a better path.
            }

            // This path is the best until now. Record it!
            came_from[neighbor] = current;
            g_score[neighbor] = tentative_g_score;
            f_score[neighbor] = g_score[neighbor] + heuristic(std::make_pair(neighbor->x, neighbor->y), std::make_pair(end->x, end->y));
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
    return std::sqrt(std::pow(p2.first - p1.first, 2) + std::pow(p2.second - p1.second, 2));
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
