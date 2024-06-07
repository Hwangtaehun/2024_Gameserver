#include "Client.h"

int main(int argc, char* argv[])
{
    Client client = Client();
    client.Ready();
    client.Run();
    client.~Client();
    
    return 0;
}
