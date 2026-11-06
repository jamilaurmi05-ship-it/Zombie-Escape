#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

class Player
{
private:
    sf::RectangleShape shape;
    float speed = 250.0f;

public:
    Player();

    void update(float dt);
    void draw(sf::RenderWindow& window) const;
    sf::Vector2f getPosition() const;
};

#endif