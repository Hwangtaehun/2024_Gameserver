#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "MySocket.h"

class EchoServer : public MySocket
{
private:
	int addrlen;
	int retval;
public:
	EchoServer(void);
	~EchoServer(void);
	DWORD WINAPI ProcessClient();
	int Init();
	void A();
	void B();
	void C();
	void G();
	void D();
	void E();
	static DWORD WINAPI ThreadHandler(LPVOID lpRaram);
};