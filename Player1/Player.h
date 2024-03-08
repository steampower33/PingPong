#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>

class Player {
protected:
    const int screen_width = 1280;
    const int screen_height = 800;
    float x, y;
    float width, height;
    int speed;
    Player(const Player& other);
    Player& operator=(const Player& other);
public:
    Player();
    Player(int isLeftRight);
    ~Player();
    void Draw();
    void Update();
    void LimitMovement();
    float GetX();
    float GetY();
    float GetWidth();
    float GetHeight();
    void SetLeftPosition();
    void SetRightPosition();
};

#endif