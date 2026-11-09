
#include <SFML/Graphics.hpp>
#include <optional>
#include <string>
#include <vector>

#include "player.h"
#include "zombie.h"
#include "score.h"

int main()
{
    sf::RenderWindow window(
        sf::VideoMode({800u, 600u}),
        "Zombie Escape"
    );

    window.setFramerateLimit(60);

    // Player
    Player player;

    // Zombies
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

    for (const auto& wall : walls)
    {
        wallBounds.push_back(wall.getGlobalBounds());
    }

    // Exit
    sf::RectangleShape exitPoint({40.f, 40.f});
    exitPoint.setPosition({730.f, 530.f});
    exitPoint.setFillColor(sf::Color::Cyan);

    // Score system
    ScoreManager scoreManager;

    // Clocks
    sf::Clock clock;
    sf::Clock damageClock;

    // Game states
    bool started = false;
    bool gameOver = false;
    bool gameWon = false;

    // Font
    sf::Font font;

    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        return 1;
    }

    // Start menu title
    sf::Text titleText(font);
    titleText.setString("ZOMBIE ESCAPE");
    titleText.setCharacterSize(48);
    titleText.setFillColor(sf::Color::Red);
    titleText.setPosition({190.f, 150.f});

    sf::Text menuText(font);
    menuText.setString(
        "Press ENTER to Play\n\n"
        "Press ESC to Exit"
    );
    menuText.setCharacterSize(25);
    menuText.setFillColor(sf::Color::White);
    menuText.setPosition({260.f, 280.f});

    // Game information
    sf::Text infoText(font);
    infoText.setCharacterSize(20);
    infoText.setFillColor(sf::Color::White);
    infoText.setPosition({20.f, 20.f});

    // Health bar background
    sf::RectangleShape healthBackground({200.f, 18.f});
    healthBackground.setPosition({20.f, 55.f});
    healthBackground.setFillColor(sf::Color(80, 80, 80));

    // Health bar
    sf::RectangleShape healthBar({200.f, 18.f});
    healthBar.setPosition({20.f, 55.f});
    healthBar.setFillColor(sf::Color::Green);

    // Health label
    sf::Text healthLabel(font);
    healthLabel.setString("HEALTH");
    healthLabel.setCharacterSize(16);
    healthLabel.setFillColor(sf::Color::White);
    healthLabel.setPosition({230.f, 52.f});

    // End screen
    sf::Text endText(font);
    endText.setCharacterSize(35);
    endText.setFillColor(sf::Color::Yellow);
    endText.setPosition({190.f, 230.f});

    // Main game loop
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (const auto* key =
                event->getIf<sf::Event::KeyPressed>())
            {
                // Start menu
                if (!started)
                {
                    if (key->code == sf::Keyboard::Key::Enter)
                    {
                        started = true;
                        gameOver = false;
                        gameWon = false;

                        player.reset();
                        player.resetHealth();

                        for (auto& zombie : zombies)
                        {
                            zombie.reset();
                        }

                        scoreManager.reset();
                        damageClock.restart();
                    }
                    else if (key->code == sf::Keyboard::Key::Escape)
                    {
                        window.close();
                    }
                }

                // Restart or return to menu
                else if (gameOver || gameWon)
                {
                    if (key->code == sf::Keyboard::Key::R)
                    {
                        player.reset();
                        player.resetHealth();

                        for (auto& zombie : zombies)
                        {
                            zombie.reset();
                        }

                        scoreManager.reset();
                        damageClock.restart();

                        gameOver = false;
                        gameWon = false;
                    }

                    if (key->code == sf::Keyboard::Key::M)
                    {
                        started = false;
                        gameOver = false;
                        gameWon = false;
                    }
                }
            }
        }

        float dt = clock.restart().asSeconds();

        // Game update
        if (started && !gameOver && !gameWon)
        {
            // Update player
            player.update(dt, wallBounds);

            // Update score and survival time
            scoreManager.update(dt);

            // Increase zombie speed
            float zombieSpeed =
                90.f + (scoreManager.getSurvivalTime() / 10) * 10;

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

            // Player takes damage
            if (hit &&
                damageClock.getElapsedTime().asSeconds() >= 1.f)
            {
                player.takeDamage(20);
                damageClock.restart();

                player.reset();

                for (auto& zombie : zombies)
                {
                    zombie.reset();
                }

                if (player.getHealth() <= 0)
                {
                    gameOver = true;
                }
            }

            // Check exit
            if (player.getBounds().findIntersection(
                exitPoint.getGlobalBounds()))
            {
                gameWon = true;
            }
        }

        // Update information
        infoText.setString(
            "Score: " +
            std::to_string(scoreManager.getScore()) +
            "     Survival Time: " +
            std::to_string(scoreManager.getSurvivalTime()) +
            " sec"
        );

        // Update health bar
        healthBar.setSize({
            player.getHealth() * 2.f,
            18.f
        });

        if (player.getHealth() > 50)
        {
            healthBar.setFillColor(sf::Color::Green);
        }
        else if (player.getHealth() > 20)
        {
            healthBar.setFillColor(sf::Color::Yellow);
        }
        else
        {
            healthBar.setFillColor(sf::Color::Red);
        }

        // Draw screen
        window.clear(sf::Color(30, 30, 30));

        // Start menu
        if (!started)
        {
            window.draw(titleText);
            window.draw(menuText);
        }

        // Game screen
        else
        {
            for (const auto& wall : walls)
            {
                window.draw(wall);
            }

            window.draw(exitPoint);

            player.draw(window);

            for (const auto& zombie : zombies)
            {
                zombie.draw(window);
            }

            window.draw(infoText);
            window.draw(healthBackground);
            window.draw(healthBar);
            window.draw(healthLabel);

            // Game over screen
            if (gameOver)
            {
                endText.setString(
                    "GAME OVER!\n"
                    "Press R to Restart\n"
                    "Press M for Menu"
                );

                window.draw(endText);
            }

            // Win screen
            if (gameWon)
            {
                endText.setString(
                    "YOU WIN!\n"
                    "Press R to Restart\n"
                    "Press M for Menu"
                );

                window.draw(endText);
            }
        }

        window.display();
    }

    return 0;
}
