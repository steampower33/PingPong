#include "Network.h"

Network::Network() {
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // 소켓 생성
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // 서버 주소 설정
    serverAddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);
    serverAddr.sin_port = htons(SERVER_PORT);

    // 서버에 연결
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cout << "Failed to connect to the server." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        exit(1);
    }

    std::cout << "Connected to the server!" << std::endl;
}

void Network::sendData(int playerY, Ball& ball) {
    char buffer[4];

    for (int i = 0; i < 4; i++)
    {
        if (i != 3) {
            buffer[2 - i] = '0' + playerY % 10;
            playerY /= 10;
        }
    }
    buffer[3] = '0' + ball.GetCollisionDetected();

    send(clientSocket, buffer, 4, 0);
}

int Network::recvPositionData(int dataSize) {
    // 서버로부터 상대 바 위치 받기
    bytesReceived = recv(clientSocket, buffer, dataSize, 0);
    if (bytesReceived <= 0) {
        std::cout << "Connection closed by the server." << std::endl;
        exit(1);
    }

    int position = buffer[0] - '0';
    return position;
}

void Network::recvData(int dataSize, Ball& ball, Enemy& enemy) {
    bytesReceived = recv(clientSocket, buffer, dataSize, 0);
    for (int i = 0; i < 10; i++) {
        std::cout << buffer[i];
    }
    std::cout << std::endl;
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
    std::cout << "enemyY: " << enemyY << "ballX: " << ballX << "ballY:" << ballY << std::endl;
}

Network::~Network() {
    // 소켓 해제
    closesocket(clientSocket);
    WSACleanup();
}
