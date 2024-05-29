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
    // 데이터 입력
    printf("\n[보낼 데이터] ");
    if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
        return true;

    // '\n' 문자 제거
    len = strlen(buf);
    if (buf[len - 1] == '\n')
        buf[len - 1] = '\0';//null 문자까지 처리
    if (strlen(buf) == 0)
        return true;

    // 데이터 보내기
    retval = send(sock, buf, strlen(buf), 0);
    if (retval == SOCKET_ERROR) {
        Err_display("send()");
        return true;
    }
    printf("[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);

    return false;
}

bool Client::Frecv()
{
    // 데이터 받기
    retval = recv(sock, buf, BUFSIZE, 0);
    if (retval == SOCKET_ERROR) {
        Err_display("recv()");
        return true;
    }
    else if (retval == 0)
        return true;

    // 받은 데이터 출력
    buf[retval] = '\0';
    printf("[TCP 클라이언트] %d바이트를 받았습니다.\n", retval);
    printf("[받은 데이터] %s\n", buf);

    return false;
}

// 소켓 함수 오류 출력 후 종료
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

// 소켓 함수 오류 출력
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