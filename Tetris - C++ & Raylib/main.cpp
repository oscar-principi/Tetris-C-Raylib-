#include <raylib.h>
#include <iostream>
#include "Game.h"
#include "Colors.h"
#include "Menu.h"
#include "Level.h"
#include "GameState.h"

double lastUpdateTime = 0;
GameState currentState = MENU;
int level = 1;
double dropInterval = 0.5;

bool EventTriggered(double interval)
{
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

int main()
{
    InitWindow(500, 620, "Tetris - C++ & Raylib");
    SetTargetFPS(60);

    Font font = LoadFontEx("Font/monogram.ttf", 64, 0, 0);
    Game game = Game();

    while (!WindowShouldClose())
    {
        if (currentState == MENU)
        {
            if (IsKeyPressed(KEY_ENTER))
            {
                currentState = PLAYING;
                ResetLevel(level, dropInterval);
            }
        }
        else if (currentState == PLAYING)
        {
            UpdateMusicStream(game.music);
            game.HandleInput();

            if (EventTriggered(dropInterval))
            {
                game.MoveBlockDown();
            }

            UpdateLevel(game.score, level, dropInterval);
        }

        BeginDrawing();
        ClearBackground(darkBlue);

        if (currentState == MENU)
        {
            DrawMenu(font);
        }
        else if (currentState == PLAYING)
        {
            DrawGame(font, game, level);
        }

        EndDrawing();
    }

    CloseWindow();
}
