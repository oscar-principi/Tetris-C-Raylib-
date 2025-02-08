#include "Menu.h"
#include "Colors.h"

void DrawMenu(Font font)
{
    DrawRectangleRounded({ 110, 190, 310, 150 }, 0.3, 6, lightBlue);
    DrawTextEx(font, "TETRIS", { 150, 200 }, 50, 2, WHITE);
    DrawTextEx(font, "Presiona ENTER para jugar", { 80, 300 }, 20, 2, WHITE);
}