#pragma once

#include <iostream>
#include "Ball.h"
#include "Player.h"
#include "Enemy.h"

class Game {
private:
    const int screen_width = 1280;
    const int screen_height = 800;
    Ball ball;
    Player player;
    Enemy enemy;
    Game(const Game& other);
    Game& operator=(const Game& other);

public:
    Game();
    Game(int isLeftRight);
    ~Game();
    void DrawGame();
    bool IsWindowOpen();
    Ball& GetBall();
    Player& GetPlayer();
    Enemy& GetEnemy();
    void DrawEndText();
};
