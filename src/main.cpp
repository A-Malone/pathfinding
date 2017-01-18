#include <SFML/Graphics.hpp>

#include "analysis/RSR.hpp"
#include "core/map.hpp"
#include "core/world.hpp"
#include "core/unit.hpp"
#include "pathfinding/astar.hpp"
#include "pathfinding/rtaa.hpp"

int main()
{
    int w = 200;
    int h = 200;

    float scale = 3;

    // Set up terrain
    World* world = new World(w, h);
    Map* map = new Map(world);

    // The actual pathfinder
    RTAA solver = RTAA();

    // A reduction on the map
    RSR rsr = RSR();
    rsr.update(world);

    // The unit that we will be navigating with
    Unit* unit = world->spawn(0,0);

    // Render window
    sf::RenderWindow window(sf::VideoMode(w*scale, h*scale), "Pathfinding");

    // Limit the framerate to 20 frames per second (this step is optional)
    window.setFramerateLimit(20);

    int frame_count = 0;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        world->render(window, scale);
        rsr.render(&window, scale);


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

        ++frame_count;
    }

    return 0;
}

