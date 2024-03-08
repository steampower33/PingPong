#include "Network.h"

Network::Network() {
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // ���� ����
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // ���� �ּ� ����
    serverAddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);
    serverAddr.sin_port = htons(SERVER_PORT);

    // ������ ����
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
    // �����κ��� ������ �ޱ�
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
            // ���ڰ� �ƴ� ���ڰ� ���ԵǾ� ������ 0�� ��ȯ
            return 0;
        }
    }
    std::cout << number << std::endl;
    return number;
}

Network::~Network() {
    // ���� ����
    closesocket(clientSocket);
    WSACleanup();
}
