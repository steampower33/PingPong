#include <iostream>
#include "network.h"
#include "Game.h"

using namespace std;

int main() {
    cout << "Starting the game" << endl;
    Network network;
    Game game(network.recvPositionData(1));

    while (game.IsWindowOpen() == false) {
        game.DrawGame();
        network.sendData(game.GetPlayer().GetY(), game.GetBall());
        network.recvData(3, game.GetBall(), game.GetEnemy());
    }
    return 0;
}