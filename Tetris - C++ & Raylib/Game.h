#pragma once
#include "grid.h"
#include "blocks.cpp"
#include <string>

class Game
{
public:
    Game();
    ~Game();
    void Draw();
    void DrawMenu(Font font);
    void DrawGame(Font font, Game& game, int level);
    void DrawGameOver(Font font, int score);
    void EndGame(Font font, int currentScore);
    void DrawPause(Font font);
    void CheckForHighScore(int currentScore);
    void HandleInput();
    void MoveBlockDown();
    void Reset();
    bool gameOver;
    int score;
    Music music; 

private:
    double lastMoveTime = 0.0;
    double lastMoveTimeLeft = 0.0;
    double lastMoveTimeRight = 0.0;
    double moveInterval = 0.1;
    void MoveBlockLeft();
    void MoveBlockRight();
    Block GetRandomBlock();
    std::vector<Block> GetAllBlocks();
    bool IsBlockOutside();
    void RotateBlock();
    void LockBlock();
    bool BlockFits();
    void UpdateScore(int linesCleared, int moveDownPoints);
    Grid grid;
    std::vector<Block> blocks;
    Block currentBlock;
    Block nextBlock;
    Sound rotateSound;
    Sound clearSound;
    std::vector<Block> fallingBlocks; 
    std::string playerName;
    bool enteringName = false; 

};