
#include "renderer.hpp"

#include <SFML/Graphics.hpp>

#include "core/astar.hpp"
#include "core/terrain.hpp"

int main()
{
    int w = 50;
    int h = 50;
    // Set up terrain
    Terrain terrain(w,h);

    AStar solver = AStar();
    std::vector<Node*> path = solver.get_path(
        terrain,
        std::make_pair(0,0),
        std::make_pair(w-1,h-1)
    );

    /*
    for (Node* node : path)
    {
        std::cout << node->x << " " << node->y << std::endl;
    }
    */

    sf::RenderWindow window(sf::VideoMode(600, 600), "SFML works!");

    // Limit the framerate to 60 frames per second (this step is optional)
    window.setFramerateLimit(60);

    int side = 5;
    int space = 5;


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        for(int x = 0; x < w; ++x)
        {
            for(int y = 0; y < h; ++y)
            {
                if (terrain.at(x,y)->wall)
                {
                    sf::RectangleShape box(sf::Vector2f(side, side));
                    box.setFillColor(sf::Color::White);
                    box.setPosition(x*(side + space), y*(side + space));
                    window.draw(box);
                }
            }
        }

        for (Node* node : path)
        {
            sf::RectangleShape box(sf::Vector2f(side, side));
            box.setFillColor(sf::Color::Red);
            box.setPosition(node->x*(side + space), node->y*(side + space));
            window.draw(box);
        }

        window.display();
    }

    return 0;
}
