/*
 * map.cpp
 *
 *  Created on: Mar 26, 2016
 *      Author: aidan
 */

#include "../core/map.hpp"

Map::Map(World* world)
{

}

// Node access
MapNode* at(const std::pair<int,int>& pos);
std::vector<Node*> neighbours(MapNode* node) const;

