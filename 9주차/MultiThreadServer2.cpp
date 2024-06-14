#pragma comment(lib,"ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include<WinSock2.h>
#include<stdlib.h>
#include<iostream>
#include<tchar.h>

#define BUFFERSIZE 512
#define PORT 9000


class ServerAgent
{
private:
	//For Socket
	USHORT SERVERPORT;
	WSADATA Wsadata;		//Initiate WinSock
	SOCKET Listen_Socket;
	SOCKADDR_IN ServerAddress;

	//Variable for Data Communication
	SOCKET clientSocket;
	SOCKADDR_IN clientAddress;
	INT AddressLen;
	TCHAR Buffer[BUFFERSIZE + 1];
public:
	ServerAgent();
	~ServerAgent();
	VOID error_Quit(TCHAR *Msg);
	VOID error_Display(TCHAR *Msg);
	VOID setReadyState();
	VOID communicate();
	static DWORD WINAPI SocketThread(LPVOID lpParam);
};

ServerAgent::ServerAgent()
{
	SERVERPORT = PORT;
	if (WSAStartup(MAKEWORD(2, 2), &Wsadata) != 0)
		return;
}
ServerAgent::~ServerAgent()
{
	//CloseSocket()
	closesocket(Listen_Socket);

	//WIndsock Quit
	WSACleanup();
}
//Displaying Socket Error
VOID ServerAgent::error_Quit(TCHAR *Msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), (LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, Msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

//Displaying Socket Function Error
VOID ServerAgent::error_Display(TCHAR *Msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	std::wcout << "[" << Msg << "]" << (TCHAR *)lpMsgBuf;
	LocalFree(lpMsgBuf);
}

VOID ServerAgent::setReadyState()
{
	INT Retval;

	//Socket()
	Listen_Socket = socket(AF_INET, SOCK_STREAM, 0);
	if (Listen_Socket == INVALID_SOCKET)
		error_Quit(_T("Socket()"));

	//bind()
	ZeroMemory(&ServerAddress, sizeof(SOCKADDR_IN));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	ServerAddress.sin_port = htons(SERVERPORT);
	Retval = bind(Listen_Socket, (SOCKADDR *)&ServerAddress, sizeof(ServerAddress));
	if (Retval == SOCKET_ERROR)
		error_Quit(_T("Bind()"));

	//listen()
	Retval = listen(Listen_Socket, SOMAXCONN);
	if (Retval == SOCKET_ERROR)
		error_Quit(_T("listen()"));
}

VOID ServerAgent::communicate()
{
	HANDLE hThread;

	while (1)
	{
		//Accept()
		AddressLen = sizeof(clientAddress);
		clientSocket = accept(Listen_Socket, (SOCKADDR *)&clientAddress, &AddressLen);
		if (clientSocket == INVALID_SOCKET)
		{
			error_Display(_T("Accept"));
			break;
		}

		//Displaying Client Display
		std::wcout << std::endl << _T("[TCP Server] Client Connected : IP Address = ") << inet_ntoa(clientAddress.sin_addr) << _T(", Port = ") << ntohs(clientAddress.sin_port) << std::endl;

		//Create Thread
		hThread = CreateThread(NULL, 0, SocketThread, this, 0, NULL);
		if (hThread == NULL)
			closesocket(clientSocket);
		else
			CloseHandle(hThread);
	}
}

DWORD WINAPI ServerAgent::SocketThread(LPVOID lpParam)
{
	ServerAgent *This = (ServerAgent *)lpParam;
	SOCKET sock = This->clientSocket;
	INT retval;
	INT addressLen;
	SOCKADDR_IN threadSocketAddress;
	TCHAR Buffer[BUFFERSIZE + 1];

	//Get Client Information
	addressLen = sizeof(SOCKADDR_IN);
	getpeername(sock, (SOCKADDR *)&threadSocketAddress, &addressLen);

	while (1)
	{
		//Receving Data
		retval = recv(sock, (CHAR *)Buffer, BUFFERSIZE, 0);
		if (retval == SOCKET_ERROR)
		{
			This->error_Display(_T("recv()"));
			break;
		}
		else if (retval == 0)
			break;

		//Displaying Receiving Data
		Buffer[retval / 2] = _T('\0');
		std::wcout << _T("[TCP/") << inet_ntoa(threadSocketAddress.sin_addr) << _T(":") << ntohs(threadSocketAddress.sin_port) << _T("  ") << Buffer << std::endl;


	}
	closesocket(sock);
	std::wcout << _T("[TCP Server] Client Disconnected : IP Address=") << inet_ntoa(threadSocketAddress.sin_addr) << _T("PORT = ") << ntohs(threadSocketAddress.sin_port) << std::endl;

	return 1;
}

INT _tmain(INT argc, TCHAR *argv[])
{
	ServerAgent Server;
	Server.setReadyState();
	Server.communicate();

	return 0;
}