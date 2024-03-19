#include "Ball.h"
#include <raylib.h>

Ball::Ball() : x(kScreenWidth / 2), y(kScreenHeight / 2), radius(20), speed_x(3), speed_y(3), left_score(0), right_score(0) {}

Ball::~Ball() {}

void Ball::Draw()
{
    DrawCircle(x, y, radius, WHITE);
}

void Ball::Update(float posY1, float posY2)
{
    x += speed_x;
    y += speed_y;

    if (y + radius >= kScreenHeight || y - radius <= 0)
    {
        speed_y *= -1;
    }
    if (CheckCollisionCircleRec(Vector2{ (float)x, (float)y }, (float)radius, Rectangle{ 10, posY1, 25, 120 }))
    {
        speed_x *= -1;
    }
    if (CheckCollisionCircleRec(Vector2{ (float)x, (float)y }, (float)radius, Rectangle{ (float)kScreenWidth - 25 - 10, posY2, 25, 120 }))
    {
        speed_x *= -1;
    }
    if (x - radius <= 0)
    {
        left_score++;
        ResetBall();
    }
    if (x + radius >= kScreenWidth)
    {
        right_score++;
        ResetBall();
    }
}

void Ball::ResetBall()
{
    x = kScreenWidth / 2;
    y = kScreenHeight / 2;

    int speed_choices[2] = { -1, 1 };
    speed_x *= speed_choices[GetRandomValue(0, 1)];
    speed_y *= speed_choices[GetRandomValue(0, 1)];
}

int Ball::GetX()
{
    return x;
}

void Ball::SetX(int num)
{
    x = num;
}

int Ball::GetY()
{
    return y;
}

void Ball::SetY(int num)
{
    x = num;
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

int Ball::GetLeftScore()
{
    return left_score;
}

int Ball::GetRightScore()
{
    return right_score;
}

int Ball::IsGameOver()
{
    if (left_score == 3 || right_score == 3)
        return 1;
    else
        return 0;
}