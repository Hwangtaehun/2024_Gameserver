#include "Server.h"

int main(int argc, char* argv[])
{
    Server server = Server();

    server.Ready();
    server.Chat();
    server.~Server();

    return 0;
}
