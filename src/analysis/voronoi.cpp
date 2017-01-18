/*
 * voronoi.cpp
 *
 *  Created on: Mar 30, 2016
 *      Author: aidan
 */

#include "voronoi.hpp"

#include "../core/world.hpp"

namespace
{
    void render_finite_linear_edge(sf::RenderWindow* window, const voronoi_diagram<double>::edge_type& edge, float scale);
    void render_infinite_linear_edge(sf::RenderWindow* window, const voronoi_diagram<double>::edge_type& edge, float scale);
}

VoronoiMap::VoronoiMap()
{
    m_map = new voronoi_diagram<double>();
}

bool VoronoiMap::update_diagram(const World* world)
{
    std::vector<Point> points;
    for (int x = 0; x < world->width(); ++x)
    {
        for (int y = 0; y < world->height(); ++y)
        {
            Node* node = world->at(x,y);
            if (node->is_wall())
            {
                bool success = true;
                for (auto nei : world->neighbours(node))
                {
                    if (!nei->is_wall())
                    {
                        success = false;
                        break;
                    }
                }

                if (success)
                {
                    points.push_back(Point(x,y));
                }
            }
        }
    }

    // Keep this empty for now
    std::vector<Segment> segments;

    construct_voronoi(points.begin(), points.end(), segments.begin(), segments.end(), m_map);

    // Filter edges and vertexes
    m_filtered_edges = m_map->edges();

    auto edge_filter = [world](const voronoi_diagram<double>::edge_type& edge)
    {
        bool remove = false;

        if(edge.is_finite())
        {
            std::pair<int,int> p0 = std::make_pair(std::round(edge.vertex0()->x()), std::round(edge.vertex0()->y()));
            std::pair<int,int> p1 = std::make_pair(std::round(edge.vertex1()->x()), std::round(edge.vertex1()->y()));

            if (world->is_valid(p0) && world->is_valid(p1))
            {
                remove = remove || world->at(p0)->is_wall() || world->at(p1)->is_wall();
            }
            else
            {
                remove = true;
            }
        }
        else
        {
            remove = true;
        }

        return remove;
    };

    m_filtered_edges.erase(std::remove_if(m_filtered_edges.begin(), m_filtered_edges.end(), edge_filter), m_filtered_edges.end());

    return true;
};


void VoronoiMap::render(sf::RenderWindow* window, float scale)
{
    // Iterate over edges
    for (const voronoi_diagram<double>::edge_type& edge : m_filtered_edges)
    {
        // Draw only straight edges
        if (edge.is_linear() && edge.is_finite())
        {
            render_finite_linear_edge(window, edge, scale);
        }
    }
}

namespace
{
void render_finite_linear_edge(sf::RenderWindow* window, const voronoi_diagram<double>::edge_type& edge, float scale)
{
    auto v0 = edge.vertex0();
    auto v1 = edge.vertex1();

    sf::Vertex line[] =
    {
        sf::Vertex(sf::Vector2f(v0->x(), v0->y())*scale, sf::Color::Yellow),
        sf::Vertex(sf::Vector2f(v1->x(), v1->y())*scale, sf::Color::Yellow)
    };

    window->draw(line, 2, sf::Lines);
}

void render_infinite_linear_edge(sf::RenderWindow* window, const voronoi_diagram<double>::edge_type& edge, float scale)
{
    auto v0 = edge.vertex0();
    auto v1 = edge.vertex1();

    sf::Vertex line[] =
    {
        sf::Vertex(sf::Vector2f(v0->x(), v0->y())*scale),
        sf::Vertex(sf::Vector2f(v1->x(), v1->y())*scale)
    };
    window->draw(line, 2, sf::Lines);
}
}

