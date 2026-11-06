#include "zombie.h"
#include <cmath>

Zombie::Zombie()
{
    shape.setSize({32.f, 32.f});
    shape.setOrigin({16.f, 16.f});
    shape.setPosition({650.f, 300.f});
    shape.setFillColor(sf::Color::Red);
}

void Zombie::update(float dt, sf::Vector2f playerPosition)
{
    sf::Vector2f direction =
        playerPosition - shape.getPosition();

    float distance = std::sqrt(
        direction.x * direction.x +
        direction.y * direction.y
    );

    if (distance > 1.f)
    {
        direction /= distance;
        shape.move(direction * speed * dt);
    }
}

void Zombie::draw(sf::RenderWindow& window) const
{
    window.draw(shape);
}