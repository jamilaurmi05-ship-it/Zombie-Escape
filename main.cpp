#include <SFML/Graphics.hpp>
#include <optional>
#include "player.h"
#include "zombie.h"

int main()
{
    sf::RenderWindow window(
        sf::VideoMode({800u, 600u}),
        "Zombie Escape"
    );

    window.setFramerateLimit(60);

    Player player;
    Zombie zombie;

    sf::Clock clock;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        float dt = clock.restart().asSeconds();

        player.update(dt);
        zombie.update(dt, player.getPosition());

        window.clear(sf::Color(40, 40, 40));

        player.draw(window);
        zombie.draw(window);

        window.display();
    }

    return 0;
}