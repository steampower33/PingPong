#include <iostream>
#include "network.h"
#include "Game.h"

using namespace std;

int main() {
    cout << "Starting the game" << endl;
    Network network;
    Game game(static_cast<int>(network.recvData(1)));

    while (game.IsWindowOpen() == false) {
        game.DrawGame();
        network.sendData(game.GetPlayer().GetY());
        game.GetEnemy().Update(network.recvData(3));
    }
    return 0;
}