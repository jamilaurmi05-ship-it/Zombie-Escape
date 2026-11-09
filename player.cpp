#include "player.h"

Player::Player()
{
    shape.setSize({30.f, 30.f});
    shape.setOrigin({15.f, 15.f});
    shape.setPosition({100.f, 300.f});

    // Player appearance
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

    if (sf::Keyboard::isKeyPressed(
        sf::Keyboard::Key::W))
        movement.y -= currentSpeed * dt;

    if (sf::Keyboard::isKeyPressed(
        sf::Keyboard::Key::S))
        movement.y += currentSpeed * dt;

    if (sf::Keyboard::isKeyPressed(
        sf::Keyboard::Key::A))
        movement.x -= currentSpeed * dt;

    if (sf::Keyboard::isKeyPressed(
        sf::Keyboard::Key::D))
        movement.x += currentSpeed * dt;

    sf::Vector2f oldPosition = shape.getPosition();

    shape.move(movement);

    sf::Vector2f pos = shape.getPosition();

    if (pos.x < 15.f) pos.x = 15.f;
    if (pos.x > 785.f) pos.x = 785.f;
    if (pos.y < 15.f) pos.y = 15.f;
    if (pos.y > 585.f) pos.y = 585.f;

    shape.setPosition(pos);

    for (const auto& wall : walls)
    {
        if (shape.getGlobalBounds().findIntersection(wall))
        {
            shape.setPosition(oldPosition);
            break;
        }
    }
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

void Player::reset()
{
    shape.setPosition({100.f, 300.f});
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

void Player::resetHealth()
{
    health = 100;
}
