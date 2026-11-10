#include "player.h"
#include <algorithm>

Player::Player()
{
    shape.setSize({30.f, 30.f});
    shape.setOrigin({15.f, 15.f});
    shape.setPosition({100.f, 300.f});

    shape.setFillColor(sf::Color::Green);
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(sf::Color::White);
}

void Player::update(
    float dt,
    const std::vector<sf::FloatRect>& walls)
{
    sf::Vector2f movement(0.f, 0.f);

    float currentSpeed = speed;

    if (sf::Keyboard::isKeyPressed(
        sf::Keyboard::Key::LShift))
    {
        currentSpeed = 400.f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        movement.y -= currentSpeed * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        movement.y += currentSpeed * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        movement.x -= currentSpeed * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        movement.x += currentSpeed * dt;

    // Horizontal collision
    shape.move({movement.x, 0.f});

    for (const auto& wall : walls)
    {
        if (shape.getGlobalBounds().findIntersection(wall))
        {
            shape.move({-movement.x, 0.f});
            break;
        }
    }

    // Vertical collision
    shape.move({0.f, movement.y});

    for (const auto& wall : walls)
    {
        if (shape.getGlobalBounds().findIntersection(wall))
        {
            shape.move({0.f, -movement.y});
            break;
        }
    }

    // Screen boundary
    sf::Vector2f pos = shape.getPosition();

    pos.x = std::max(15.f, std::min(785.f, pos.x));
    pos.y = std::max(15.f, std::min(585.f, pos.y));

    shape.setPosition(pos);
}

void Player::draw(sf::RenderWindow& window) const
{
    window.draw(shape);
}

sf::Vector2f Player::getPosition() const
{
    return shape.getPosition();
}

sf::FloatRect Player::getBounds() const
{
    return shape.getGlobalBounds();
}

void Player::takeDamage(int damage)
{
    health -= damage;

    if (health < 0)
        health = 0;
}

int Player::getHealth() const
{
    return health;
}

// Only position reset
void Player::resetPosition()
{
    shape.setPosition({100.f, 300.f});
}

// Full reset for new game
void Player::reset()
{
    health = 100;
    resetPosition();
}
