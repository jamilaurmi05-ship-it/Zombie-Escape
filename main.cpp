
#include <SFML/Graphics.hpp>
#include <optional>
#include <string>
#include <vector>

#include "player.h"
#include "zombie.h"

int main()
{
    sf::RenderWindow window(
        sf::VideoMode({800u, 600u}),
        "Zombie Escape"
    );

    window.setFramerateLimit(60);

    // Player
    Player player;

    // Three Zombies
    std::vector<Zombie> zombies;

    zombies.emplace_back(sf::Vector2f{650.f, 100.f});
    zombies.emplace_back(sf::Vector2f{650.f, 500.f});
    zombies.emplace_back(sf::Vector2f{400.f, 100.f});

    // Walls
    std::vector<sf::RectangleShape> walls;
    std::vector<sf::FloatRect> wallBounds;

    sf::RectangleShape wall1({20.f, 300.f});
    wall1.setPosition({250.f, 100.f});
    wall1.setFillColor(sf::Color(120, 120, 120));
    walls.push_back(wall1);

    sf::RectangleShape wall2({250.f, 20.f});
    wall2.setPosition({400.f, 200.f});
    wall2.setFillColor(sf::Color(120, 120, 120));
    walls.push_back(wall2);

    sf::RectangleShape wall3({20.f, 200.f});
    wall3.setPosition({550.f, 350.f});
    wall3.setFillColor(sf::Color(120, 120, 120));
    walls.push_back(wall3);

    sf::RectangleShape wall4({150.f, 20.f});
    wall4.setPosition({100.f, 450.f});
    wall4.setFillColor(sf::Color(120, 120, 120));
    walls.push_back(wall4);

    // Store wall collision boundaries
    for (const auto& wall : walls)
    {
        wallBounds.push_back(wall.getGlobalBounds());
    }

    // Exit point
    sf::RectangleShape exitPoint({40.f, 40.f});
    exitPoint.setPosition({730.f, 530.f});
    exitPoint.setFillColor(sf::Color::Cyan);

    // Game variables
    sf::Clock clock;
    sf::Clock gameClock;
    sf::Clock damageClock;

    int lives = 3;
    int score = 0;

    bool gameOver = false;
    bool gameWon = false;

    // Font
    sf::Font font;

    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        return 1;
    }

    sf::Text infoText(font);
    infoText.setCharacterSize(22);
    infoText.setFillColor(sf::Color::White);
    infoText.setPosition({20.f, 20.f});

    sf::Text endText(font);
    endText.setCharacterSize(35);
    endText.setFillColor(sf::Color::Yellow);
    endText.setPosition({220.f, 250.f});

    // Game loop
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        float dt = clock.restart().asSeconds();

        if (!gameOver && !gameWon)
        {
            // Update player
            player.update(dt, wallBounds);

            // Update score
            score = static_cast<int>(
                gameClock.getElapsedTime().asSeconds()
            );

            // Increase zombie speed
            float zombieSpeed = 90.f + (score / 10) * 10;

            // Update zombies
            for (auto& zombie : zombies)
            {
                zombie.setSpeed(zombieSpeed);
                zombie.update(dt, player.getPosition());
            }

            // Check zombie collision
            bool hit = false;

            for (auto& zombie : zombies)
            {
                if (player.getBounds().findIntersection(
                    zombie.getBounds()))
                {
                    hit = true;
                    break;
                }
            }

            if (hit &&
                damageClock.getElapsedTime().asSeconds() >= 1.f)
            {
                lives--;
                damageClock.restart();

                player.reset();

                for (auto& zombie : zombies)
                {
                    zombie.reset();
                }

                if (lives <= 0)
                {
                    gameOver = true;
                    endText.setString(
                        "GAME OVER!\nPress R to Restart"
                    );
                }
            }

            // Check exit collision
            if (player.getBounds().findIntersection(
                exitPoint.getGlobalBounds()))
            {
                gameWon = true;

                endText.setString(
                    "YOU WIN!\nPress R to Restart"
                );
            }
        }
        else
        {
            // Restart game
            if (sf::Keyboard::isKeyPressed(
                sf::Keyboard::Key::R))
            {
                lives = 3;
                score = 0;

                gameOver = false;
                gameWon = false;

                player.reset();

                for (auto& zombie : zombies)
                {
                    zombie.reset();
                }

                gameClock.restart();
                damageClock.restart();
            }
        }

        // Update information
        infoText.setString(
            "Lives: " + std::to_string(lives) +
            "     Score: " + std::to_string(score)
        );

        // Draw everything
        window.clear(sf::Color(40, 40, 40));

        // Draw walls
        for (const auto& wall : walls)
        {
            window.draw(wall);
        }

        // Draw exit
        window.draw(exitPoint);

        // Draw player
        player.draw(window);

        // Draw zombies
        for (const auto& zombie : zombies)
        {
            zombie.draw(window);
        }

        // Draw score and lives
        window.draw(infoText);

        // Game end screen
        if (gameOver || gameWon)
        {
            window.draw(endText);
        }

        window.display();
    }

    return 0;
}
