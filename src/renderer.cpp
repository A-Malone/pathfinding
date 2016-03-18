
#include "renderer.hpp"
#include "terrain.hpp"

#include <SFML/Graphics.hpp>

int main()
{
    int w = 50;
    int h = 50;
    // Set up terrain
    Terrain* terrain = new Terrain(w,h);

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
                if (terrain->at(x,y)->wall)
                {
                    sf::RectangleShape box(sf::Vector2f(side, side));
                    box.setFillColor(sf::Color::White);
                    box.setPosition(x*(side + space), y*(side + space));
                    window.draw(box);
                }
            }
        }

        window.display();
    }

    delete terrain;

    return 0;
}
