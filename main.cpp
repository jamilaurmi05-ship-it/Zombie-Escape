
#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>
#include <string>

#include "player.h"
#include "zombie.h"
#include "score.h"
#include "pathfinder.h"

int main()
{
    sf::RenderWindow window(
        sf::VideoMode({800u, 600u}),
        "Zombie Escape - BFS Edition"
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

    // BFS PathFinder
    PathFinder pathFinder(wallBounds);

    // Exit
    sf::RectangleShape exitPoint({40.f, 40.f});
    exitPoint.setPosition({730.f, 530.f});
    exitPoint.setFillColor(sf::Color::Cyan);

    // Score
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

    // Title
    sf::Text titleText(font);
    titleText.setString("ZOMBIE ESCAPE");
    titleText.setCharacterSize(48);
    titleText.setFillColor(sf::Color::Red);
    titleText.setPosition({190.f, 130.f});

    // Menu
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
    infoText.setCharacterSize(18);
    infoText.setFillColor(sf::Color::White);
    infoText.setPosition({20.f, 15.f});

    // Health background
    sf::RectangleShape healthBackground({200.f, 18.f});
    healthBackground.setPosition({20.f, 55.f});
    healthBackground.setFillColor(sf::Color(80, 80, 80));

    // Health bar
    sf::RectangleShape healthBar({200.f, 18.f});
    healthBar.setPosition({20.f, 55.f});
    healthBar.setFillColor(sf::Color::Green);

    // End screen
    sf::Text endText(font);
    endText.setCharacterSize(28);
    endText.setFillColor(sf::Color::Yellow);
    endText.setPosition({170.f, 200.f});

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

                        for (auto& zombie : zombies)
                            zombie.reset();

                        scoreManager.reset();

                        damageClock.restart();
                        clock.restart();
                    }

                    else if (key->code == sf::Keyboard::Key::Escape)
                    {
                        window.close();
                    }
                }

                // Restart
                else if (gameOver || gameWon)
                {
                    if (key->code == sf::Keyboard::Key::R)
                    {
                        player.reset();

                        for (auto& zombie : zombies)
                            zombie.reset();

                        scoreManager.reset();

                        gameOver = false;
                        gameWon = false;

                        damageClock.restart();
                        clock.restart();
                    }

                    // Return to menu
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

        // Prevent large frame jumps
        if (dt > 0.05f)
            dt = 0.05f;

        // Game update
        if (started && !gameOver && !gameWon)
        {
            // Player movement
            player.update(dt, wallBounds);

            // Score and survival time
            scoreManager.update(dt);

            // Zombie difficulty
            float zombieSpeed =
                90.f +
                (scoreManager.getSurvivalTime() / 10) * 10.f;

            // BFS Zombie movement
            for (auto& zombie : zombies)
            {
                zombie.setSpeed(zombieSpeed);

                zombie.update(
                    dt,
                    player.getPosition(),
                    pathFinder
                );
            }

            // Check zombie collision
            bool hit = false;

            for (const auto& zombie : zombies)
            {
                if (player.getBounds().findIntersection(
                    zombie.getBounds()))
                {
                    hit = true;
                    break;
                }
            }

            // Player damage
            if (hit &&
                damageClock.getElapsedTime().asSeconds() >= 1.f)
            {
                player.takeDamage(20);

                damageClock.restart();

                // Reset positions only
                player.resetPosition();

                for (auto& zombie : zombies)
                    zombie.reset();

                if (player.getHealth() <= 0)
                {
                    gameOver = true;
                }
            }

            // Exit collision
            if (player.getBounds().findIntersection(
                exitPoint.getGlobalBounds()))
            {
                gameWon = true;
            }
        }

        // Score information
        infoText.setString(
            "Score: " +
            std::to_string(scoreManager.getScore()) +
            "   Survival Time: " +
            std::to_string(scoreManager.getSurvivalTime()) +
            " sec"
        );

        // Health bar
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

        if (!started)
        {
            window.draw(titleText);
            window.draw(menuText);
        }
        else
        {
            for (const auto& wall : walls)
                window.draw(wall);

            window.draw(exitPoint);

            player.draw(window);

            for (const auto& zombie : zombies)
                zombie.draw(window);

            window.draw(infoText);
            window.draw(healthBackground);
            window.draw(healthBar);

            if (gameOver)
            {
                endText.setString(
                    "GAME OVER!\n\n"
                    "Score: " +
                    std::to_string(scoreManager.getScore()) +
                    "\nTime: " +
                    std::to_string(scoreManager.getSurvivalTime()) +
                    " sec\n\nPress R to Restart\nPress M for Menu"
                );

                window.draw(endText);
            }

            if (gameWon)
            {
                endText.setString(
                    "YOU WIN!\n\n"
                    "Score: " +
                    std::to_string(scoreManager.getScore()) +
                    "\nTime: " +
                    std::to_string(scoreManager.getSurvivalTime()) +
                    " sec\n\nPress R to Restart\nPress M for Menu"
                );

                window.draw(endText);
            }
        }

        window.display();
    }

    return 0;
}
