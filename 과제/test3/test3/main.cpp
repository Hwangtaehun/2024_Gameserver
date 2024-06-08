#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <vector>

using namespace std;

struct Inf
{
    char ip[16];
    char port[6];
};

int main() {
    vector<Inf> client;

    Inf myclient = { "127.0.0.1", "1000" };
    client.push_back(myclient);
    strcpy(myclient.ip, "192.0.0.1");
    strcpy(myclient.port, "2000");
    client.push_back(myclient);
    strcpy(myclient.ip, "192.0.1.2");
    strcpy(myclient.port, "3000");
    client.push_back(myclient);
    strcpy(myclient.ip, "192.0.2.3");
    strcpy(myclient.port, "4000");
    client.push_back(myclient);
    strcpy(myclient.ip, "192.0.3.4");
    strcpy(myclient.port, "5000");
    client.push_back(myclient);

    for (auto i : client) {
        printf("ip: %s, port: %s\n", i.ip, i.port);
    }

    client.erase(remove_if(client.begin(), client.end(),[](const Inf& client) { return strcmp(client.port, "3000") == 0;}),client.end());
    printf("\n");

    for (auto i : client) {
        printf("ip: %s, port: %s\n", i.ip, i.port);
    }

    return 0;
}