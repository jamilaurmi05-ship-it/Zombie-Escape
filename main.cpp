#include <SFML/Graphics.hpp>
#include <optional>
#include <string>
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

    int lives = 3;
    bool gameOver = false;

    sf::Font font;

    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf"))
        return 1;

    sf::Text lifeText(font);
    lifeText.setCharacterSize(24);
    lifeText.setFillColor(sf::Color::White);
    lifeText.setPosition({20.f, 20.f});

    sf::Text gameOverText(font);
    gameOverText.setString(
        "GAME OVER!\nPress R to Restart"
    );
    gameOverText.setCharacterSize(35);
    gameOverText.setFillColor(sf::Color::Yellow);
    gameOverText.setPosition({220.f, 250.f});

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        float dt = clock.restart().asSeconds();

        if (!gameOver)
        {
            player.update(dt);
            zombie.update(dt, player.getPosition());

            if (player.getBounds().findIntersection(
                    zombie.getBounds()))
            {
                lives--;

                player.reset();
                zombie.reset();

                if (lives <= 0)
                    gameOver = true;
            }
        }
        else
        {
            if (sf::Keyboard::isKeyPressed(
                    sf::Keyboard::Key::R))
            {
                lives = 3;
                gameOver = false;

                player.reset();
                zombie.reset();
            }
        }

        lifeText.setString(
            "Lives: " + std::to_string(lives)
        );

        window.clear(sf::Color(40, 40, 40));

        player.draw(window);
        zombie.draw(window);

        window.draw(lifeText);

        if (gameOver)
            window.draw(gameOverText);

        window.display();
    }

    return 0;
}