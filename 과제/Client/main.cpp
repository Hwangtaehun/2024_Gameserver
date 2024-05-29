#include "Client.h"

int main(int argc, char* argv[])
{
    // 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;
    
    Client client = Client(wsa);
    client.Fsocket();
    client.Fceconnet();
    client.Frun();

    // closesocket()
    client.~Client();

    // 윈속 종료
    WSACleanup();
    return 0;
}
