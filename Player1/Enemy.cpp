#include "Enemy.h"
#include <raylib.h>

Enemy::Enemy() {}

Enemy::Enemy(int isLeftRight)
{
    if (isLeftRight == 2)
    {
        SetLeftPosition();
    }
    else if (isLeftRight == 1)
    {
        SetRightPosition();
    }
}

void Enemy::SetLeftPosition()
{
    width = 25;
    height = 120;
    x = 10;
    y = screen_height / 2 - height / 2;
    speed = 6;
}

void Enemy::SetRightPosition()
{
    width = 25;
    height = 120;
    x = screen_width - width - 10;
    y = screen_height / 2 - height / 2;
    speed = 6;
}

void Enemy::Update(float ball_y)
{
    y = ball_y;
    LimitMovement();
}

Enemy::~Enemy() {}

void Enemy::Draw()
{
    DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, WHITE);
}

void Enemy::LimitMovement()
{
    if (y <= 0)
    {
        y = 0;
    }
    if (y + height >= GetScreenHeight())
    {
        y = GetScreenHeight() - height;
    }
}

float Enemy::GetX()
{
    return x;
}

void Enemy::SetY(int num)
{
    y = num;
}

float Enemy::GetY()
{
    return y;
}

float Enemy::GetWidth() {
    return width;
}

float Enemy::GetHeight() {
    return height;
}
