#include "Server.h"

Server::Server(WSADATA wsa)
{
    this->wsa = wsa;
}

Server::~Server()
{
    // closesocket()
    closesocket(listen_sock);
}

void Server::FSocket()
{
    // socket()
    listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET) Err_quit("socket()");
}

void Server::FBind()
{
    // bind()
    SOCKADDR_IN serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) Err_quit("bind()");
}

void Server::FListen()
{
    // listen()
    retval = listen(listen_sock, SOMAXCONN);
    if (retval == SOCKET_ERROR) Err_quit("listen()");
}

bool Server::FAccept()
{
    // accept()
    addrlen = sizeof(clientaddr);
    client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
    if (client_sock == INVALID_SOCKET) {
        Err_display("accept()");
        return true;
    }

    // ������ Ŭ���̾�Ʈ ���� ���
    printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
        inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

    return false;
}

bool Server::FSend()
{
    // ������ ������
    retval = send(client_sock, buf, strlen(buf), 0);
    if (retval == SOCKET_ERROR) {
        Err_display("send()");
        return true;
    }

    return false;
}

bool Server::FRecv()
{
    // ������ �ޱ�
    retval = recv(client_sock, buf, BUFSIZE, 0);
    if (retval == SOCKET_ERROR) {
        Err_display("recv()");
        return true;
    }
    else if (retval == 0)
        return true;

    // ���� ������ ���
    buf[retval] = '\0';
    printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
        ntohs(clientaddr.sin_port), buf);
    sprintf(buf, "%s from Server", buf);

    return false;
}

void Server::FCloseSocket()
{
    // closesocket()
    closesocket(client_sock);
    printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
        inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
}

// ���� �Լ� ���� ��� �� ����
void Server::Err_quit(const char* msg)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL);
    MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
    LocalFree(lpMsgBuf);
    exit(1);
}

// ���� �Լ� ���� ���
void Server::Err_display(const char* msg)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL);
    printf("[%s] %s", msg, (char*)lpMsgBuf);
    LocalFree(lpMsgBuf);
}