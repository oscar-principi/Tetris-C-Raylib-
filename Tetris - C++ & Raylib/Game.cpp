#include "game.h"
#include <raylib.h>
#include <random>
#include "Block.h"
#include "Score.cpp"

Game::Game()
{
    grid = Grid();
    blocks = GetAllBlocks();
    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();
    gameOver = false;
    score = 0;
    InitAudioDevice();
    music = LoadMusicStream("../Sounds/music.mp3");
    PlayMusicStream(music);
    rotateSound = LoadSound("../Sounds/rotate.mp3");
    clearSound = LoadSound("../Sounds/clear.mp3");
}

Game::~Game()
{
    UnloadSound(rotateSound);
    UnloadSound(clearSound);
    UnloadMusicStream(music);
    CloseAudioDevice();
}

Block Game::GetRandomBlock()
{
    if (blocks.empty())
    {
        blocks = GetAllBlocks();
    }
    int randomIndex = rand() % blocks.size();
    Block block = blocks[randomIndex];
    blocks.erase(blocks.begin() + randomIndex);
    return block;
}

std::vector<Block> Game::GetAllBlocks()
{
    return {IBlock(), JBlock(), LBlock(), OBlock(), SBlock(), TBlock(), ZBlock()};
}

void Game::Draw()
{
    grid.Draw();
    currentBlock.Draw(11, 11);
    switch (nextBlock.id)
    {
    case 3:
        nextBlock.Draw(255, 290);
        break;
    case 4:
        nextBlock.Draw(255, 280);
        break;
    default:
        nextBlock.Draw(270, 270);
        break;
    }
}
void Game::DrawMenu(Font font)
{
    HighScore highScore = ReadHighScore("highscore.txt");

    DrawRectangleRounded({ 110, 190, 310, 150 }, 0.3, 6, lightBlue);
    DrawTextEx(font, "TETRIS", { 160, 200 }, 50, 2, WHITE);
    DrawTextEx(font, "Presiona ENTER para jugar", { 120, 300 }, 20, 2, WHITE);

    std::string recordText = " Record: " + highScore.playerName + " - " + std::to_string(highScore.score) + " puntos";
    DrawTextEx(font, recordText.c_str(), { 120, 550 }, 20, 2, YELLOW);

    Block block2 = OBlock();
    block2.Draw(-50, 50);

    Block block3 = TBlock();
    block3.Draw(60, 50);

    Block block4 = ZBlock();
    block4.Draw(180, 50);

    Block block5 = SBlock();
    block5.Draw(300, 50);
}

void Game::CheckForHighScore(int currentScore)
{
    HighScore highScore = ReadHighScore("highscore.txt");

    if (currentScore > highScore.score) {
        std::string playerName;
        std::cout << "Nuevo record! Ingresa tu nombre: ";
        std::getline(std::cin, playerName);

        highScore.playerName = playerName;
        highScore.score = currentScore;

        SaveHighScore("highscore.txt", highScore);
    }
}


void Game::DrawGame(Font font, Game& game, int level)
{
    DrawTextEx(font, "Score", { 365, 15 }, 38, 2, WHITE);
    DrawTextEx(font, "Next", { 370, 175 }, 38, 2, WHITE);
    DrawTextEx(font, TextFormat("Level: %d", level), { 365, 400 }, 38, 2, WHITE); 

    if (game.gameOver)
    {
        DrawTextEx(font, "GAME \n OVER", { 320, 450 }, 38, 2, WHITE);
    }

    DrawRectangleRounded({ 320, 55, 170, 60 }, 0.3, 6, lightBlue);

    char scoreText[10];
    sprintf_s(scoreText, "%d", game.score);
    Vector2 textSize = MeasureTextEx(font, scoreText, 38, 2);

    DrawTextEx(font, scoreText, { 320 + (170 - textSize.x) / 2, 65 }, 38, 2, WHITE);
    DrawRectangleRounded({ 320, 215, 170, 180 }, 0.3, 6, lightBlue);

    game.Draw();
}

void Game::DrawGameOver(Font font, int score)
{
    HighScore highScore = ReadHighScore("highscore.txt");

    if (score > highScore.score && !enteringName)
    {
        enteringName = true;
        playerName = "";
    }

    BeginDrawing();
    ClearBackground(darkBlue);

    DrawTextEx(font, "GAME OVER", { 130, 200 }, 50, 2, RED);
    DrawTextEx(font, TextFormat("Final Score: %d", score), { 150, 300 }, 30, 2, WHITE);

    if (enteringName)
    {
        DrawTextEx(font, "Nuevo record! ", { 150, 350 }, 20, 2, YELLOW);
        DrawTextEx(font, "Ingresa tu nombre : ", { 100, 370 }, 20, 2, YELLOW);
        DrawTextEx(font, playerName.c_str(), { 310, 365 }, 30, 2, WHITE);
        DrawTextEx(font, "Presiona ENTER para guardar", { 100, 450 }, 20, 2, GREEN);
    }
    else
    {
        std::string recordText = "Record: " + highScore.playerName + " - " + std::to_string(highScore.score) + " puntos";
        DrawTextEx(font, recordText.c_str(), { 150, 350 }, 20, 2, YELLOW);
        DrawTextEx(font, "Press ENTER to return to menu", { 100, 400 }, 20, 2, WHITE);
    }

    EndDrawing();

    if (enteringName)
    {
        int key = GetCharPressed();

        while (key > 0)
        {
            if ((key >= 32) && (key <= 125) && playerName.length() < 10) 
            {
                playerName += static_cast<char>(key);
            }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE) && !playerName.empty())
        {
            playerName.pop_back();
        }

        if (IsKeyPressed(KEY_ENTER) && !playerName.empty())
        {
            highScore.playerName = playerName;
            highScore.score = score;
            SaveHighScore("highscore.txt", highScore);

            enteringName = false; 
        }
    }
}


