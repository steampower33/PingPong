#include "Game.h"
#include <raylib.h>

Game::Game() {}

Game::Game(int isLeftRight) : ball(), player(isLeftRight), enemy(isLeftRight)
{
    InitWindow(screen_width, screen_height, "My Pong Game!");
    SetTargetFPS(60);
}

void Game::DrawGame()
{
    BeginDrawing();

    player.Update();

    ClearBackground(BLACK);
    DrawRectangle(screen_width / 2, 0, screen_width / 2, screen_height, BLACK);
    DrawCircle(screen_width / 2, screen_height / 2, 150, BLACK);
    DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);

    ball.Draw();
    player.Draw();
    enemy.Draw();

    DrawText(TextFormat("%i", ball.GetLeftScore()), screen_width / 4 - 20, 20, 80, WHITE);
    DrawText(TextFormat("%i", ball.GetRightScore()), 3 * screen_width / 4 - 20, 20, 80, WHITE);

    EndDrawing();
}

bool Game::IsWindowOpen()
{
    return WindowShouldClose();
}

Ball& Game::GetBall()
{
    return ball;
}

Player& Game::GetPlayer()
{
    return player;
}

Enemy& Game::GetEnemy()
{
    return enemy;
}

Game::~Game() {
    CloseWindow();
}

void Game::DrawEndText()
{
    BeginDrawing();
    DrawText("Game Over", screen_width / 3 - 40, screen_height / 2 - 50, 100, WHITE);
    EndDrawing();
    WaitTime(3);
}