#include "renderer.hpp"

#include <SFML/Graphics.hpp>

#include "ai/astar.hpp"
#include "ai/rtaa.hpp"
#include "core/world.hpp"
#include "core/unit.hpp"

int main()
{
    int w = 50;
    int h = 50;
    // Set up terrain
    World world(w, h, 10);

    //AStar solver = AStar();
    RTAA solver = RTAA();

    Unit* unit = world.spawn(0,0);

    sf::RenderWindow window(sf::VideoMode(600, 600), "SFML works!");

    // Limit the framerate to 60 frames per second (this step is optional)
    window.setFramerateLimit(60);

    int side = 5;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        for (int x = 0; x < w; ++x)
        {
            for (int y = 0; y < h; ++y)
            {
                Node* node = world.at(x, y);
                sf::RectangleShape box(sf::Vector2f(side, side));
                box.setPosition(x * side, y * side);

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

        std::vector<Node*> path = solver.get_path(
            world,
            world.at(unit->pos()),
            world.at(w - 1, h - 1)
        );
        unit->set_path(path);

        for (Node* node : path)
        {
            sf::RectangleShape box(sf::Vector2f(side, side));
            box.setFillColor(sf::Color::Red);
            box.setPosition(node->x * side, node->y * side);
            window.draw(box);
        }

        world.step();

        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        window.display();
    }

    return 0;
}
