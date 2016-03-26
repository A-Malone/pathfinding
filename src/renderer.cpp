#include "renderer.hpp"

#include <SFML/Graphics.hpp>

#include "ai/astar.hpp"
#include "ai/rtaa.hpp"
#include "core/world.hpp"
#include "core/unit.hpp"

int main()
{
    int w = 200;
    int h = 200;
    // Set up terrain
    World world(w, h, 10);

    RTAA solver = RTAA();

    Unit* unit = world.spawn(0,0);

    sf::RenderWindow window(sf::VideoMode(600, 600), "SFML works!");

    // Limit the framerate to 60 frames per second (this step is optional)
    window.setFramerateLimit(60);

    int side = 3;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        world.render(window, side);
        window.display();

        std::vector<Node*> path = solver.get_path(
			world,
			world.at(unit->pos()),
			world.at(w - 1, h - 1)
		);
		unit->set_path(path);

		world.step();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    return 0;
}
