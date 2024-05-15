#include "Server.h"

int main(int argc, char* argv[])
{
    // 윈속 초기화
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

        // 클라이언트와 데이터 통신
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

    // 윈속 종료
    WSACleanup();
    return 0;
}
