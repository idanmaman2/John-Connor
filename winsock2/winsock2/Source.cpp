#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <WS2tcpip.h>
#include <iostream>
#include<stdio.h>
#include<windows.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;
enum names { RUN, PONG, SAVE, VERSION, UPDATE, ELSE };
int   maps(char* a)
{
	if (strlen(a) < 3)
		return ELSE;
	if (!strcmp(a, "run") || !strcmp(a, "RUN"))
		return RUN;
	if (!strcmp(a, "ping") || !strcmp(a, "PING"))
		return PONG;
	if (!strcmp(a, "version") || !strcmp(a, "VERSION"))
		return VERSION;
	if (!strcmp(a, "update") || !strcmp(a, "UPDATE"))
		return UPDATE;
	return ELSE;
}
int  getwrite(SOCKET& clientSocket, char* buf)
{
	int bytesReceived = recv(clientSocket, buf, 256, 0);
	if (bytesReceived == SOCKET_ERROR)
	{
		cerr << "Error in recv(). Quitting" << endl;
		return 1;
	}

	if (bytesReceived == 0)
	{
		cout << "Client disconnected " << endl;
		return  2;
	}
	buf[bytesReceived - 1] = '\0';


	// Echo message back to client
	send(clientSocket, buf, 256, 0);

	return 3;
}
void sendwrite(SOCKET& sock, char userInput[])
{
	char buf[256];
	int sendResult = send(sock, userInput, strlen(userInput)+1, 0);
	if (sendResult != SOCKET_ERROR)
	{
		// Wait for response
		ZeroMemory(buf, 256);
		int bytesReceived = recv(sock, buf, 256, 0);
		if (bytesReceived > 0)
		{
			// Echo response to console
			
		}
	}

}
void print()
{
#define print2 "d88888b d888888b d8b   db  .d8b.  db        d8888b. d8888b.  .d88b.     d88b d88888b  .o88b. d888888b   d888888b d8b   db    .d88b.  .d8888.   \n88'       `88'   888o  88 d8' `8b 88        88  `8D 88  `8D .8P  Y8.    `8P' 88'     d8P  Y8 `~~88~~'     `88'   888o  88   .8P  Y8. 88'  YP   \n88ooo      88    88V8o 88 88ooo88 88        88oodD' 88oobY' 88    88     88  88ooooo 8P         88         88    88V8o 88   88    88 `8bo.     \n88~~~      88    88 V8o88 88~~~88 88        88~~~   88`8b   88    88     88  88~~~~~ 8b         88         88    88 V8o88   88    88   `Y8b.   \n88        .88.   88  V888 88   88 88booo.   88      88 `88. `8b  d8' db. 88  88.     Y8b  d8    88        .88.   88  V888   `8b  d8' db   8D   \nYP      Y888888P VP   V8P YP   YP Y88888P   88      88   YD  `Y88P'  Y8888P  Y88888P  `Y88P'    YP      Y888888P VP   V8P    `Y88P'  `8888Y'   \n                                                                                                                                               \n                                                                                                                                               \n"
	#define print1  "\n\n\n\n\n\n\a\t\t\td888888b   d8888b.   db   db   .88b  d88.           dD    .o88b.   Cb       \n\a\t\t\t  `88'     88  `8D   88   88   88'YbdP`88         d8'    d8P  Y8    `8b     \n\a\t\t\t   88      88   88   88ooo88   88  88  88        d8      8P           8b    \n\a\t\t\t   88      88   88   88~~~88   88  88  88       C88      8b           88D   \n\a\t\t\t  .88.     88  .8D   88   88   88  88  88        V8      Y8b  d8      8P    \n\a\t\t\tY888888P   Y8888D'   YP   YP   YP  YP  YP         V8.     `Y88P'    .8P     \n\a\t\t\t                                                    VD             CP       \n\a\t\t\t                                                                            \n\a\t\t"
	system("color 0A"); 

	printf(print1);
	Sleep(5000); 
	system("CLS");
	for (int i = 0; i < 20; i++)
	{
		printf(print1);
		Sleep(50);
		system("CLS");
		Sleep(50);
	} 
	printf("\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\tTHE FINAL PROJECT IN OS");
	Sleep(2000); 
	system("CLS"); 

} 
void main()
{
	print(); 
	string ipAddress = "127.0.0.1";			// IP Address of the server
	int port = 54000;						// Listening port # on the server

	// Initialize WinSock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "Can't start Winsock, Err #" << wsResult << endl;
		return;
	}

	// Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cerr << "Can't create socket, Err #" << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}

	// Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	// Connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return;
	}

	// Do-while loop to send and receive data
	char buf[4096];
	string userInput;
	char get[256];
	do
	{
		// Prompt the user for some text
		printf("READY >>");
		scanf("%s",get);
	
	
		sendwrite(sock, get);
		if (maps(get)==RUN)
		{
			CHAR buf[256];
			fgets(buf, 2, stdin);
			fgets(buf, 256, stdin);
		
			buf[strlen(buf) - 1] = '\0';
		
			sendwrite(sock,buf); 
		}
		else if (maps(get) == PONG)
		{
			getwrite(sock, get);
			printf("%s\n", get);
			 
		}
		else if (maps(get) == VERSION)
		{
			getwrite(sock, get);
			printf("%s\n", get);

		}
		else if (maps(get) == UPDATE)
		{

		
			CHAR buf[256];


			fgets(buf, 256, stdin);
			CHAR* buffer;
			DWORD len;
			DWORD beg = 0, end = 0;
			buf[strlen(buf) - 1] = '\0';
			HANDLE file = CreateFileA(buf, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	
			cout << buf << endl;
			len = GetFileSize(
				file,
				NULL
			);

			printf("%d", len);
			buffer = (CHAR*)malloc(sizeof(CHAR) * len + 1);
			ZeroMemory(buffer, len + 1);
			INT counter = 0;
			ReadFile(file, buffer, len, NULL, NULL);
			sprintf_s(buf, "%d", len);
			puts(buf);

		
			for (int i = 0; i < len; i++)
			{
				cout << buffer[i];
			}
			buffer[strlen(buffer) + 1] = '\0';
			cout << len << endl;
			sendwrite(sock, buf);
			 send(sock, buffer, len , 0);
			CloseHandle(file);
			free(buffer);
			system("CLS"); 
			
			
			

		}

		} while (strcmp(get,"exit"));

		// Gracefully close down everything
		closesocket(sock);
		WSACleanup();
	}
