#pragma once
#include <vector>
#include <map>
#include "position.h"
#include "colors.h"

class Block
{
public:
    Block();
    int id;
    void Draw(int offsetX, int offsetY);
    void Move(int rows, int columns);
    std::vector<Position> GetCellPositions();
    int GetCellSize();
    void Rotate();
    void UndoRotation();
    std::map<int, std::vector<Position>> cells;

    int GetRowOffset();
    void SetRowOffset(int value);

    int GetColumnOffset();
    void SetColumnOffset(int value);

private:
    int cellSize;
    int rotationState;
    std::vector<Color> colors;
    int rowOffset;
    int columnOffset;
};