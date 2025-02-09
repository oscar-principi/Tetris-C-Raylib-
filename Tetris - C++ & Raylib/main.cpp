#include <raylib.h>
#include <iostream>
#include "Game.h"
#include "GameState.h"
#include "Colors.h"
#include "Level.h"

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
                game.Reset();
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

            if (game.gameOver)
            {
                currentState = GAME_OVER;
            }

            if (IsKeyPressed(KEY_P))
            {
                currentState = PAUSED;
            }
        }
        else if (currentState == PAUSED)
        {
            if (IsKeyPressed(KEY_ENTER))
            {
                currentState = PLAYING;
            }
            else if (IsKeyPressed(KEY_ESCAPE))
            {
                currentState = MENU;
            }
        }
        else if (currentState == GAME_OVER)
        {
            if (IsKeyPressed(KEY_ENTER))
            {
                currentState = MENU;
                level = 1;
                dropInterval = 0.5;
                game.Reset();
            }
        }

        BeginDrawing();
        ClearBackground(darkBlue);

        if (currentState == MENU)
        {
            game.DrawMenu(font);
        }
        else if (currentState == PLAYING)
        {
            game.DrawGame(font, game, level);
        }
        else if (currentState == PAUSED)
        {
            game.DrawPause(font);
        }
        else if (currentState == GAME_OVER)
        {
            game.DrawGameOver(font, game.score);
        }

        EndDrawing();
    }

    CloseWindow();
}
