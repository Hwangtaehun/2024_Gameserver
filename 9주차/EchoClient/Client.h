#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS // �ֽ� VC++ ������ �� ��� ����
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE    512

class Client
{
public:
    Client(WSADATA wsa);
    ~Client();
    void Fsocket();
    void Fceconnet();
    void Frun();
    bool Fsend();
    bool Frecv();

private:
    int retval;
    WSADATA wsa;
    SOCKET sock;
    // ������ ��ſ� ����� ����
    char buf[BUFSIZE + 1];
    int len;

    void Err_quit(const char* msg);
    void Err_display(const char* msg);
};