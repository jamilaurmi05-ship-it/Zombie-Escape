#include "player.h"

Player::Player()
{
    shape.setSize({30.f, 30.f});
    shape.setOrigin({15.f, 15.f});
    shape.setPosition({100.f, 300.f});
    shape.setFillColor(sf::Color::Green);
}

void Player::update(float dt)
{
    sf::Vector2f movement(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        movement.y -= speed * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        movement.y += speed * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        movement.x -= speed * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        movement.x += speed * dt;

    shape.move(movement);

    sf::Vector2f pos = shape.getPosition();

    if (pos.x < 15.f) pos.x = 15.f;
    if (pos.x > 785.f) pos.x = 785.f;
    if (pos.y < 15.f) pos.y = 15.f;
    if (pos.y > 585.f) pos.y = 585.f;

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

// Day 2: Collision Detection
sf::FloatRect Player::getBounds() const
{
    return shape.getGlobalBounds();
}

// Day 2: Reset Player Position
void Player::reset()
{
    shape.setPosition({100.f, 300.f});
}
