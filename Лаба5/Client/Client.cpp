#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include "time.h"
#include "Error.h"
#include <Windows.h>
#pragma comment(lib, "WS2_32.lib")



bool GetServerByName(char* call, char* hostname ,short port, sockaddr* from, int* flen)
{
	SOCKET sock;
	hostent* server;
	int optval = 1;
	if ((sock = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
	{
		throw SetErrorMsgText("socket: ", WSAGetLastError());
	}
	//setsockopt - функция предназначена для установки режимов использования сокета
	if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(int)) == SOCKET_ERROR)
		throw SetErrorMsgText("opt: ", WSAGetLastError());
	if ((server = gethostbyname(hostname)) == NULL)
		throw SetErrorMsgText("Error: server not found.", 0);
	SOCKADDR_IN all;
	all.sin_family = AF_INET;
	all.sin_port = htons(port);
	all.sin_addr.s_addr = INADDR_BROADCAST;
	all.sin_addr.s_addr = ((in_addr*)server->h_addr_list[0])->s_addr;
	char buf[] = "hello";
	if (sendto(sock, call, strlen(call) + 1, NULL, (sockaddr*)&all, sizeof(all)) == SOCKET_ERROR)
	{
		throw SetErrorMsgText("sendto: ", WSAGetLastError());
	}
	char ibuf[50];
	if (recvfrom(sock, ibuf, sizeof(ibuf), NULL, from, flen) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAETIMEDOUT)
			return false;
		else
			throw SetErrorMsgText("recvfrom: ", WSAGetLastError());
	}
	cout << ibuf << endl;
	if (closesocket(sock) == SOCKET_ERROR)
	{
		throw SetErrorMsgText("closesocket: ", WSAGetLastError());
	}
	return true;
}


void main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	WSADATA wsaData;
	char call[] = "hello";
	char hostname[] = "Bazil";
	try
	{
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
		{
			throw SetErrorMsgText("Startup: ", WSAGetLastError());
		}
		SOCKADDR_IN from;
		memset(&from, 0, sizeof(from));
		int lfrom = sizeof(from);
		GetServerByName(call, hostname, 2000, (sockaddr*)&from, &lfrom);
		cout << "Server socket -> ";
		cout << inet_ntoa(from.sin_addr) << ": " << htons(from.sin_port) << endl;
		if (WSACleanup() == SOCKET_ERROR)
		{
			throw SetErrorMsgText("Cleanup: ", WSAGetLastError());
		}
	}
	catch (string errorMsgText)
	{
		cout << "WSAGetLastError: " << errorMsgText << endl;
	}
	cout << "End" << endl;
	system("pause");
}