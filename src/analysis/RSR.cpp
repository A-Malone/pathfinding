/*
 * RSR.cpp
 *
 *  Created on: Apr 10, 2016
 *      Author: aidan
 */

#include "RSR.hpp"

#include "../core/world.hpp"
#include "../pathfinding/priority_queue.hpp"


namespace
{
    std::vector<box_t> get_rects(std::vector<std::vector<bool>>& map);
    box_t largest_rect_for_point(const point_t& dim, const point_t& point, const std::vector<int>& cache);
    void update_cache(const std::vector<bool>& col, std::vector<int>& cache);
}

struct box_comp
{
    bool operator()(box_t a, box_t b)
    {
        return bg::area(a) < bg::area(b);
    }
};

RSR::RSR()
{
    // TODO Auto-generated constructor stub

}

RSR::~RSR()
{
    // TODO Auto-generated destructor stub
}

void RSR::update(const World* world)
{
    // STEP 0: CREATE MAP
    std::vector<std::vector<bool>> map;

    for (int x = 0; x < world->width(); ++x)
    {
        std::vector<bool> col;
        col.reserve(world->height());

        for (int y = 0; y < world->height(); ++y)
        {
            col.push_back(!world->at(x,y)->is_wall());
        }

        map.push_back(col);
    }

    // STEP 1: REDUCE INTO RECTANGLES
    std::vector<box_t> boxes = get_rects(map);

    m_rects.clear();
    for (const box_t& box : boxes)
    {
        m_rects.push_back(ReductionRect(box));
    }

    // STEP 2: CREATE MACRO EDGES
    // TODO

}

void RSR::render(sf::RenderWindow* window, float scale)
{
    for (const ReductionRect& rect : m_rects)
    {
        point_t rect_size = rect.get_size();

        sf::RectangleShape draw_box(sf::Vector2f(rect_size.get<0>(), rect_size.get<1>()) * scale);
        draw_box.setPosition(rect.top_left().get<0>() * scale, rect.top_left().get<1>() * scale);

        draw_box.setOutlineThickness(scale);
        draw_box.setFillColor(sf::Color::Transparent);
        draw_box.setOutlineColor(sf::Color::Blue);

        window->draw(draw_box);
    }
}


namespace
{

std::vector<box_t> get_rects(std::vector<std::vector<bool>>& map)
{
    PriorityQueue<box_t,box_comp> box_queue;

    int width = map.size();
    int height = map[0].size();
    point_t dim = point_t(width, height);

    std::vector<int> cache;             // Cache

    cache.clear();
    cache.resize(height,0);

    // Step 1 : Enqueue the largest rect for each square
    for (int x = width - 1; x >= 0; --x)
    {
        // Cache a column, aggregating the number of walls at or above each
        update_cache(map[x], cache);

        for (int y = 0; y < height; ++y)
        {
            point_t point = point_t(x,y);
            box_t box = largest_rect_for_point(dim, point, cache);
            box_queue.push(box);
        }
    }

    // Step 2 : Begin popping rects
    std::vector<box_t> rects;

    while(true)
    {
        const box_t& box = box_queue.top();
        box_queue.pop();

        if (bg::area(box) == 1)
        {
            break;
        }

        bool success = true;
        for (const box_t& existing : rects)
        {
            if (bg::intersects(box, existing))
            {
                success = false;
                break;
            }
        }

        if (success)
        {
            rects.push_back(box);
        }
    }

    return rects;
}


box_t largest_rect_for_point(const point_t& dim, const point_t& point, const std::vector<int>& cache)
{
    point_t ur = point;
    bg::subtract_point(ur, point_t(1,1));

    int x_max = INT_MAX;
    int x = point.get<0>();
    int max_area = 0;

    for(int y = point.get<1>(); y < dim.get<1>(); ++y)
    {
        if (!cache[y+1])
        {
            break;
        }

        x = std::min(point.get<0>() + cache[y] - 1, x_max); // Use the cache, Luke!
        x_max = x;
        if (bg::area(box_t(point, point_t(x,y))) > max_area)
        {
            max_area = bg::area(box_t(point, point_t(x,y)));
            ur = point_t(x,y);
        }
    }

    return box_t(point, ur);
}

void update_cache(const std::vector<bool>& col, std::vector<int>& cache)
{
    for (int y = 0; y < col.size(); ++y)
    {
        cache[y] = col[y] ? cache[y]+1 : 0;
    }
}

}