void Game::EndGame(Font font, int currentScore)
{
    DrawGameOver(font, currentScore);

    std::cout << "Game Over! Tu puntaje final es: " << currentScore << std::endl;

    currentScore = 0;
}


HighScore ReadHighScore(const std::string& filename) {
    HighScore highScore;

    std::ifstream file(filename);

    if (!file.is_open()) {
        std::ofstream newFile(filename);
        if (newFile.is_open()) {
            newFile << highScore.playerName << "\n" << highScore.score << "\n";
            newFile.close();
        } else {
            std::cerr << "Error al crear el archivo de puntajes." << std::endl;
        }
    } else {
        std::getline(file, highScore.playerName);
        file >> highScore.score;
        file.close();
    }

    return highScore;
}


void SaveHighScore(const std::string& filename, const HighScore& highScore) {
    std::ofstream file(filename);

    if (file.is_open()) {
        file << highScore.playerName << "\n" << highScore.score << "\n";
        file.close();
    } else {
        std::cerr << "Error al abrir el archivo para guardar el puntaje." << std::endl;
    }
}




void Game::DrawPause(Font font)
{
    DrawTextEx(font, "PAUSED", { 180, 200 }, 50, 2, YELLOW);
    DrawTextEx(font, "Press ENTER to resume", { 100, 300 }, 20, 2, WHITE);
    DrawTextEx(font, "Press ESC to return to menu", { 100, 350 }, 20, 2, WHITE);
}

void Game::HandleInput()
{
    int keyPressed = GetKeyPressed();  
    double currentTime = GetTime();    

    if (gameOver && keyPressed != 0)
    {
        gameOver = false;
        Reset();
    }

    switch (keyPressed)
    {
    case KEY_LEFT:

        if (currentTime - lastMoveTimeLeft >= moveInterval)
        {
            MoveBlockLeft();  
            lastMoveTimeLeft = currentTime;  
        }
        break;

    case KEY_RIGHT:
        if (currentTime - lastMoveTimeRight >= moveInterval)
        {
            MoveBlockRight();
            lastMoveTimeRight = currentTime;
        }
        break;

    case KEY_UP:
        RotateBlock();  
        break;

    case KEY_DOWN:  
        if (currentTime - lastMoveTime >= moveInterval)
        {
            MoveBlockDown();
            UpdateScore(0, 1);
            lastMoveTime = currentTime; 
        }
        break;

    default:
        break;  
    }

    if (IsKeyDown(KEY_LEFT) && currentTime - lastMoveTimeLeft >= moveInterval)
    {
        MoveBlockLeft();
        lastMoveTimeLeft = currentTime;
    }

    if (IsKeyDown(KEY_RIGHT) && currentTime - lastMoveTimeRight >= moveInterval)
    {
        MoveBlockRight();
        lastMoveTimeRight = currentTime;
    }

    if (IsKeyDown(KEY_DOWN) && currentTime - lastMoveTime >= moveInterval)
    {
        MoveBlockDown();
        UpdateScore(0, 1);
        lastMoveTime = currentTime;
    }
}



void Game::MoveBlockLeft()
{
    if (!gameOver)
    {
        currentBlock.Move(0, -1);
        if (IsBlockOutside() || BlockFits() == false)
        {
            currentBlock.Move(0, 1);
        }
    }
}

void Game::MoveBlockRight()
{
    if (!gameOver)
    {
        currentBlock.Move(0, 1);
        if (IsBlockOutside() || BlockFits() == false)
        {
            currentBlock.Move(0, -1);
        }
    }
}

void Game::MoveBlockDown()
{
    if (!gameOver)
    {
        currentBlock.Move(1, 0);
        if (IsBlockOutside() || BlockFits() == false)
        {
            currentBlock.Move(-1, 0);
            LockBlock();
        }
    }
}

bool Game::IsBlockOutside()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for (Position item : tiles)
    {
        if (grid.IsCellOutside(item.row, item.column))
        {
            return true;
        }
    }
    return false;
}

void Game::RotateBlock()
{
    if (!gameOver)
    {
        currentBlock.Rotate();
        if (IsBlockOutside() || BlockFits() == false)
        {
            currentBlock.UndoRotation();
        }
        else
        {
            PlaySound(rotateSound);
        }
    }
}

void Game::LockBlock()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for (Position item : tiles)
    {
        grid.grid[item.row][item.column] = currentBlock.id;
    }
    currentBlock = nextBlock;
    if (BlockFits() == false)
    {
        gameOver = true;
    }
    nextBlock = GetRandomBlock();
    int rowsCleared = grid.ClearFullRows();
    if (rowsCleared > 0)
    {
        PlaySound(clearSound);
        UpdateScore(rowsCleared, 0);
    }
}

bool Game::BlockFits()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for (Position item : tiles)
    {
        if (grid.IsCellEmpty(item.row, item.column) == false)
        {
            return false;
        }
    }
    return true;
}

void Game::Reset()
{
    grid.Initialize();
    blocks = GetAllBlocks();
    score = 0;
    gameOver = false;

    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();   

    fallingBlocks.clear(); 

    StopSound(rotateSound);
    StopSound(clearSound);
}

void Game::UpdateScore(int linesCleared, int moveDownPoints)
{
    switch (linesCleared)
    {
    case 1:
        score += 100;
        break;
    case 2:
        score += 300;
        break;
    case 3:
        score += 500;
        break;
    default:
        break;
    }

    score += moveDownPoints;
}
