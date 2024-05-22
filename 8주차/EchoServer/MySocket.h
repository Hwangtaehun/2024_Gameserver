#pragma once
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#define SERVERPORT 9000
#define BUFSIZE    512

class MySocket
{
private:
protected:
	SOCKET listen_sock;
	SOCKET client_sock;
	SOCKADDR_IN serveraddr;
	SOCKADDR_IN clientaddr;
	HANDLE hThread;
	WSADATA wsa;
public:
	virtual int Init() = 0;
	virtual void My_Socket() {};
	virtual void My_Bind() {};
	virtual void My_Listen() {};
	virtual void My_Create_Thread() {};
	virtual void My_accept() {};
	virtual void My_CloseSocket() {};
	void err_quit(char* msg);
	void err_display(char* msg);
	MySocket(void);
	~MySocket(void);
};