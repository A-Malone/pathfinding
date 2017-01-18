/*
 * RSR.hpp
 *
 *  Created on: Apr 10, 2016
 *      Author: aidan
 */

#ifndef SRC_ANALYSIS_RSR_HPP_
#define SRC_ANALYSIS_RSR_HPP_

#include "../common.hpp"

#include <SFML/Graphics.hpp>

#include "reduction.hpp"

class ReductionRect
{
public:
    ReductionRect(const box_t& box) : m_rect(box) {};

    int area() const {return bg::area(m_rect);}

    point_t top_left() const {return m_rect.min_corner();}
    point_t bottom_right() const {return m_rect.max_corner();}

    point_t get_size() const
    {
        return point_t(
            bg::get<bg::max_corner,0>(m_rect) - bg::get<bg::min_corner,0>(m_rect),
            bg::get<bg::max_corner,1>(m_rect) - bg::get<bg::min_corner,1>(m_rect)
        );
    }

private:
    box_t m_rect;
    std::vector<ReductionNode> m_edge_cells;
};

class RSR : Reduction
{
public:
    RSR();
    virtual ~RSR();

    void update(const World* world);
    void render(sf::RenderWindow* window, float scale = 1.0);

private:
    std::vector<ReductionRect> m_rects;
};

#endif /* SRC_ANALYSIS_RSR_HPP_ */
