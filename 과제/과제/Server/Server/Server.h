#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <vector>
#include "Packet.h"

#define SERVERPORT 9000
#define BUFSIZE    512

//클라이언트 정보
struct Inf
{
    char ip[16];
    u_short port;
    SOCKET socket;
};

static char buf[BUFSIZE + 1];
static vector<Inf> client;

class Server
{
public:
    Server();
    ~Server();
    void Ready();
    void Chat();

private:
    int retval;
    WSADATA wsa;
    SOCKET listen_sock;
    // 데이터 통신에 사용할 변수
    SOCKET client_sock;
    SOCKADDR_IN clientaddr;
    int addrlen;

    static void Err_quit(const char* msg);
    static void Err_display(const char* msg);
    static DWORD WINAPI ProcessClient(LPVOID arg);
    static void Print();
};