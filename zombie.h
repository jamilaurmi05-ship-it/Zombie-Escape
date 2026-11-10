
#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <SFML/Graphics.hpp>
#include <vector>

#include "pathfinder.h"

class Zombie
{
private:
    sf::RectangleShape shape;

    sf::Vector2f startPosition;

    float speed = 90.f;
    float pathTimer = 0.f;

    std::vector<sf::Vector2f> path;

public:
    Zombie(sf::Vector2f position);

    void update(
        float dt,
        sf::Vector2f playerPosition,
        const PathFinder& finder
    );

    void draw(sf::RenderWindow& window) const;

    sf::FloatRect getBounds() const;

    void reset();
    void setSpeed(float newSpeed);
};

#endif
