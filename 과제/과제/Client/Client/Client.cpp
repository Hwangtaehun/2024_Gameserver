#include "Client.h"

Client::Client()
{
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return;
}

Client::~Client()
{
    closesocket(sock);
    // ���� ����
    WSACleanup();
}

void Client::Ready()
{
    // socket()
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) Err_quit("socket()");

    SOCKADDR_IN serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) Err_quit("connect()");
}

void Client::Run()
{
    Packet pk = Packet();
    HANDLE hThread;

    sprintf(buf, "connect");
    retval = send(sock, buf, strlen(buf), 0);
    if (retval == SOCKET_ERROR) {
        Err_display("send()");
        return;
    }

    retval = recv(sock, buf, BUFSIZE, 0);
    if (retval == SOCKET_ERROR) {
        Err_display("recv()");
        return;
    }
    else if (retval == 0)
        return;

    // ���� ������ ���
    buf[retval] = '\0';
    pk.RecvMsg(buf);
    pk.GetData(buf);
    printf("%s\n", buf);
    printf("M: ��ġ����, C: ��ȭ, X: ����� ���ϴ� �޴��� �������ּ���.\n");

    hThread = CreateThread(NULL, 0, ThreadRecv, (LPVOID)sock, 0, NULL);
    
    // ������ ������ ���
    while (1) {
        char me;
        if (!pk.Check()) {
            me = 'M';
        }
        else {
            //�޴� ����
            me = getchar();
            getchar();
        }
        

        if(me == 'X'){
            pk.SetClose(SERVERIP);
            memcpy(buf, pk.GetBuf(), pk.GetSize());
        }
        else if (me != 'C' && me != 'M') {
            printf("�߸� �Է��߽��ϴ�.\n");
            continue;
        }
        else {
            // ������ �Է�
            printf("\n[���� ������] ");
            if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
                break;

            // '\n' ���� ����
            len = strlen(buf);
            if (buf[len - 1] == '\n')
                buf[len - 1] = '\0';//null ���ڱ��� ó��
            if (strlen(buf) == 0)
                break;

            if (me == 'M') {
                pk.SetMove(SERVERIP, buf);
                if (!pk.Check()) {
                    printf("��ǥ�� �߸� �Է��߽��ϴ�. �ٽ� �Է����ּ���.");
                    continue;
                }
                memcpy(buf, pk.GetBuf(), pk.GetSize());
            }
            else {
                pk.SendMsg(buf);
                memcpy(buf, pk.GetBuf(), pk.GetSize());
            }
        }

        // ������ ������
        retval = send(sock, buf, pk.GetSize(), 0);
        if (retval == SOCKET_ERROR) {
            Err_display("send()");
            break;
        }
        printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� ���½��ϴ�.\n", retval);
        if (me == 'X') {
            break;
        }
    }

    if (hThread == NULL) { closesocket(sock); }
    else { CloseHandle(hThread); }
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

DWORD __stdcall Client::ThreadRecv(LPVOID arg)
{
    int retval;
    char buf[BUFSIZE + 1];
    Packet pk = Packet();
    SOCKET sock = (SOCKET)arg;
    
    while (1)
    {
        // ������ �ޱ�
        retval = recv(sock, buf, BUFSIZE, 0);
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
        printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� �޾ҽ��ϴ�.\n", retval);
        printf("[���� ������] %s\n", buf);
    }

    return 0;
}
