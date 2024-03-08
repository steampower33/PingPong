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

void Network::sendData(int y) {
    char data[3];


    for (int i = 0; i < 3; i++)
    {
        data[2 - i] = '0' + y % 10;
        y /= 10;
    }

    send(clientSocket, data, 3, 0);
}

float Network::recvData(int dataSize) {
    memset(buffer, 0, dataSize);
    // 서버로부터 데이터 받기
    bytesReceived = recv(clientSocket, buffer, dataSize, 0);
    if (bytesReceived <= 0) {
        std::cout << "Connection closed by the server." << std::endl;
        return 0;
    }
    buffer[bytesReceived] = '\0';

    float number = 0;
    for (int i = 0; i < dataSize; i++) {
        if (buffer[i] >= '0' && buffer[i] <= '9') {
            number = number * 10 + (buffer[i] - '0');
        }
        else {
            // 숫자가 아닌 문자가 포함되어 있으면 0을 반환
            return 0;
        }
    }
    std::cout << number << std::endl;
    return number;
}

Network::~Network() {
    // 소켓 해제
    closesocket(clientSocket);
    WSACleanup();
}
