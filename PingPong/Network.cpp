#include "Network.h"

int Network::numPlayers = 0;
Network::PlayerInfo Network::players[MAX_PLAYERS];
std::mutex Network::playersMutex;
std::mutex Network::ballMutex;

Network::Network() {
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // 소켓 생성
    listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // 바인딩
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);
    bind(listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));

    // 수신 대기
    listen(listenSocket, SOMAXCONN);
    std::cout << "Server started on port " << PORT << std::endl;
}

Network::~Network() {
    // 소켓 해제
    closesocket(listenSocket);
    WSACleanup();
}

void Network::AcceptPlayer() {
    char isLeftRight[1];
    int leftRight = 0;
    while (true) {
        // 플레이어 연결 수락
        sockaddr_in clientAddr;
        int clientAddrSize = sizeof(clientAddr);
        SOCKET clientSocket = accept(listenSocket, (sockaddr*)&clientAddr, &clientAddrSize);

        // 최대 플레이어 수 초과 시 거부
        if (numPlayers >= MAX_PLAYERS) {
            closesocket(clientSocket);
            continue;
        }

        // 플레이어 정보 저장
        players[numPlayers].socket = clientSocket;
        players[numPlayers].playerID = numPlayers;

        isLeftRight[0] = '0' + leftRight++;
        send(players[numPlayers].socket, isLeftRight, 1, 0);

        // 스레드 생성
        _beginthreadex(NULL, 0, &PlayerThread, &players[numPlayers], 0, NULL);
        numPlayers++;

        std::cout << "New client connected!" << std::endl;
    }
}

unsigned __stdcall Network::PlayerThread(void* data) {
    PlayerInfo* player = (PlayerInfo*)data;
    char buffer[10];
    int bytesReceived;
    Ball ball;

    while (true) {

        // 플레이어로부터 데이터 받기
        bytesReceived = recv(player->socket, buffer, 4, 0);
        if (bytesReceived <= 0) {
            // 연결 끊김 처리
            break;
        }

        if (buffer[3] - '0' == 1)
            ball.SetSpeedX(ball.GetSpeedX() * -1);

        ball.Update();
        
        int x = ball.GetX();
        int y = ball.GetY();

        for (int i = 0; i < 4; i++) {
            buffer[6 - i] = '0' + x % 10;
            x /= 10;
            if (i != 3) {
                buffer[9 - i] = '0' + y % 10;
                y /= 10;
            }
        }
        std::cout << player->playerID << ": ";
        for (int i = 0; i < 10; i++) {
            std::cout << buffer[i];
        }
        std::cout << std::endl;

        // 플레이어 정보를 모든 클라이언트에 전송
        // 상대방 플레이어 클라이언트 -> 지금 플레이어 위치를 갱신해서 전달
        {
            std::lock_guard<std::mutex> lock(playersMutex);

            // 상대방 플레이어에게 데이터 전송
            int otherIndex = (player->playerID == 0) ? 1 : 0;
            send(player[otherIndex].socket, buffer, 10, 0);
        }
    }

    // 플레이어 연결 종료
    closesocket(player->socket);
    players[player->playerID].socket = INVALID_SOCKET;
    numPlayers--;

    _endthreadex(0);
    return 0;
}
