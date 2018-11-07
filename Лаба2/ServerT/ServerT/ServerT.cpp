// ServerT.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"


int main()
{
	setlocale(LC_CTYPE, "Russian");
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(2, 2);
	WSAStartup(wVersionRequested, &wsaData);
	SOCKET sS; // дескриптор сокет
	SOCKADDR_IN sockaddr_in; 
	sockaddr_in.sin_family = AF_INET;
	sockaddr_in.sin_port = htons(SERVER_PORT);
	sockaddr_in.sin_addr.s_addr = INADDR_ANY;
	try
	{

		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
			throw SetErrorMsgText("Startup: ", WSAGetLastError());
		while (true) {
			if ((sS = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
				throw SetErrorMsgText("Socket: ", WSAGetLastError());
			//------
			if (bind(sS, reinterpret_cast<LPSOCKADDR>(&sockaddr_in), sizeof(sockaddr_in)) == SOCKET_ERROR)
				throw SetErrorMsgText("Bind:", WSAGetLastError());
			if (listen(sS, SOMAXCONN) == SOCKET_ERROR)
				throw SetErrorMsgText("Listen:", WSAGetLastError());
			//--------
			SOCKET cS; //сокет обмена данными с клиентом
			SOCKADDR_IN clnt;
			memset(&clnt, 0, sizeof(clnt));
			int lclnt = sizeof(clnt);
			std::cout << "The server waits for clients." << std::endl;
			if ((cS = accept(sS, (sockaddr*)&clnt, &lclnt)) == INVALID_SOCKET)
				throw SetErrorMsgText("accept:", WSAGetLastError());
			//=--------
			char ibuf[30];
			int libuf = 0,
				lobuf = 0;
			std::cout << "Client " << inet_ntoa(clnt.sin_addr) << ":" << clnt.sin_port << " conneced." << std::endl;
			clock_t start = clock();
			while (true) {
				memset(&ibuf, 0, sizeof ibuf);
				if ((libuf = recv(cS, ibuf, sizeof(ibuf), NULL)) == SOCKET_ERROR)
					throw SetErrorMsgText("recv:", WSAGetLastError());
				std::cout << ibuf << std::endl;
				if (strlen(ibuf) != 0) {
					if ((libuf = send(cS, ibuf, sizeof(ibuf), NULL)) == SOCKET_ERROR)
						throw SetErrorMsgText("send:", WSAGetLastError());
					if ((libuf = recv(cS, ibuf, sizeof(ibuf), NULL)) == SOCKET_ERROR)
						throw SetErrorMsgText("recv:", WSAGetLastError());
					std::cout << ibuf << std::endl;
				}
				else break;
			}
			clock_t end = clock();
			std::cout << "Time is : " << end - start << "ms" << std::endl;
			//--------
			if (closesocket(sS) == SOCKET_ERROR)
				throw  SetErrorMsgText("Closesocket:", WSAGetLastError());
		}
		if (WSACleanup() == SOCKET_ERROR)
			throw  SetErrorMsgText("Cleanup:", WSAGetLastError());
	}
	catch (std::string errorMsgText)
	{
		std::cout << std::endl << "WSAGetLastError: " << errorMsgText;
	}
	system("pause");
	return 0;
}

