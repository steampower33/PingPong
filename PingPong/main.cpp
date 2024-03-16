#include "Network.h"
#include "Ball.h"

int main() {
    Network network;
    Ball ball;
    while (1)
    {
        network.HandleEvent(ball);
        ball.Update(network.GetPosY1(), network.GetPosY2());
    }
    return 0;
}