/*
 * map.hpp
 *
 *  Created on: Mar 26, 2016
 *      Author: aidan
 */

#ifndef SRC_CORE_MAP_HPP_
#define SRC_CORE_MAP_HPP_

#include "../common.hpp"

#include "world.hpp"

class NodeData
{
    // Base class
};

class MapNode
{
    friend class Map;

public:
    MapNode(Node* node) : m_node(node), m_data(nullptr) {};

    int x() const {return m_node->x;};
    int y() const {return m_node->y;};

    template <class T>
    T* get_data() const {return static_cast<T*>(m_data);};

    template <class T>
    void set_data(T* data) {m_data = static_cast<NodeData*>(data);};

    bool is_seen() const {return m_node->is_seen();};

protected:
    NodeData* m_data;
    Node* m_node;
};

class Map
{
public:
    Map(World* world);

    // Map dimensions
    bool is_valid(const std::pair<int,int> pos) const {return m_world->is_valid(pos.first, pos.second);};
    int width() const {return m_world->width();};
    int height() const {return m_world->height();};

    // Node access
    MapNode* at(const std::pair<int,int>& pos);
    std::vector<MapNode*> neighbours(MapNode* node);

    std::vector<Node*> to_world_path(const std::vector<MapNode*> path) const;

private:
    World* m_world;

    // Sparse map TODO: Define hash function for unordered map
    std::map<std::pair<int,int>, MapNode*> m_map;
};

#endif /* SRC_CORE_MAP_HPP_ */
