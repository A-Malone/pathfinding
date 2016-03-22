#include "rtaa.hpp"

#include "astar.hpp"

std::vector<Node*> RTAA::get_path
(
	Terrain& terrain,
	std::pair<int,int> p1,
	std::pair<int,int> p2
)
{
	// Get the starting and ending nodes
	Node* start = terrain.at(p1);
	Node* end = terrain.at(p2);

	Node* current = start;

	std::vector<Node*> ret_path;
	ret_path.push_back(current);

	while(current != end)
	{
		AStar a_star;
		std::vector<Node*> path = a_star.get_path(terrain, std::make_pair(current->x, current->y), p2);

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

		int m_movements = 4;

		for (int i = 0; i < std::min(m_movements, static_cast<int>(path.size())); ++i)
		{
			Node* node = path[path.size()-i-1];
			current = node;
			terrain.set_current(current);
			ret_path.push_back(current);

			// Add adjustment stuff here
		}
	}

	return ret_path;
}
