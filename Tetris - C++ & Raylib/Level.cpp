#include "level.h"

void ResetLevel(int& level, double& dropInterval)
{
    level = 1;
    dropInterval = 0.5;
}

void UpdateLevel(int score, int& level, double& dropInterval)
{
    int newLevel = score / 500 + 1;
    if (newLevel > level)
    {
        level = newLevel;
        dropInterval *= 0.9; // Aumenta la velocidad de caída
    }
}
