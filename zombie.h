
#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <SFML/Graphics.hpp>

class Zombie
{
private:
    sf::RectangleShape shape;
    float speed = 90.f;
    sf::Vector2f startPosition;

public:
    Zombie(sf::Vector2f position);

    void update(float dt, sf::Vector2f playerPosition);
    void draw(sf::RenderWindow& window) const;

    sf::FloatRect getBounds() const;

    void reset();
    void setSpeed(float newSpeed);
};

#endif
