#include "stdafx.h"
#include "windows.h"
#include <string>
#include "string.h"
#include <iostream>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{	setlocale(LC_CTYPE, "Russian");
	
	cout << "ClientNP" << endl << endl;
	
	try
	{
		HANDLE h_NP;

		char rbuf[50];
		char wbuf[50] = "Hello from client ";
		DWORD rb = sizeof(rbuf);
		DWORD wb = sizeof(wbuf);

		for(int i=0; i<10; i++)
		{
			if((h_NP = CreateFile(L"\\\\.\\pipe\\Tube", GENERIC_WRITE|GENERIC_READ, FILE_SHARE_WRITE|FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL)) == INVALID_HANDLE_VALUE)
				throw "Error: CreateFile";
			
			char inum[6];
			sprintf(inum, "%d", i+1);
			char buf[sizeof(wbuf)+sizeof(inum)];
			strcpy(buf, wbuf);
			strcat(buf, inum);
			wb = sizeof(wbuf);

			if(!WriteFile(h_NP, buf, sizeof(wbuf), &wb, NULL))
				throw "Error: WriteFile";
			if(!ReadFile(h_NP, rbuf, sizeof(rbuf), &rb, NULL))
				throw "Error: ReadFile";
			
			cout << rbuf << endl;
			
			CloseHandle(h_NP);
		}
		
		char chr[] = " ";

		if((h_NP = CreateFile(L"\\\\.\\pipe\\Tube", GENERIC_WRITE|GENERIC_READ, FILE_SHARE_WRITE|FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL)) == INVALID_HANDLE_VALUE)
			throw "Error: CreateFile";
		if(!WriteFile(h_NP, chr, sizeof(chr), &wb, NULL))
			throw "Error: WriteFile";
		if(!ReadFile(h_NP, rbuf, sizeof(rbuf), &rb, NULL))
			throw "Error: ReadFile";
		
		CloseHandle(h_NP);
	}
	catch(string e)
	{
		cout << e << endl;
	}
	catch(...)
	{
		cout << "Error" << endl;
	}

	return 0;
}