#include "Network.h"

Network::Network() {
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // 家南 积己
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // 辑滚 林家 汲沥
    serverAddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);
    serverAddr.sin_port = htons(SERVER_PORT);

    // 辑滚俊 楷搬
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cout << "Failed to connect to the server." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        exit(1);
    }

    std::cout << "Connected to the server!" << std::endl;
}

int Network::recvPositionData(int dataSize) {
    bytesReceived = recv(clientSocket, buffer, dataSize, 0);
    if (bytesReceived <= 0) {
        std::cout << "Connection closed by the server." << std::endl;
        exit(1);
    }

    int position = buffer[0] - '0';
    return position;
}

void Network::sendData(int playerY, Ball& ball) {
    char buffer[3];

    for (int i = 0; i < 4; i++)
    {
        if (i != 3) {
            buffer[2 - i] = '0' + playerY % 10;
            playerY /= 10;
        }
    }
    send(clientSocket, buffer, 3, 0);
}

void Network::recvData(int dataSize, Ball& ball, Enemy& enemy) {
    bytesReceived = recv(clientSocket, buffer, dataSize, 0);
    if (bytesReceived <= 0) {
        std::cout << "Connection closed by the server." << std::endl;
        exit(1);
    }

    int enemyY = 0;
    int ballX = 0;
    int ballY = 0;
    for (int i = 0; i < 4; i++) {
        if (i != 3) {
            enemyY = enemyY * 10 + (buffer[i] - '0');
            ballY = ballY * 10 + (buffer[7 + i] - '0');
        }
        ballX = ballX * 10 + (buffer[3 + i] - '0');
    }
    enemy.SetY(enemyY);
    ball.SetX(ballX);
    ball.SetY(ballY);
    ball.SetLeftScore(buffer[10] - '0');
    ball.SetRightScore(buffer[11] - '0');
}

Network::~Network() {
    // 家南 秦力
    closesocket(clientSocket);
    WSACleanup();
}
