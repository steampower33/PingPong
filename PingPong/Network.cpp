#include "Network.h"

int Network::pos[2] = { 340, 340 };
Ball Network::ball;
HANDLE Network::pos_mutex;
HANDLE Network::ball_mutex;
HANDLE Network::send_buffer_mutex;
HANDLE Network::recv_buffer_mutex;

Network::Network() {
	WSADATA	wsaData;
	HANDLE hComPort;
	SYSTEM_INFO sysInfo;
	LPPER_IO_DATA ioInfo;
	LPPER_HANDLE_DATA handleInfo;
	int recvBytes, i, flags = 0;

	SOCKET hServSock;
	SOCKADDR_IN servAdr;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hComPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	GetSystemInfo(&sysInfo);

	// Create Mutex
	pos_mutex = CreateMutex(NULL, FALSE, NULL);
	ball_mutex = CreateMutex(NULL, FALSE, NULL);
	send_buffer_mutex = CreateMutex(NULL, FALSE, NULL);
	recv_buffer_mutex = CreateMutex(NULL, FALSE, NULL);

	for (i = 0; i < 1; i++)
		_beginthreadex(NULL, 0, EchoThreadMain, (LPVOID)hComPort, 0, NULL);

	hServSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(kPort);

	bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr));
	listen(hServSock, 2);

	int numOfClntSock = 0;
	while (1)
	{
		SOCKET hClntSock;
		SOCKADDR_IN clntAdr;
		int addrLen = sizeof(clntAdr);

		hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &addrLen);
		
		char p[1];
		p[0] = '0' + numOfClntSock++;
		send(hClntSock, p, 1, 0);
		std::cout << p[0] << std::endl;
		
		handleInfo = new PER_HANDLE_DATA;
		handleInfo->hClntSock = hClntSock;
		memcpy(&(handleInfo->clntAdr), &clntAdr, addrLen);

		CreateIoCompletionPort((HANDLE)hClntSock, hComPort, (ULONG_PTR)handleInfo, 0);

		ioInfo = new PER_IO_DATA;
		memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
		ioInfo->wsaBuf.len = 4;
		ioInfo->wsaBuf.buf = ioInfo->buffer;
		ioInfo->rwMode = kRead;
		WSARecv(handleInfo->hClntSock, &(ioInfo->wsaBuf),
			1, (LPDWORD)&recvBytes, (LPDWORD)&flags, &(ioInfo->overlapped), NULL);
	}
}

UINT WINAPI Network::EchoThreadMain(LPVOID pComPort)
{
	HANDLE hComPort = (HANDLE)pComPort;
	SOCKET sock;
	DWORD bytesTrans;
	LPPER_HANDLE_DATA handleInfo;
	LPPER_IO_DATA ioInfo;
	DWORD flags = 0;

	while (1)
	{
		GetQueuedCompletionStatus(hComPort, &bytesTrans,
			(PULONG_PTR)&handleInfo, (LPOVERLAPPED*)&ioInfo, INFINITE);
		sock = handleInfo->hClntSock;

		if (ioInfo->rwMode == kRead)
		{
			std::cout << "message received!" << std::endl;
			if (bytesTrans == 0)
			{
				closesocket(sock);
				free(handleInfo); free(ioInfo);
				continue;
			}
			WaitForSingleObject(recv_buffer_mutex, INFINITE);
			int position = ioInfo->buffer[0] - '0';
			int playerY = 0;
			for (int i = 1; i < 4; i++)
				playerY = playerY * 10 + (ioInfo->buffer[i] - '0');
			ReleaseMutex(recv_buffer_mutex);

			WaitForSingleObject(pos_mutex, INFINITE);
			pos[position] = playerY;
			int enemyY = (position == 0) ? pos[1] : pos[0];
			ReleaseMutex(pos_mutex);

			WaitForSingleObject(ball_mutex, INFINITE);
			int ballX = ball.GetX();
			int ballY = ball.GetY();
			ReleaseMutex(ball_mutex);

			WaitForSingleObject(send_buffer_mutex, INFINITE);
			for (int i = 0; i < 4; i++)
			{
				if (i != 3)
				{
					ioInfo->buffer[2 - i] = '0' + enemyY % 10;
					enemyY /= 10;
					ioInfo->buffer[9 - i] = '0' + ballY % 10;
					ballY /= 10;
				}
				ioInfo->buffer[6 - i] = '0' + ballX % 10;
				ballX /= 10;
			}
			ioInfo->buffer[10] = '0' + ball.GetLeftScore();
			ioInfo->buffer[11] = '0' + ball.GetRightScore();
			ioInfo->buffer[12] = '\0';
			ReleaseMutex(send_buffer_mutex);

			memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
			ioInfo->wsaBuf.len = kBufSize;
			ioInfo->rwMode = kWrite;
			WSASend(sock, &(ioInfo->wsaBuf),
				1, NULL, 0, &(ioInfo->overlapped), NULL);

			WaitForSingleObject(pos_mutex, INFINITE);
			WaitForSingleObject(ball_mutex, INFINITE);
			ball.Update(pos[0], pos[1]);
			ReleaseMutex(ball_mutex);
			ReleaseMutex(pos_mutex);

			ioInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
			memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
			ioInfo->wsaBuf.len = 4;
			ioInfo->wsaBuf.buf = ioInfo->buffer;
			ioInfo->rwMode = kRead;
			WSARecv(sock, &(ioInfo->wsaBuf),
				1, NULL, &flags, &(ioInfo->overlapped), NULL);
		}
		else
		{
			std::cout << "message send!" << std::endl;
			free(ioInfo);
		}
	}
	return 0;
}

Network::~Network() {
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