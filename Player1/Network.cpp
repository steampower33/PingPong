#include "Network.h"

Network::Network() : playerY(340), ballX(0), ballY(0), playerPos(0) {
    // WinSock 초기화
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("WSAStartup failed.");

    // 소켓 생성
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET)
        ErrorHandling("Failed to create socket.");

    // 서버 주소 설정
    serverAddr.sin_family = AF_INET;
    if (inet_pton(AF_INET, kServerIp, &serverAddr.sin_addr) != 1)
        ErrorHandling("Invalid address format.");
    serverAddr.sin_port = htons(kServerPort);

    // 서버에 연결
    if (connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR)
        ErrorHandling("Failed to connect to the server.");

    std::cout << "Connected to the server!" << std::endl;
}

int Network::recvPositionData(int dataSize) {
    bytesReceived = recv(clientSocket, buffer, dataSize, 0);
    if (bytesReceived <= 0)
        ErrorHandling("Connection closed by the server.");

    int position = buffer[0] - '0';
    playerPos = position;
    return position;
}

void Network::sendData(int playerY, Ball& ball) {
    char buffer[4];

    buffer[0] = '0' + playerPos;
    for (int i = 0; i < 3; i++)
    {
        buffer[3 - i] = '0' + playerY % 10;
        playerY /= 10;
    }
    send(clientSocket, buffer, 4, 0);
}

void Network::recvData(int dataSize, Ball& ball, Enemy& enemy) {
    bytesReceived = recv(clientSocket, buffer, dataSize, 0);
    if (bytesReceived <= 0)
        ErrorHandling("Connection closed by the server.");

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
    // 소켓 해제
    closesocket(clientSocket);
    WSACleanup();
}

void Network::ErrorHandling(const char* buf)
{
    std::cerr << buf << std::endl;
    exit(EXIT_FAILURE);
}