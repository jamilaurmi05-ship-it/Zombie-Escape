
#include "score.h"

void ScoreManager::update(float dt)
{
    survivalTime += dt;

    score = static_cast<int>(survivalTime * 10);
}

int ScoreManager::getScore() const
{
    return score;
}

int ScoreManager::getSurvivalTime() const
{
    return static_cast<int>(survivalTime);
}

void ScoreManager::reset()
{
    survivalTime = 0.f;
    score = 0;
}
