#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERPORT 9000
#define BUFSIZE    512

class Server
{
public:
    Server(WSADATA wsa);
    ~Server();
    void FSocket();
    void FBind();
    void FListen();
    bool FAccept();
    bool FSend();
    bool FRecv();
    void FCloseSocket();

private:
    int retval;
    WSADATA wsa;
    SOCKET listen_sock;
    // 데이터 통신에 사용할 변수
    SOCKET client_sock;
    SOCKADDR_IN clientaddr;
    int addrlen;
    char buf[BUFSIZE + 1];

    void Err_quit(const char* msg);
    void Err_display(const char* msg);
};