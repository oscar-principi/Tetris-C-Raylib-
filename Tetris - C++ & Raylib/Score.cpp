#include <fstream>
#include <iostream>
#include "Game.h"
#include "Score.h"

struct HighScore {
    std::string playerName;
    int score;

    HighScore() : playerName(""), score(0) {}
};


