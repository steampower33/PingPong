#pragma once
#include <iostream>

class Ball {
private:
    const int screen_width = 1280;
    const int screen_height = 800;
    float x, y;
    int radius;
    int leftScore;
    int rightScore;
    Ball(const Ball& other);
    Ball& operator=(const Ball& other);
public:
    Ball();
    ~Ball();
    void Draw();
    float GetX();
    void SetX(int num);
    float GetY();
    void SetY(int num);
    int GetRadius();
    int GetLeftScore();
    void SetLeftScore(int score);
    int GetRightScore();
    void SetRightScore(int score);
    int IsGameOver();
};
