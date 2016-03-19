#include "terrain.hpp"

Terrain::Terrain(int w, int h)
{
    m_width = w;
    m_height = h;

    for (int x = 0; x < w; ++x)
    {
        std::vector<Node*> col;
        col.reserve(h);
        for (int y = 0; y < h; ++y)
        {
            col.push_back(new Node(x,y));
        }
        m_map.push_back(col);
    }
}

Terrain::~Terrain()
{
    m_map.clear();
}

Node* Terrain::at(int x, int y) const
{    
    return m_map.at(x).at(y);
}

Node* Terrain::at(std::pair<int,int> p) const
{
    return m_map.at(p.first).at(p.second);
}

std::vector<Node*> Terrain::neighbours(Node* node) const
{
    std::vector<Node*> nodes;
    if (node->x > 0)
    {
        nodes.push_back(at(node->x-1,node->y));
    }
    if (node->x < m_width - 1)
    {
        nodes.push_back(at(node->x+1,node->y));
    }

    if (node->y > 0)
    {
        nodes.push_back(at(node->x,node->y-1));
    }
    if (node->y < m_height - 1)
    {
        nodes.push_back(at(node->x,node->y+1));
    }

    return nodes;
}
