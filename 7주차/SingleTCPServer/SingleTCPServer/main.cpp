#include "Server.h"

int main(int argc, char* argv[])
{
    // ���� �ʱ�ȭ
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    Server server = Server(wsa);
    server.FSocket();
    server.FBind();
    server.FListen();

    while (1) {
        if (server.FAccept()) {
            break;
        }

        // Ŭ���̾�Ʈ�� ������ ���
        while (1) {
            if (server.FRecv()) {
                break;
            }

            if (server.FSend()) {
                break;
            }
        }

        server.FCloseSocket();
    }

    server.~Server();

    // ���� ����
    WSACleanup();
    return 0;
}
