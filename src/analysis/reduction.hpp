#ifndef ANALYZER_REDUCTION_H
#define ANALYZER_REDUCTION_H

#include "../common.hpp"

// FORWARD DECLARATIONS

class ReductionNode;
class ReductionEdge;

class World;
class Node;

// CLASS DEFINITIONS

class ReductionNode
{
public:
    ReductionNode(Node* node) : m_node(node) {};

    const Node* get_node() {return m_node;};
    point_t position() const {return point_t(m_node->x(),m_node->y());};

    virtual std::vector<const ReductionNode*> neighbours() const = 0;

protected:
    const Node* m_node;
};

class ReductionEdge
{
public:
    ReductionEdge(ReductionNode* a, ReductionNode* b, float cost) : m_a(a), m_b(b), m_cost(cost) {}

    float cost() { return m_cost; };
    std::pair<ReductionNode*,ReductionNode*> vertices() {return std::make_pair(m_a,m_b);}

private:
    ReductionNode* m_a;
    ReductionNode* m_b;

    float m_cost;
};


class Reduction
{
public:
    virtual void update(const World* world) = 0;
    virtual void render(sf::RenderWindow* window, float scale = 1.0) = 0;
};

#endif  // ANALYZER_REDUCTION_H
