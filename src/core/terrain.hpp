#ifndef TERRAIN_H
#define TERRAIN_H

#include "../common.hpp"

// Forward declarations
class Terrain;

class NodeData
{
	// Base class
};

class Node
{
	friend class Terrain;

public:

    Node(int px, int py)
    : data(nullptr)
    {
        x = px;
        y = py;
    }

    int x;
	int y;
	bool visited = false;

	NodeData* data;

	bool is_wall() const {return wall && seen;};
	bool is_seen() const {return seen;};

protected:
    bool wall = false;
	bool seen = false;
};

class Terrain
{
public:
    Terrain(unsigned int w, unsigned int h, unsigned int sight = std::numeric_limits<unsigned int>::max());
    ~Terrain();

    int width() const {return m_width;};
    int height() const {return m_height;};

    bool is_valid(int x, int y) const;

    Node* at(int x, int y) const;
    Node* at(std::pair<int,int> p) const;

    void set_current(Node* n);

    std::vector<Node*> neighbours(Node* node) const;


private:
    unsigned int m_width;
    unsigned int m_height;

    bool m_full_sight;
    unsigned int m_sight;

    std::vector<std::vector<Node*>> m_map;

    Node* m_current;

    void create_obstacle(int x, int y, float r);
    void update_visible();
};

#endif /* TERRAIN_H */
