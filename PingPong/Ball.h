#pragma once
#include <iostream>

class Ball {
private:
    static constexpr int kScreenWidth = 1280;
    static constexpr int kScreenHeight = 800;
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
