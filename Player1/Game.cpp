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

    // Updating
    ball.Update();
    enemy.Update(enemy.GetY());
    player.Update();

    ball.SetCollisionDetected(0);
    // Checking for collisions
    if (CheckCollisionCircleRec(Vector2{ ball.GetX(), ball.GetY() }, ball.GetRadius(), Rectangle{ player.GetX(), player.GetY(), player.GetWidth(), player.GetHeight() }))
    {
        ball.SetSpeedX(ball.GetSpeedX() * -1);
        ball.SetCollisionDetected(1);
    }
    if (CheckCollisionCircleRec(Vector2{ ball.GetX(), ball.GetY() }, ball.GetRadius(), Rectangle{ enemy.GetX(), enemy.GetY(), enemy.GetWidth(), enemy.GetHeight() }))
    {
        ball.SetSpeedX(ball.GetSpeedX() * -1);
        ball.SetCollisionDetected(1);
    }

    // Drawing
    ClearBackground(BLACK);
    DrawRectangle(screen_width / 2, 0, screen_width / 2, screen_height, BLACK);
    DrawCircle(screen_width / 2, screen_height / 2, 150, BLACK);
    DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);

    ball.Draw();
    player.Draw();
    enemy.Draw();

    DrawText(TextFormat("%i", ball.GetCpuScore()), screen_width / 4 - 20, 20, 80, WHITE);
    DrawText(TextFormat("%i", ball.GetPlayerScore()), 3 * screen_width / 4 - 20, 20, 80, WHITE);
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