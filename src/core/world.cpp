#include "world.hpp"

#include "unit.hpp"

World::World
(
    unsigned int w,
    unsigned int h,
    unsigned int sight // = std::numeric_limits<unsigned int>::max())
)
    :m_width(w)
    ,m_height(h)
    ,m_sight(sight)
{
    m_full_sight = sight > w && sight > h;

    for (int x = 0; x < w; ++x)
    {
        std::vector<Node*> col;
        col.reserve(h);
        for (int y = 0; y < h; ++y)
        {
            Node* n = new Node(x, y);
            n->seen = m_full_sight;
            col.push_back(n);
        }
        m_map.push_back(col);
    }

    float blob_average = 10.0 / 2500.0 * w * h;

    auto blobs_gen =
            std::bind(
                std::normal_distribution<float>(blob_average, blob_average / 3.0),
                std::knuth_b(std::chrono::system_clock::now().time_since_epoch().count())
            );

    auto rand_x =
            std::bind(
                std::uniform_int_distribution<int>(0, w - 1),
                std::knuth_b(std::chrono::system_clock::now().time_since_epoch().count())
            );

    auto rand_y =
            std::bind(
                std::uniform_int_distribution<int>(0, w - 1),
                std::knuth_b(std::chrono::system_clock::now().time_since_epoch().count())
            );

    auto rand_r =
            std::bind(
                std::normal_distribution<float>(4.0, 2.0),
                std::knuth_b(std::chrono::system_clock::now().time_since_epoch().count())
            );

    int num_blobs = blobs_gen();

    for (int i = 0; i < num_blobs; ++i)
    {
        create_obstacle(rand_x(), rand_y(), rand_r());
    }
}

bool World::step()
{
    for (Unit* unit : m_units)
    {
        unit->move(1);
    }
    update_visible();
}

Unit* World::spawn(int x, int y)
{
    m_units.push_back(new Unit(at(x,y)));
    update_visible();
    return m_units.back();
};

World::~World()
{
    m_map.clear();
}

bool World::is_valid(int x, int y) const
{
    if (x >= m_width || x < 0)
    {
        return false;
    }
    if (y >= m_height || y < 0)
    {
        return false;
    }
    return true;
}

Node* World::at(int x, int y) const
{
    return m_map.at(x).at(y);
}

Node* World::at(std::pair<int, int> p) const
{
    return m_map.at(p.first).at(p.second);
}

std::vector<Node*> World::neighbours(Node* node) const
{
    std::vector<Node*> nodes;
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            if (is_valid(node->x + x, node->y + y) && !(x == y && x == 0))
            {
                Node* new_node = at(node->x + x, node->y + y);
                if (!new_node->is_wall())
                {
                    nodes.push_back(new_node);
                }
            }
        }
    }
    return nodes;
}

void World::render(sf::RenderWindow& window, int scale)
{
    for (const std::vector<Node*>& col : m_map)
    {
        for (Node* node : col)
        {
            sf::RectangleShape box(sf::Vector2f(scale, scale));
            box.setPosition(node->x * scale, node->y * scale);

            if (node->is_wall())
            {
                box.setFillColor(sf::Color::White);
            }
            else if (node->is_seen())
            {
                box.setFillColor(sf::Color(100, 100, 100));
            }
            else
            {
                box.setFillColor(sf::Color::Black);
            }
            window.draw(box);
        }
    }

    for (Unit* unit : m_units)
    {
        for (Node* node : unit->get_path())
        {
            sf::RectangleShape box(sf::Vector2f(scale, scale));
            box.setFillColor(sf::Color::Red);
            box.setPosition(node->x * scale, node->y * scale);
            window.draw(box);
        }

        sf::RectangleShape box(sf::Vector2f(scale, scale));
        if(unit->is_idle())
        {
            box.setFillColor(sf::Color::Green);
        }
        else
        {
            box.setFillColor(sf::Color::Yellow);
        }

        auto pos = unit->pos();
        box.setPosition(pos.first * scale, pos.second * scale);
        window.draw(box);
    }
}

void World::create_obstacle(int px, int py, float r)
{
    for (int x = px - r; x <= px + r; ++x)
    {
        for (int y = py - r; y <= py + r; ++y)
        {
            if (is_valid(x, y) && sqrt(std::pow(px - x, 2) + std::pow(py - y, 2)) < r)
            {
                at(x, y)->wall = true;
            }
        }
    }
}

void World::update_visible()
{
    for (Unit* unit : m_units)
    {
        std::pair<int,int> pos = unit->pos();
        for (int x = pos.first - m_sight; x <= pos.first + m_sight; ++x)
        {
            for (int y = pos.second - m_sight; y <= pos.second + m_sight; ++y)
            {
                bool in_range = sqrt(std::pow(pos.first - x, 2) + std::pow(pos.second - y, 2)) < m_sight;
                if (is_valid(x, y) && in_range && !at(x, y)->seen)
                {
                    at(x, y)->seen = true;
                }
            }
        }
    }
}

