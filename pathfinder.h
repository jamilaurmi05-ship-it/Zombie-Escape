
#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <utility>

class PathFinder
{
private:
    static const int ROWS = 15;
    static const int COLS = 20;
    static const int CELL = 40;

    std::vector<std::vector<int>> grid;

    std::pair<int, int> toGrid(
        sf::Vector2f position
    ) const;

    sf::Vector2f toWorld(
        std::pair<int, int> cell
    ) const;

public:
    PathFinder(
        const std::vector<sf::FloatRect>& walls
    );

    std::vector<sf::Vector2f> findPath(
        sf::Vector2f start,
        sf::Vector2f goal
    ) const;
};

#endif
