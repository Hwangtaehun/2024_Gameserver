#include "Client.h"

Client::Client(WSADATA wsa)
{
    this->wsa = wsa;
}

Client::~Client()
{
    closesocket(sock);
}

void Client::Fsocket()
{
    // socket()
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) Err_quit("socket()");
}

void Client::Fceconnet()
{
    SOCKADDR_IN serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) Err_quit("connect()");
}

bool Client::Fsend()
{
    // ������ �Է�
    printf("\n[���� ������] ");
    if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
        return true;

    // '\n' ���� ����
    len = strlen(buf);
    if (buf[len - 1] == '\n')
        buf[len - 1] = '\0';//null ���ڱ��� ó��
    if (strlen(buf) == 0)
        return true;

    // ������ ������
    retval = send(sock, buf, strlen(buf), 0);
    if (retval == SOCKET_ERROR) {
        Err_display("send()");
        return true;
    }
    printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� ���½��ϴ�.\n", retval);

    return false;
}

bool Client::Frecv()
{
    // ������ �ޱ�
    retval = recv(sock, buf, BUFSIZE, 0);
    if (retval == SOCKET_ERROR) {
        Err_display("recv()");
        return true;
    }
    else if (retval == 0)
        return true;

    // ���� ������ ���
    buf[retval] = '\0';
    printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� �޾ҽ��ϴ�.\n", retval);
    printf("[���� ������] %s\n", buf);

    return false;
}

// ���� �Լ� ���� ��� �� ����
void Client::Err_quit(const char* msg)
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
void Client::Err_display(const char* msg)
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