#include "Ball.h"
#include <raylib.h>

Ball::Ball() : radius(20), x(screen_width / 2), y(screen_height / 2), leftScore(0), rightScore(0) {}

Ball::~Ball() {}

void Ball::Draw()
{
    DrawCircle(x, y, radius, WHITE);
}

float Ball::GetX()
{
    return x;
}

void Ball::SetX(int num)
{
    x = num;
}

float Ball::GetY()
{
    return y;
}

void Ball::SetY(int num)
{
    y = num;
}

int Ball::GetRadius()
{
    return radius;
}

int Ball::GetLeftScore()
{
    return leftScore;
}

void Ball::SetLeftScore(int score)
{
    leftScore = score;
}

int Ball::GetRightScore()
{
    return rightScore;
}

void Ball::SetRightScore(int score)
{
    rightScore = score;
}

int Ball::IsGameOver()
{
    if (leftScore == 5 || rightScore == 5)
        return 1;
    else
        return 0;
}