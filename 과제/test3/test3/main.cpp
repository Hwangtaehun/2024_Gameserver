#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <vector>

using namespace std;

struct Inf
{
    char ip[16];
    short port;
};

int main() {
    vector<Inf> client;

    Inf myclient = { "127.0.0.1", 1000 };
    client.push_back(myclient);
    strcpy(myclient.ip, "192.0.0.1");
    myclient.port = (short)2000;
    client.push_back(myclient);
    strcpy(myclient.ip, "192.0.1.2");
    myclient.port = (short)3000;
    client.push_back(myclient);
    strcpy(myclient.ip, "192.0.2.3");
    myclient.port = (short)4000;
    client.push_back(myclient);
    strcpy(myclient.ip, "192.0.3.4");
    myclient.port = (short)5000;
    client.push_back(myclient);

    for (auto i : client) {
        printf("ip: %s, port: %hd\n", i.ip, i.port);
    }

    short value = 3000;

    client.erase(remove_if(client.begin(), client.end(),[value](const Inf& client) { return client.port == value;}),client.end());
    printf("\n");

    for (auto i : client) {
        printf("ip: %s, port: %hd\n", i.ip, i.port);
    }

    return 0;
}