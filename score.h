
#ifndef SCORE_H
#define SCORE_H

class ScoreManager
{
private:
    float survivalTime = 0.f;
    int score = 0;

public:
    void update(float dt);

    int getScore() const;
    int getSurvivalTime() const;

    void reset();
};

#endif
