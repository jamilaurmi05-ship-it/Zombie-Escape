#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <vector>

class Player
{
private:
    sf::RectangleShape shape;
    float speed = 250.f;
    int health = 100;

public:
    Player();

    void update(
        float dt,
        const std::vector<sf::FloatRect>& walls
    );

    void draw(sf::RenderWindow& window) const;

    sf::Vector2f getPosition() const;
    sf::FloatRect getBounds() const;

    void takeDamage(int damage);
    int getHealth() const;

    void resetPosition();
    void reset();
};

#endif
