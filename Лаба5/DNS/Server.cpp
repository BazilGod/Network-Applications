#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <time.h>
#include "Error.h"
#include <Windows.h>
#pragma comment(lib, "WS2_32.lib")

//функция предназначена для перессылки позывного сервера программе клиента
//name - позывной сервера
//to - указатель на SOCKADDR_IN
//from - указатель на размер from
bool PutAnswerToClient(char* name, SOCKET sock, sockaddr* to, int* lto)
{
	if (sendto(sock, name, strlen(name) + 1, NULL, to, *lto) == SOCKET_ERROR)
	{
		throw SetErrorMsgText("sendto: ", WSAGetLastError());
	}
	return true;
}

bool GetRequestFromClient(char* name, SOCKET sock, sockaddr* from, int* flen)
{
	char ibuf[50];
	cout << "Wait message..." << endl;
	while (true)
	{
		if (recvfrom(sock, ibuf, sizeof(ibuf), NULL, from, flen) == SOCKET_ERROR)
		{
			if (WSAGetLastError() == WSAETIMEDOUT)
				return false;
			else
				throw SetErrorMsgText("recvfrom: ", WSAGetLastError());
		}
		if (strcmp(ibuf, name) == 0)
		{
			cout << "Client socket -> ";
			cout << inet_ntoa(((SOCKADDR_IN*)from)->sin_addr) << ": " << htons(((SOCKADDR_IN*)from)->sin_port) << endl;
			return true;
		}
		else
		{
			return false;
		}
	}
}



void main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	WSADATA wsaData;
	SOCKET sockServer;
	hostent* server;
	hostent* client;
	char name[] = "hello";
	char ans[] = "Access";
	char hostname[32];
	try
	{
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
		{
			throw SetErrorMsgText("Startup: ", WSAGetLastError());
		}
		cout << "Search server..." << endl;
		if ((sockServer = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
		{
			throw SetErrorMsgText("Socket: ", WSAGetLastError());
		}
		SOCKADDR_IN serv;
		serv.sin_family = AF_INET;
		serv.sin_port = htons(2000);
		serv.sin_addr.s_addr = INADDR_ANY;
		if (bind(sockServer, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
		{
			throw SetErrorMsgText("bind: ", WSAGetLastError());
		}
		SOCKADDR_IN from;
		memset(&from, 0, sizeof(from));
		int lfrom = sizeof(from);
		gethostname(hostname, sizeof(hostname));
		cout << "Server "<< hostname << " start" << endl;
		while (true)
		{
			if (GetRequestFromClient(name, sockServer, (sockaddr*)&from, &lfrom))
			{
				cout << "Succes" << endl;
				cout << "Client: " << gethostbyaddr((char*)&from.sin_addr, sizeof(from.sin_addr), AF_INET) -> h_name <<endl;
				PutAnswerToClient(ans, sockServer, (sockaddr*)&from, &lfrom);
			}
			else
				cout << "Error" << endl;
		}
		if (closesocket(sockServer) == SOCKET_ERROR)
		{
			throw SetErrorMsgText("closesocket: ", WSAGetLastError());
		}

		if (WSACleanup() == SOCKET_ERROR)
		{
			throw SetErrorMsgText("Cleanup: ", WSAGetLastError());
		}
	}
	catch (string errorMsgText)
	{
		cout << "WSAGetLastError: " << errorMsgText << endl;
	}
	system("pause");
}