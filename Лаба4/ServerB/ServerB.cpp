#include "stdafx.h"
#include <iostream>
#include <string>
#include <time.h>
#include "Winsock2.h"

#pragma comment(lib, "WS2_32.lib")

SOCKET sS;

std::string  GetErrorMsgText(int code)
{
	switch (code)
	{
	case WSAEINTR: return "������ ������� �������� ";
	case WSAEACCES: return "���������� ����������";
	case WSAEFAULT:	return "��������� �����";
	case WSAEINVAL:	return "������ � ���������";
	case WSAEMFILE: return "������� ����� ������ �������";
	case WSAEWOULDBLOCK: return "������ �������� ����������";
	case WSAEINPROGRESS: return "�������� � �������� ��������";
	case WSAEALREADY: return "�������� ��� �����������";
	case WSAENOTSOCK: return "����� ����� �����������";
	case WSAEDESTADDRREQ: return "��������� ����� ������������";
	case WSAEMSGSIZE: return "��������� ������� �������";
	case WSAEPROTOTYPE: return "������������ ��� ��������� ��� ������";
	case WSAENOPROTOOPT: return "������ � ����� ���������";
	case WSAEPROTONOSUPPORT: return "�������� �� ��������������";
	case WSAESOCKTNOSUPPORT: return "��� ������ �� ��������������";
	case WSAEOPNOTSUPP:	return "�������� �� ��������������";
	case WSAEPFNOSUPPORT: return "��� ���������� �� ��������������";
	case WSAEAFNOSUPPORT: return "��� ������� �� �������������� ����������";
	case WSAEADDRINUSE:	return "����� ��� ������������";
	case WSAEADDRNOTAVAIL: return "����������� ����� �� ����� ���� �����������";
	case WSAENETDOWN: return "���� ���������";
	case WSAENETUNREACH: return "���� �� ���������";
	case WSAENETRESET: return "���� ��������� ����������";
	case WSAECONNABORTED: return "����������� ����� �����";
	case WSAECONNRESET:	return "����� �������������";
	case WSAENOBUFS: return "�� ������� ������ ��� �������";
	case WSAEISCONN: return "����� ��� ���������";
	case WSAENOTCONN: return "����� �� ���������";
	case WSAESHUTDOWN: return "������ ��������� send : ����� �������� ������";
	case WSAETIMEDOUT: return "���������� ���������� �������� �������";
	case WSAECONNREFUSED: return "���������� ���������";
	case WSAEHOSTDOWN: return "���� � ����������������� ���������";
	case WSAEHOSTUNREACH: return "��� �������� ��� �����";
	case WSAEPROCLIM: return "������� ����� ���������";
	case WSASYSNOTREADY: return "���� �� ��������";
	case WSAVERNOTSUPPORTED: return "������ ������ ����������";
	case WSANOTINITIALISED:	return "�� ��������� ������������� WS2_32.DLL";
	case WSAEDISCON: return "����������� ����������";
	case WSATYPE_NOT_FOUND: return "����� �� ������";
	case WSAHOST_NOT_FOUND:	return "���� �� ������";
	case WSATRY_AGAIN: return "������������������ ���� �� ������";
	case WSANO_RECOVERY: return "��������������  ������";
	case WSANO_DATA: return "��� ������ ������������ ����";
	case WSA_INVALID_HANDLE: return "��������� ���������� �������  � �������";
	case WSA_INVALID_PARAMETER: return "���� ��� ����� ���������� � �������";
	case WSA_IO_INCOMPLETE:	return "������ ����� - ������ �� � ���������� ���������";
	case WSA_IO_PENDING: return "�������� ���������� �����";
	case WSA_NOT_ENOUGH_MEMORY: return "�� ���������� ������";
	case WSA_OPERATION_ABORTED: return "�������� ����������";
	case WSASYSCALLFAILURE: return "��������� ���������� ���������� ������";
	default: return "**ERROR**";
	};
};

std::string  SetErrorMsgText(std::string msgText, int code)
{
	return  msgText + GetErrorMsgText(code);
};

bool GetRequestFromClient(char *name, short port, sockaddr* from, int* flen)
{
	char nameServer[50];
	memset(from, 0, sizeof(flen));

	if ((sS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)//��� �������� ��������� ipv4, ������������ �����
	{
		throw  SetErrorMsgText("socket:", WSAGetLastError());
	}

	SOCKADDR_IN serv;
	serv.sin_family = AF_INET;
	serv.sin_port = htons(port);
	serv.sin_addr.s_addr = INADDR_ANY;

	if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
	{
		throw  SetErrorMsgText("bind:", WSAGetLastError());
	}

	if ((recvfrom(sS, nameServer, sizeof(nameServer), NULL, from, flen)) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAETIMEDOUT)
		{
			return false;
		}
		else
		{
			throw  SetErrorMsgText("recv:", WSAGetLastError());
		}
	}

	if (!strcmp(nameServer, name))
	{
		std::cout << std::endl << "�������� ������� ���������.";
		return true;
	}
	else
	{
		std::cout << std::endl << "�������� ������� �� ���������.";
		return false;
	}
}

bool PutAnswerToClient(char* name, sockaddr* to, int* lto)
{
	if ((sendto(sS, name, strlen(name) + 1, NULL, to, *lto)) == SOCKET_ERROR)
	{
		throw  SetErrorMsgText("send:", WSAGetLastError());
	}

	return true;
}

int main(int argc, _TCHAR* argv[])
{
	setlocale(LC_CTYPE, "rus");

	WSADATA wsaData;

	try
	{
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
		{
			throw  SetErrorMsgText("Startup:", WSAGetLastError());
		}

		char name[] = "Hello";

		for (;;)
		{
			SOCKADDR_IN clnt;
			int lc = sizeof(clnt);

			if (GetRequestFromClient(name, 2000, (sockaddr*)&clnt, &lc))
			{
				std::cout << std::endl << PutAnswerToClient(name, (sockaddr*)&clnt, &lc);
			}
			else
			{
				std::cout << std::endl << PutAnswerToClient("**Eror name**", (sockaddr*)&clnt, &lc);
			}

			SOCKADDR_IN* addr = (SOCKADDR_IN*)&clnt;
			std::cout << std::endl << "���� �������: " << addr->sin_port;
			std::cout << std::endl << "IP-����� �������: " << inet_ntoa(addr->sin_addr);

			if (closesocket(sS) == SOCKET_ERROR)
			{
				throw  SetErrorMsgText("closesocket:", WSAGetLastError());
			}
		}

		if (WSACleanup() == SOCKET_ERROR)
		{
			throw  SetErrorMsgText("Cleanup:", WSAGetLastError());
		}
	}
	catch (std::string errorMsgText)
	{
		std::cout << std::endl << errorMsgText;
	}

	std::cout << std::endl;
	system("pause");
	return 0;
}