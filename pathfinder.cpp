
#include "pathfinder.h"

#include <queue>
#include <algorithm>

PathFinder::PathFinder(
    const std::vector<sf::FloatRect>& walls)
{
    grid.assign(
        ROWS,
        std::vector<int>(COLS, 0)
    );

    // Create grid and mark blocked cells
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            sf::FloatRect cell(
                {c * 40.f + 4.f, r * 40.f + 4.f},
                {32.f, 32.f}
            );

            for (const auto& wall : walls)
            {
                if (cell.findIntersection(wall))
                {
                    grid[r][c] = 1;
                    break;
                }
            }
        }
    }
}

// Convert world position to grid cell
std::pair<int, int> PathFinder::toGrid(
    sf::Vector2f position) const
{
    int row = static_cast<int>(position.y / CELL);
    int col = static_cast<int>(position.x / CELL);

    row = std::max(0, std::min(ROWS - 1, row));
    col = std::max(0, std::min(COLS - 1, col));

    return {row, col};
}

// Convert grid cell to world position
sf::Vector2f PathFinder::toWorld(
    std::pair<int, int> cell) const
{
    return {
        cell.second * CELL + 20.f,
        cell.first * CELL + 20.f
    };
}

// BFS shortest path
std::vector<sf::Vector2f> PathFinder::findPath(
    sf::Vector2f start,
    sf::Vector2f goal) const
{
    using Cell = std::pair<int, int>;

    Cell source = toGrid(start);
    Cell destination = toGrid(goal);

    std::queue<Cell> q;

    // Visited array
    std::vector<std::vector<bool>> visited(
        ROWS,
        std::vector<bool>(COLS, false)
    );

    // Parent array
    std::vector<std::vector<Cell>> parent(
        ROWS,
        std::vector<Cell>(COLS, {-1, -1})
    );

    q.push(source);
    visited[source.first][source.second] = true;

    const int dr[] = {-1, 1, 0, 0};
    const int dc[] = {0, 0, -1, 1};

    bool found = false;

    while (!q.empty())
    {
        Cell current = q.front();
        q.pop();

        if (current == destination)
        {
            found = true;
            break;
        }

        // Check four directions
        for (int i = 0; i < 4; i++)
        {
            int nr = current.first + dr[i];
            int nc = current.second + dc[i];

            if (nr < 0 || nr >= ROWS ||
                nc < 0 || nc >= COLS)
                continue;

            if (visited[nr][nc])
                continue;

            Cell next = {nr, nc};

            if (grid[nr][nc] == 1 &&
                !(next == destination))
                continue;

            visited[nr][nc] = true;
            parent[nr][nc] = current;

            q.push(next);
        }
    }

    std::vector<sf::Vector2f> path;

    if (!found)
        return path;

    // Reconstruct path
    Cell current = destination;

    while (!(current == source))
    {
        path.push_back(toWorld(current));

        current = parent[current.first][current.second];
    }

    path.push_back(toWorld(source));

    std::reverse(path.begin(), path.end());

    return path;
}
