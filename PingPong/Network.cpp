#include "Network.h"

Network::Network() {
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(kPort);

	if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");

	if (listen(hServSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");

	newEvent = WSACreateEvent();
	if (WSAEventSelect(hServSock, newEvent, FD_ACCEPT) == SOCKET_ERROR)
		ErrorHandling("WSAEventSelect() error");

	hSockArr[numOfClntSock] = hServSock;
	hEventArr[numOfClntSock] = newEvent;
	numOfClntSock++;
}

Network::~Network() {
	WSACleanup();
}

void Network::HandleEvent(Ball& ball) {
	posInfo = WSAWaitForMultipleEvents(
		numOfClntSock, hEventArr, FALSE, WSA_INFINITE, FALSE);
	startIdx = posInfo - WSA_WAIT_EVENT_0;

	for (i = startIdx; i < numOfClntSock; i++)
	{
		int sigEventIdx =
			WSAWaitForMultipleEvents(1, &hEventArr[i], TRUE, 0, FALSE);
		if ((sigEventIdx == WSA_WAIT_FAILED || sigEventIdx == WSA_WAIT_TIMEOUT))
		{
			continue;
		}
		else
		{
			sigEventIdx = i;
			WSAEnumNetworkEvents(
				hSockArr[sigEventIdx], hEventArr[sigEventIdx], &netEvents);

			if (EventAccept(sigEventIdx) || EventRead(sigEventIdx, ball) || EventClose(sigEventIdx))
				break;
		}
	}
}

int Network::EventAccept(int sigEventIdx)
{
	if (netEvents.lNetworkEvents & FD_ACCEPT)
	{
		if (netEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
		{
			std::cout << "Accept Error" << std::endl;
			return 1;
		}
		clntAdrLen = sizeof(clntAdr);
		hClntSock = accept(
			hSockArr[sigEventIdx], (SOCKADDR*)&clntAdr, &clntAdrLen);
		newEvent = WSACreateEvent();
		WSAEventSelect(hClntSock, newEvent, FD_READ | FD_CLOSE);

		hEventArr[numOfClntSock] = newEvent;
		hSockArr[numOfClntSock] = hClntSock;

		char p[1];
		p[0] = '0' + numOfClntSock;
		send(hSockArr[numOfClntSock], p, 1, 0);
		numOfClntSock++;
		std::cout << "connected new client..." << std::endl;
		return 0;
	}
}

int Network::EventRead(int sigEventIdx, Ball& ball)
{
	if (netEvents.lNetworkEvents & FD_READ)
	{
		if (netEvents.iErrorCode[FD_READ_BIT] != 0)
		{
			std::cout << "Read Error" << std::endl;
			return 1;
		}

		recv(hSockArr[sigEventIdx], buf, 3, 0);

		int recvValue = 0;
		for (int i = 0; i < 3; i++)
		{
			recvValue = recvValue * 10 + (buf[i] - '0');
		}
		pos[sigEventIdx] = recvValue;

		int enemyY = (sigEventIdx == 1) ? pos[2] : pos[1];
		int ballX = ball.GetX();
		int ballY = ball.GetY();
		for (int i = 0; i < 4; i++)
		{
			if (i != 3)
			{
				buf[2 - i] = '0' + enemyY % 10;
				enemyY /= 10;
				buf[9 - i] = '0' + ballY % 10;
				ballY /= 10;
			}
			buf[6 - i] = '0' + ballX % 10;
			ballX /= 10;
		}
		buf[10] = '0' + ball.GetLeftScore();
		buf[11] = '0' + ball.GetRightScore();
		buf[12] = (ball.IsGameOver()) ? '1' : '0';
		send(hSockArr[sigEventIdx], buf, 12, 0);
		return 0;
	}
}

int Network::EventClose(int sigEventIdx)
{
	if (netEvents.lNetworkEvents & FD_CLOSE)
	{
		if (netEvents.iErrorCode[FD_CLOSE_BIT] != 0)
		{
			std::cout << "Close Error" << std::endl;
			return 1;
		}
		WSACloseEvent(hEventArr[sigEventIdx]);
		closesocket(hSockArr[sigEventIdx]);

		std::cout << "Client " << sigEventIdx << " Closed" << std::endl;
		numOfClntSock--;
		CompressSockets(hSockArr, sigEventIdx, numOfClntSock);
		CompressEvents(hEventArr, sigEventIdx, numOfClntSock);
		return 0;
	}
}

void Network::CompressSockets(SOCKET hSockArr[], int idx, int total)
{
	for (int i = idx; i < total; i++)
		hSockArr[i] = hSockArr[i + 1];
}
void Network::CompressEvents(WSAEVENT hEventArr[], int idx, int total)
{
	for (int i = idx; i < total; i++)
		hEventArr[i] = hEventArr[i + 1];
}
void Network::ErrorHandling(const char* buf)
{
	std::cerr << buf << std::endl;
	exit(EXIT_FAILURE);
}

int Network::GetPosY1()
{
	return pos[1];
}

int Network::GetPosY2()
{
	return pos[2];
}