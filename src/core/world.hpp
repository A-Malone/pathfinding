#ifndef TERRAIN_H
#define TERRAIN_H

#include "../common.hpp"

#include <SFML/Graphics.hpp>

// Forward declarations
class World;
class Unit;

class NodeData
{
    // Base class
};

class Node
{
    friend class World;

public:

    Node(int px, int py) :
            data(nullptr)
    {
        x = px;
        y = py;
    }

    int x;
    int y;
    bool visited = false;

    NodeData* data;

    bool is_wall() const
    {
        return wall && seen;
    };
    bool is_seen() const
    {
        return seen;
    };


protected:
    bool wall = false;
    bool seen = false;
};

class World
{
public:
    World(unsigned int w, unsigned int h, unsigned int sight =
            std::numeric_limits<unsigned int>::max());
    ~World();

    int width() const
    {
        return m_width;
    }
    ;
    int height() const
    {
        return m_height;
    }
    ;

    bool step();

    Unit* spawn(int x, int y);

    bool is_valid(int x, int y) const;

    Node* at(int x, int y) const;
    Node* at(std::pair<int, int> p) const;

    std::vector<Node*> neighbours(Node* node) const;

    void render(sf::RenderWindow& window, int scale);

protected:
    unsigned int m_width;
    unsigned int m_height;

private:
    std::vector<std::vector<Node*>> m_map;

    bool m_full_sight;
    int m_sight;

    std::vector<Unit*> m_units;

    void create_obstacle(int x, int y, float r);
    void update_visible();
};

#endif /* TERRAIN_H */
