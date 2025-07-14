#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <mutex>
#include <queue>
#include <string>
#include <algorithm>

#define SERVERPORT 9000
#define BUFSIZE    512

// 클라이언트별 상태를 저장하는 구조체
struct SOCKETINFO {
    OVERLAPPED overlapped;
    SOCKET sock;
    char buf[BUFSIZE + 1]; // 수신 버퍼
    int recvbytes;
    int sendbytes;
    WSABUF wsabuf;
    bool sending; // 현재 송신 중 여부
    std::queue<std::string> sendQueue; // 송신 대기열 (여러 메시지 처리)
};

std::vector<SOCKETINFO*> clients;
std::mutex clients_mutex;

DWORD WINAPI WorkerThread(LPVOID arg);
void err_quit(char* msg);
void err_display(char* msg);
bool send(SOCKETINFO* ptr);
bool receive(SOCKETINFO* ptr);
void broadcast(SOCKETINFO* sender, const char* msg, int len);
void remove_client(SOCKETINFO* ptr);

int main(int argc, char* argv[]) {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

    HANDLE hcp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    if (hcp == NULL) return 1;

    SYSTEM_INFO si;
    GetSystemInfo(&si);
    for (int i = 0; i < (int)si.dwNumberOfProcessors * 2; i++) {
        HANDLE hThread = CreateThread(NULL, 0, WorkerThread, hcp, 0, NULL);
        CloseHandle(hThread);
    }

    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET) err_quit("socket()");

    SOCKADDR_IN serveraddr = { 0 };
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);
    if (bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR)
        err_quit("bind()");

    if (listen(listen_sock, SOMAXCONN) == SOCKET_ERROR)
        err_quit("listen()");

    printf("[Chat Server] Listening on port %d...\n", SERVERPORT);

    while (1) {
        SOCKADDR_IN clientaddr;
        int addrlen = sizeof(clientaddr);
        SOCKET client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET) {
            err_display("accept()");
            continue;
        }
        printf("[Client Connected] %s:%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

        CreateIoCompletionPort((HANDLE)client_sock, hcp, (ULONG_PTR)client_sock, 0);

        SOCKETINFO* ptr = new SOCKETINFO;
        ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
        ptr->sock = client_sock;
        ptr->recvbytes = ptr->sendbytes = 0;
        ptr->wsabuf.buf = ptr->buf;
        ptr->wsabuf.len = BUFSIZE;
        ptr->sending = false;

        {
            std::lock_guard<std::mutex> lock(clients_mutex);
            clients.push_back(ptr);
        }

        receive(ptr);
    }

    WSACleanup();
    return 0;
}

DWORD WINAPI WorkerThread(LPVOID arg) {
    HANDLE hcp = (HANDLE)arg;
    DWORD cbTransferred;
    SOCKET client_sock;
    SOCKETINFO* ptr;
    int retval;

    while (1) {
        retval = GetQueuedCompletionStatus(hcp, &cbTransferred, (LPDWORD)&client_sock, (LPOVERLAPPED*)&ptr, INFINITE);
        if (retval == 0 || cbTransferred == 0) {
            err_display("Client Disconnected");
            closesocket(ptr->sock);
            remove_client(ptr);
            delete ptr;
            continue;
        }

        if (ptr->sending) {
            // 송신 완료 처리
            ptr->sendbytes += cbTransferred;
            if (ptr->sendbytes < ptr->recvbytes) {
                // 남은 데이터 추가 송신
                send(ptr);
            }
            else {
                // 현재 메시지 송신 완료, 다음 메시지 처리
                ptr->sending = false;
                ptr->recvbytes = ptr->sendbytes = 0;
                if (!ptr->sendQueue.empty()) {
                    std::string next_msg = ptr->sendQueue.front();
                    ptr->sendQueue.pop();
                    memcpy(ptr->buf, next_msg.c_str(), next_msg.size());
                    ptr->recvbytes = next_msg.size();
                    ptr->sending = true;
                    send(ptr);
                }
                else {
                    receive(ptr);
                }
            }
        }
        else {
            // 수신 완료 처리
            ptr->recvbytes = cbTransferred;
            ptr->buf[cbTransferred] = '\0';

            SOCKADDR_IN clientaddr;
            int addrlen = sizeof(clientaddr);
            getpeername(ptr->sock, (SOCKADDR*)&clientaddr, &addrlen);

            printf("[Message from %s:%d] %s\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), ptr->buf);

            broadcast(ptr, ptr->buf, cbTransferred);
            receive(ptr);
        }
    }
    return 0;
}

// 클라이언트에게 메시지를 브로드캐스트 (전송큐에 삽입)
void broadcast(SOCKETINFO* sender, const char* msg, int len) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    for (auto client : clients) {
        if (client->sock == sender->sock) continue; // 본인 제외

        std::string message(msg, len);
        client->sendQueue.push(message);

        if (!client->sending) {
            std::string next_msg = client->sendQueue.front();
            client->sendQueue.pop();
            memcpy(client->buf, next_msg.c_str(), next_msg.size());
            client->recvbytes = next_msg.size();
            client->sendbytes = 0;
            client->sending = true;
            send(client);
        }
    }
}

// 클라이언트 연결 종료 시 리스트에서 삭제
void remove_client(SOCKETINFO* ptr) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    clients.erase(std::remove(clients.begin(), clients.end(), ptr), clients.end());
}

// 비동기 송신 처리
bool send(SOCKETINFO* ptr) {
    ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
    ptr->wsabuf.buf = ptr->buf + ptr->sendbytes;
    ptr->wsabuf.len = ptr->recvbytes - ptr->sendbytes;
    DWORD sendbytes;
    int retval = WSASend(ptr->sock, &ptr->wsabuf, 1, &sendbytes, 0, &ptr->overlapped, NULL);
    if (retval == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
        err_display("WSASend()");
    return retval == SOCKET_ERROR;
}

// 비동기 수신 처리
bool receive(SOCKETINFO* ptr) {
    ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
    ptr->wsabuf.buf = ptr->buf;
    ptr->wsabuf.len = BUFSIZE;
    DWORD recvbytes, flags = 0;
    int retval = WSARecv(ptr->sock, &ptr->wsabuf, 1, &recvbytes, &flags, &ptr->overlapped, NULL);
    if (retval == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
        err_display("WSARecv()");
    return retval == SOCKET_ERROR;
}

// 치명적 오류 발생 시 종료
void err_quit(char* msg) {
    LPVOID lpMsgBuf;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
    MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
    LocalFree(lpMsgBuf);
    exit(1);
}

// 오류 메시지 출력
void err_display(char* msg) {
    LPVOID lpMsgBuf;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
    printf("[%s] %s\n", msg, (char*)lpMsgBuf);
    LocalFree(lpMsgBuf);
}