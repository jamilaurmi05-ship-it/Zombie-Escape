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

    // Speed Boost
    if (sf::Keyboard::isKeyPressed(
        sf::Keyboard::Key::LShift))
    {
        currentSpeed = 400.f;
    }

    // WASD Movement
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        movement.y -= currentSpeed * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        movement.y += currentSpeed * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        movement.x -= currentSpeed * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        movement.x += currentSpeed * dt;


    // Arrow Key Movement
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        movement.y -= currentSpeed * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        movement.y += currentSpeed * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        movement.x -= currentSpeed * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        movement.x += currentSpeed * dt;


    // Horizontal Collision
    shape.move({movement.x, 0.f});

    for (const auto& wall : walls)
    {
        if (shape.getGlobalBounds().findIntersection(wall))
        {
            shape.move({-movement.x, 0.f});
            break;
        }
    }


    // Vertical Collision
    shape.move({0.f, movement.y});

    for (const auto& wall : walls)
    {
        if (shape.getGlobalBounds().findIntersection(wall))
        {
            shape.move({0.f, -movement.y});
            break;
        }
    }


    // Screen Boundary
    sf::Vector2f pos = shape.getPosition();

    pos.x = std::max(15.f, std::min(785.f, pos.x));
    pos.y = std::max(15.f, std::min(585.f, pos.y));

    shape.setPosition(pos);
}


// Draw Player
void Player::draw(sf::RenderWindow& window) const
{
    window.draw(shape);
}


// Get Player Position
sf::Vector2f Player::getPosition() const
{
    return shape.getPosition();
}


// Get Player Boundary
sf::FloatRect Player::getBounds() const
{
    return shape.getGlobalBounds();
}


// Reduce Health
void Player::takeDamage(int damage)
{
    health -= damage;

    if (health < 0)
        health = 0;
}


// Get Health
int Player::getHealth() const
{
    return health;
}


// Reset Position Only
void Player::resetPosition()
{
    shape.setPosition({100.f, 300.f});
}


// Reset Health and Position
void Player::reset()
{
    health = 100;
    resetPosition();
}
