#pragma once
#include <iostream>

class Ball {
private:
    const int screen_width = 1280;
    const int screen_height = 800;
    int x, y;
    int speed_x, speed_y;
    int radius;
    int leftScore = 0;
    int rightScore = 0;
    Ball(const Ball& other);
    Ball& operator=(const Ball& other);
public:
    Ball();
    ~Ball();
    void Draw();
    void Update(float posX, float posY);
    void ResetBall();
    int GetX();
    void SetX(int num);
    int GetY();
    void SetY(int num);
    int GetSpeedX();
    void SetSpeedX(int num);
    int GetSpeedY();
    void SetSpeedY(int num);
    int GetRadius();
    int GetLeftScore();
    int GetRightScore();
    int IsGameOver();
};
