
#include "zombie.h"

#include <cmath>

Zombie::Zombie(sf::Vector2f position)
{
    startPosition = position;

    shape.setSize({32.f, 32.f});
    shape.setOrigin({16.f, 16.f});
    shape.setPosition(position);

    shape.setFillColor(sf::Color(180, 30, 30));
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(sf::Color::White);
}

void Zombie::update(
    float dt,
    sf::Vector2f playerPosition,
    const PathFinder& finder)
{
    pathTimer += dt;

    // Recalculate path every 0.25 seconds
    if (pathTimer >= 0.25f || path.empty())
    {
        path = finder.findPath(
            shape.getPosition(),
            playerPosition
        );

        pathTimer = 0.f;
    }

    if (path.empty())
        return;

    sf::Vector2f target = path.front();

    if (path.size() > 1)
        target = path[1];

    sf::Vector2f direction =
        target - shape.getPosition();

    float distance = std::sqrt(
        direction.x * direction.x +
        direction.y * direction.y
    );

    if (distance > 2.f)
    {
        direction /= distance;

        float step = speed * dt;

        if (step >= distance)
            shape.setPosition(target);
        else
            shape.move(direction * step);
    }
}

void Zombie::draw(sf::RenderWindow& window) const
{
    window.draw(shape);
}

sf::FloatRect Zombie::getBounds() const
{
    return shape.getGlobalBounds();
}

void Zombie::reset()
{
    shape.setPosition(startPosition);
    speed = 90.f;
    pathTimer = 0.f;
    path.clear();
}

void Zombie::setSpeed(float newSpeed)
{
    speed = newSpeed;
}
