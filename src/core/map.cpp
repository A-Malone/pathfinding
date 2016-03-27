#include "map.hpp"

Map::Map(World* world)
    :m_world(world)
{

}

// Node access
MapNode* Map::at(const std::pair<int,int>& pos)
{
    MapNode* map_node;

    auto node_it = m_map.find(pos);
    if (node_it != m_map.end())
    {
        map_node = node_it->second;
    }
    else
    {
        map_node = new MapNode(m_world->at(pos));
        m_map.insert(std::make_pair(pos, map_node));
    }

    return map_node;
}

std::vector<MapNode*> Map::neighbours(MapNode* map_node)
{
    std::vector<MapNode*> map_nodes;

    for (Node* node : m_world->neighbours(map_node->m_node))
    {
        const std::pair<int,int>& pos = node->pos();
        MapNode* nb = this->at(pos);
        map_nodes.push_back(nb);
    }
    return map_nodes;
}

std::vector<Node*> Map::to_world_path(const std::vector<MapNode*> path) const
{
    std::vector<Node*> world_path;
    world_path.reserve(path.size());

    for (MapNode* node : path)
    {
        world_path.push_back(node->m_node);
    }
    return world_path;
}

