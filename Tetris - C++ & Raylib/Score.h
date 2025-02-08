#pragma once
#include <string>

struct HighScore;
HighScore ReadHighScore(const std::string& filename);
void SaveHighScore(const std::string& filename, const HighScore& highScore);
