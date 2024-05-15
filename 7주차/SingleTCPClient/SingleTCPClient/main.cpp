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

    // ������ ������ ���
    while (1) {
        if (client.Fsend()) {
            break;
        }

        if (client.Frecv()) {
            break;
        }
    }

    // closesocket()
    client.~Client();

    // ���� ����
    WSACleanup();
    return 0;
}
