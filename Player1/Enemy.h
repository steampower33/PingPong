#pragma once
#ifndef ENEMY_H
#define ENEMY_H

#include <iostream>

class Enemy {
protected:
    const int screen_width = 1280;
    const int screen_height = 800;
    float x, y;
    float width, height;
    int speed;
    Enemy(const Enemy& other);
    Enemy& operator=(const Enemy& other);
public:
    Enemy();
    Enemy(int isLeftRight);
    ~Enemy();
    void Draw();
    void Update(float ball_y);
    void LimitMovement();
    void SetLeftPosition();
    void SetRightPosition();
    float GetX();
    float GetY();
    float GetWidth();
    float GetHeight();
};

#endif