#include <iostream>
#include <vector>
#include <thread>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 9000
#define MAX_CLIENTS 64

struct ClientContext {
    OVERLAPPED overlapped;
    SOCKET socket;
    char buffer[1024];
    WSABUF dataBuffer;
    DWORD bytesReceived;
};

std::vector<ClientContext*> clients;

DWORD WINAPI WorkerThread(LPVOID lpParam) {
    HANDLE completionPort = reinterpret_cast<HANDLE>(lpParam);
    DWORD bytesTransferred;
    ULONG_PTR completionKey;
    LPOVERLAPPED overlapped;

    while (true) {
        BOOL result = GetQueuedCompletionStatus(completionPort, &bytesTransferred, &completionKey, &overlapped, INFINITE);

        if (result == 0 || bytesTransferred == 0) {
            closesocket(reinterpret_cast<ClientContext*>(completionKey)->socket);
            delete reinterpret_cast<ClientContext*>(completionKey);
            continue;
        }

        ClientContext* clientContext = reinterpret_cast<ClientContext*>(completionKey);
        for (auto& client : clients) {
            if (client != clientContext) {
                WSABUF dataBuffer;
                dataBuffer.buf = clientContext->buffer;
                dataBuffer.len = bytesTransferred;
                DWORD sentBytes;
                WSASend(client->socket, &dataBuffer, 1, &sentBytes, 0, nullptr, nullptr);
            }
        }

        memset(clientContext->buffer, 0, sizeof(clientContext->buffer));
        clientContext->dataBuffer.len = sizeof(clientContext->buffer);
        DWORD flags = 0;
        WSARecv(clientContext->socket, &clientContext->dataBuffer, 1, &clientContext->bytesReceived, &flags, &clientContext->overlapped, nullptr);
    }

    return 0;
}

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET listenSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(PORT);
    hint.sin_addr.S_un.S_addr = INADDR_ANY;

    bind(listenSocket, reinterpret_cast<sockaddr*>(&hint), sizeof(hint));
    listen(listenSocket, SOMAXCONN);

    HANDLE completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
    for (int i = 0; i < std::thread::hardware_concurrency(); ++i) {
        CreateThread(nullptr, 0, WorkerThread, completionPort, 0, nullptr);
    }

    while (true) {
        ClientContext* clientContext = new ClientContext();
        clientContext->socket = accept(listenSocket, nullptr, nullptr);
        CreateIoCompletionPort(reinterpret_cast<HANDLE>(clientContext->socket), completionPort, reinterpret_cast<ULONG_PTR>(clientContext), 0);

        memset(&clientContext->overlapped, 0, sizeof(clientContext->overlapped));
        clientContext->dataBuffer.buf = clientContext->buffer;
        clientContext->dataBuffer.len = sizeof(clientContext->buffer);
        DWORD flags = 0;
        WSARecv(clientContext->socket, &clientContext->dataBuffer, 1, &clientContext->bytesReceived, &flags, &clientContext->overlapped, nullptr);

        clients.push_back(clientContext);
    }

    closesocket(listenSocket);
    WSACleanup();

    return 0;
}