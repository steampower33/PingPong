#include "Network.h"

Network::Network() {
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(PORT);

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

void Network::HandleEvent() {
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
			if (netEvents.lNetworkEvents & FD_ACCEPT)
			{
				if (netEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
				{
					puts("Accept Error");
					break;
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
				puts("connected new client...");
			}

			if (netEvents.lNetworkEvents & FD_READ)
			{
				if (netEvents.iErrorCode[FD_READ_BIT] != 0)
				{
					puts("Read Error");
					break;
				}
				strLen = recv(hSockArr[sigEventIdx], msg, sizeof(msg), 0);
				int recvValue = 0;
				for (int i = 0; i < 3; i++)
					recvValue = recvValue * 10 + (msg[i] - '0');
				pos[sigEventIdx] = recvValue;

				int sendValue = (sigEventIdx == 1) ? pos[2] : pos[1];
				for (int i = 0; i < 3; i++)
				{
					msg[2 - i] = '0' + sendValue % 10;
					sendValue /= 10;
				}
				send(hSockArr[sigEventIdx], msg, strLen, 0);
			}

			if (netEvents.lNetworkEvents & FD_CLOSE)
			{
				if (netEvents.iErrorCode[FD_CLOSE_BIT] != 0)
				{
					puts("Close Error");
					break;
				}
				WSACloseEvent(hEventArr[sigEventIdx]);
				closesocket(hSockArr[sigEventIdx]);

				numOfClntSock--;
				CompressSockets(hSockArr, sigEventIdx, numOfClntSock);
				CompressEvents(hEventArr, sigEventIdx, numOfClntSock);
			}
		}
	}
}

void Network::CompressSockets(SOCKET hSockArr[], int idx, int total)
{
	int i;
	for (i = idx; i < total; i++)
		hSockArr[i] = hSockArr[i + 1];
}
void Network::CompressEvents(WSAEVENT hEventArr[], int idx, int total)
{
	int i;
	for (i = idx; i < total; i++)
		hEventArr[i] = hEventArr[i + 1];
}
void Network::ErrorHandling(const char* msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}