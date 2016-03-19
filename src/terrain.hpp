#ifndef TERRAIN_H
#define TERRAIN_H

#include "common.hpp"

struct Node
{
    int x;
    int y;
    bool wall = false;

    Node(int px, int py)
    {
        x = px;
        y = py;
    }
};

class Terrain
{
public:
    Terrain(int w, int h);
    ~Terrain();

    int width() const {return m_width;};
    int height() const {return m_height;};

    Node* at(int x, int y) const;
    Node* at(std::pair<int,int> p) const;

    std::vector<Node*> neighbours(Node* node) const;


private:
    int m_width;
    int m_height;

    std::vector<std::vector<Node*>> m_map;
};

#endif /* TERRAIN_H */
