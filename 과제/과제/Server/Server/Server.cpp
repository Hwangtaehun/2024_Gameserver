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
    // ���� ����
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

    while (1) {
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET) {
            Err_display("accept()");
            break;
        }

        // ������ Ŭ���̾�Ʈ ���� ���
        printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
            inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

        hThread = CreateThread(NULL, 0, ProcessClient,
            (LPVOID)client_sock, 0, NULL);
        if (hThread == NULL) { closesocket(client_sock); }
        else { CloseHandle(hThread); }
    }
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

DWORD Server::ProcessClient(LPVOID arg)
{
    SOCKET client_sock = (SOCKET)arg;
    int retval;
    SOCKADDR_IN clientaddr;
    int addrlen;
    Packet pk = Packet();
    Inf myclient;

    // Ŭ���̾�Ʈ ���� ���
    addrlen = sizeof(clientaddr);
    getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);

    //�߰�
    time_t timer;
    timer = time(NULL);
    struct tm* t;
    t = localtime(&timer);
    strcpy(myclient.ip, inet_ntoa(clientaddr.sin_addr));
    myclient.port = ntohs(clientaddr.sin_port);
    myclient.socket = client_sock;
    client.push_back(myclient);

    // ������ �ޱ�
    retval = recv(client_sock, buf, BUFSIZE, 0);
    if (retval == SOCKET_ERROR) {
        Err_display("recv()");
        return 0;
    }
    else if (retval == 0)
        return 0;

    // ���� ������ ���
    buf[retval] = '\0';

    sprintf(buf, "%d�� %d�� %d�� %d�� %d�� %d�� %s",
        t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, inet_ntoa(clientaddr.sin_addr));
    pk.SendAllConnect(buf);
    memcpy(buf, pk.GetBuf(), pk.GetSize());

    retval = send(client_sock, buf, pk.GetSize(), 0);
    if (retval == SOCKET_ERROR) {
        Err_display("send()");
        return 0;
    }

    while (1) {
        // ������ �ޱ�
        retval = recv(client_sock, buf, BUFSIZE, 0);
        if (retval == SOCKET_ERROR) {
            Err_display("recv()");
            break;
        }
        else if (retval == 0)
            break;

        // ���� ������ ���
        buf[retval] = '\0';
        pk.RecvMsg(buf);
        pk.GetData(buf);
        printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
            ntohs(clientaddr.sin_port), buf);

        if (req_move == pk.GetType()) {
            pk.SendAllMove();
        }
        else if (req_dis == pk.GetType()) {
            sprintf(buf, "%d�� %d�� %d�� %d�� %d�� %d�� %s",
                t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, inet_ntoa(clientaddr.sin_addr));
            pk.SetClose(buf);
        }

        // ������ ������
        memcpy(buf, pk.GetBuf(), pk.GetSize());
        for (int i = 0; i < client.size(); i++) {
            retval = send(client[i].socket, buf, pk.GetSize(), 0);
            if (retval == SOCKET_ERROR) {
                Err_display("send()");
                break;
            }
        }
        /*retval = send(client_sock, buf, pk.GetSize(), 0);
        if (retval == SOCKET_ERROR) {
            Err_display("send()");
            break;
        }*/
    }

    // closesocket()
    closesocket(client_sock);
    printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
        inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
    u_short value = myclient.port;
    client.erase(remove_if(client.begin(), client.end(), [value](const Inf& client) { return client.port == value; }), client.end());

    return 0;
}

void Server::Print()
{
    SOCKET m_sk;
    for (int i = 0; i < client.size(); i++)
    {
        m_sk = client[i].socket;
        printf("%d��° %s, %d\n", i, client[i].ip, client[i].port);
    }
}
