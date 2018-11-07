// ClientT.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"


int main()
{
	setlocale(LC_CTYPE, "Russian");
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(2, 2);
	WSAStartup(wVersionRequested, &wsaData);
	SOCKET cC;

	try
	{
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
			throw SetErrorMsgText("Startup: ", WSAGetLastError());
		if ((cC = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
			throw SetErrorMsgText("Socket: ", WSAGetLastError());
		//----
		SOCKADDR_IN serv;
		serv.sin_family = AF_INET;
		serv.sin_port = htons(CLIENT_PORT);
		serv.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
		if ((connect(cC, (sockaddr*)&serv, sizeof(serv))) == SOCKET_ERROR)
			throw  SetErrorMsgText("connect:", WSAGetLastError());

		//----

		int libuf = 0,
			lobuf = 0;
		int count_from_Hello = 100;
		for (int i = 0; i < count_from_Hello; i++) {
			char num[4];
			char ibuf[30],
				obuf[30] = "Hello from client ";
			_itoa(i, num, 10);
			strcat(obuf, num);
			if ((lobuf = send(cC, obuf, strlen(obuf) + 1, NULL)) == SOCKET_ERROR)
				throw  SetErrorMsgText("send:", WSAGetLastError());
			std::cout << obuf << std::endl;
			if ((recv(cC, obuf, strlen(obuf) + 1, NULL)) == SOCKET_ERROR)
				throw  SetErrorMsgText("recv:", WSAGetLastError());
			char numtwo[4];
			char Message[30] = "Hello from client ";
			_itoa(i+1, numtwo, 10);
			strcat(Message, numtwo);
			if ((lobuf = send(cC, Message, strlen(Message) + 1, NULL)) == SOCKET_ERROR)
				throw  SetErrorMsgText("send:", WSAGetLastError());
			std::cout << " " << std::endl;
			std::cout << Message << std::endl;
		}
		if ((lobuf = send(cC,"", strlen("") + 1, NULL)) == SOCKET_ERROR)
			throw  SetErrorMsgText("send:", WSAGetLastError());

		//-----
		if (closesocket(cC) == SOCKET_ERROR)
			throw  SetErrorMsgText("Closesocket:", WSAGetLastError());
		if (WSACleanup() == SOCKET_ERROR)
			throw  SetErrorMsgText("Cleanup:", WSAGetLastError());
	}

	catch (std::string errorMsgText)
	{
		std::cout << std::endl << "WSAGetLastError: " << errorMsgText;
	}

	WSACleanup();
	system("pause");
	return 0;
}