#include "Ball.h"
#include <raylib.h>

Ball::Ball()
{
    radius = 20;
    x = screen_width / 2;
    y = screen_height / 2;
    speed_x = 7;
    speed_y = 7;
}

Ball::~Ball() {}

void Ball::Draw()
{
    DrawCircle(x, y, radius, WHITE);
}

void Ball::Update()
{
    x += speed_x;
    y += speed_y;

    if (y + radius >= GetScreenHeight() || y - radius <= 0)
    {
        speed_y *= -1;
    }
    if (x + radius >= GetScreenWidth()) // Cpu Wins
    {
        cpu_score++;
        ResetBall();
    }
    if (x - radius <= 0)
    {
        player_score++;
        ResetBall();
    }
}

void Ball::ResetBall()
{
    x = GetScreenWidth() / 2;
    y = GetScreenHeight() / 2;

    int speed_choices[2] = { -1, 1 };
    speed_x *= speed_choices[GetRandomValue(0, 1)];
    speed_y *= speed_choices[GetRandomValue(0, 1)];
}

float Ball::GetY()
{
    return y;
}

float Ball::GetX()
{
    return x;
}

int Ball::GetSpeedX()
{
    return speed_x;
}

void Ball::SetSpeedX(int num)
{
    speed_x = num;
}

int Ball::GetSpeedY()
{
    return speed_y;
}

void Ball::SetSpeedY(int num)
{
    speed_y = num;
}

int Ball::GetRadius()
{
    return radius;
}

int Ball::GetPlayerScore()
{
    return player_score;
}

int Ball::GetCpuScore()
{
    return cpu_score;
}
