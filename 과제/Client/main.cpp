#include "Client.h"

int main(int argc, char* argv[])
{
    // ���� �ʱ�ȭ
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;
    
    Client client = Client(wsa);
    client.Fsocket();
    client.Fceconnet();
    client.Frun();

    // closesocket()
    client.~Client();

    // ���� ����
    WSACleanup();
    return 0;
}
