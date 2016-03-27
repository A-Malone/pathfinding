#include "renderer.hpp"

#include <SFML/Graphics.hpp>

#include "ai/astar.hpp"
#include "ai/rtaa.hpp"
#include "core/map.hpp"
#include "core/world.hpp"
#include "core/unit.hpp"

int main()
{
    int w = 100;
    int h = 100;
    // Set up terrain
    World* world = new World(w, h, 10);
    Map* map = new Map(world);

    RTAA solver = RTAA();

    Unit* unit = world->spawn(0,0);

    sf::RenderWindow window(sf::VideoMode(600, 600), "Pathfinding");

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

        world->render(window, side);
        window.display();

        std::vector<MapNode*> path = solver.get_path(
            map,
            map->at(unit->pos()),
            map->at(std::make_pair(w - 1, h - 1))
        );
        std::vector<Node*> global_path = map->to_world_path(path);
        unit->set_path(global_path);

        // Step the world forward
        world->step();
        //std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return 0;
}
