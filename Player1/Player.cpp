#include "Player.h"
#include <raylib.h>

Player::Player() {}

Player::Player(int isLeftRight)
{
    if (isLeftRight == 0)
    {
        SetLeftPosition();
    }
    else if (isLeftRight == 1)
    {
        SetRightPosition();
    }
}

void Player::SetLeftPosition()
{
    width = 25;
    height = 120;
    x = 10;
    y = screen_height / 2 - height / 2;
    speed = 6;
}

void Player::SetRightPosition()
{
    width = 25;
    height = 120;
    x = screen_width - width - 10;
    y = screen_height / 2 - height / 2;
    speed = 6;
}

Player::~Player() {}

void Player::Draw()
{
    DrawRectangleRounded(Rectangle{ x, y, width, height }, 0.8, 0, WHITE);
}

void Player::Update()
{
    if (IsKeyDown(KEY_UP))
    {
        y = y - speed;
    }
    if (IsKeyDown(KEY_DOWN))
    {
        y = y + speed;
    }
    LimitMovement();
}

void Player::LimitMovement()
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

float Player::GetX()
{
    return x;
}

float Player::GetY()
{
    return y;
}

float Player::GetWidth() {
    return width;
}

float Player::GetHeight() {
    return height;
}
