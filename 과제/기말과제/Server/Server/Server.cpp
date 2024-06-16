#include "Server.h"

Server::Server()
{
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return;
}

Server::~Server()
{
    // closesocket()
    closesocket(listen_sock);
    // 윈속 종료
    WSACleanup();
}

void Server::Ready()
{
    // socket()
    listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET) Err_quit("socket()");

    // bind()
    SOCKADDR_IN serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) Err_quit("bind()");

    // listen()
    retval = listen(listen_sock, SOMAXCONN);
    if (retval == SOCKET_ERROR) Err_quit("listen()");
}

void Server::Chat()
{
    HANDLE hThread;

    InitializeCriticalSection(&hCriticalSection);

    while (1) {
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET) {
            Err_display("accept()");
            break;
        }

        // 접속한 클라이언트 정보 출력
        printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
            inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

        hThread = CreateThread(NULL, 0, ProcessClient, (LPVOID)client_sock, 0, NULL);
        if (hThread == NULL) { 
            closesocket(client_sock); 
        }
        else { 
            CloseHandle(hThread); 
        }
    }

    DeleteCriticalSection(&hCriticalSection);
}

// 소켓 함수 오류 출력 후 종료
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

// 소켓 함수 오류 출력
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

DWORD Server::ProcessClient(LPVOID arg)
{
    SOCKET client_sock = (SOCKET)arg;
    int retval;
    SOCKADDR_IN clientaddr;
    int addrlen;
    ServerPacket pk = ServerPacket();
    Inf myclient;

    // 클라이언트 정보 얻기
    addrlen = sizeof(clientaddr);
    getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);

    //접속 시간
    time_t timer;
    char m_buf[BUFSIZE + 1];
    timer = time(NULL);
    struct tm* t;
    t = localtime(&timer);

    // 데이터 받기
    retval = recv(client_sock, m_buf, BUFSIZE, 0);
    if (retval == SOCKET_ERROR) {
        Err_display("recv()");
        return 0;
    }
    else if (retval == 0)
        return 0;

    // 받은 데이터 출력
    m_buf[retval] = '\0';
    pk.RecvMsg(m_buf);
    pk.GetData(m_buf);

    //클라이언트 정보 백터형식으로 추가
    strcpy(myclient.ip, inet_ntoa(clientaddr.sin_addr));
    myclient.port = ntohs(clientaddr.sin_port);
    myclient.socket = client_sock;
    strcpy(myclient.name, pk.GetName());
    client.push_back(myclient);
    printf("%s\n", m_buf);

    //클라이언트의 연결 성공 시 서버는 연결되어 있는 모든 클라이언트에게 보내기
    sprintf(m_buf, "%d년 %d월 %d일 %d시 %d분 %d초 %s",
        t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, inet_ntoa(clientaddr.sin_addr));
    pk.SendAllConnect(m_buf);
    memcpy(m_buf, pk.GetBuf(), pk.GetSize());

    for (int i = 0; i < client.size(); i++) {
        retval = send(client[i].socket, m_buf, pk.GetSize(), 0);
        if (retval == SOCKET_ERROR) {
            Err_display("send()");
            break;
        }
    }

    while (1) {
        // 데이터 받기
        retval = recv(client_sock, m_buf, BUFSIZE, 0);
        if (retval == SOCKET_ERROR) {
            Err_display("recv()");
            break;
        }
        else if (retval == 0)
            break;

        // 받은 데이터 출력
        m_buf[retval] = '\0';
        pk.RecvMsg(m_buf);
        pk.GetData(m_buf);
        printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
            ntohs(clientaddr.sin_port), m_buf);

        if (req_move == pk.GetType()) {
            pk.SendAllMove();
        }
        else if (req_dis == pk.GetType()) {
            timer = time(NULL);
            t = localtime(&timer);
            sprintf(m_buf, "%d년 %d월 %d일 %d시 %d분 %d초 %s",
                t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, inet_ntoa(clientaddr.sin_addr));
            pk.SetClose(m_buf);
        }

        EnterCriticalSection(&hCriticalSection);

        // 데이터 보내기
        if (chat_string == pk.GetType()) {
            pk.SChatMsg(myclient.name);
        }
        
        memcpy(buf, pk.GetBuf(), pk.GetSize());
        for (int i = 0; i < client.size(); i++) {
            if (myclient.port != client[i].port) {
                retval = send(client[i].socket, buf, pk.GetSize(), 0);
                if (retval == SOCKET_ERROR) {
                    Err_display("send()");
                    break;
                }
            }
        }

        LeaveCriticalSection(&hCriticalSection);

        if (req_dis == pk.GetType()) {
            break;
        }
    }

    // closesocket()
    u_short value = myclient.port;
    client.erase(remove_if(client.begin(), client.end(), [value](const Inf& client) { return client.port == value; }), client.end());

    closesocket(client_sock);
    printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
        inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

    return 0;
}

void Server::Print()
{
    SOCKET m_sk;
    for (int i = 0; i < client.size(); i++)
    {
        m_sk = client[i].socket;
        printf("%d번째 %s, %d\n", i, client[i].ip, client[i].port);
    }
}
