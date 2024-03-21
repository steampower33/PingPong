#include "Network.h"

int main() {
    Network network;
    while (1)
    {
        network.AcceptClient();
        network.SendPositionToClient();
        network.ConnectClntSockToCP();
        network.RecvData();
    }
}