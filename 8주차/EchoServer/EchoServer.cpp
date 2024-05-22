#include "EchoServer.h"

EchoServer::EchoServer(void)
{
}

EchoServer::~EchoServer(void)
{
	WSACleanup();
}

int EchoServer::Init()
{
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
}

void EchoServer::A() {
	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
		err_quit("socket()");
}

void EchoServer::B()
{
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");
}

void EchoServer::C()
{
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");
}

void EchoServer::D()
{
	while (1) {
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (sockaddr*)&clientaddr, &addrlen);

		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		};

		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		
		while (1) {
			char buf[BUFSIZE + 1];
			retval = recv(client_sock, buf, BUFSIZE, 0);
			if (retval == SOCKET_ERROR) {
				break;
			}
			else if (retval == 0)
				break;

			buf[retval] = '\0';
			printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
				ntohs(clientaddr.sin_port), buf);
			retval = send(client_sock, buf, retval, 0);
			if (retval == SOCKET_ERROR) {
				break;
			}
		}
	}
}

void EchoServer::E()
{
	closesocket(listen_sock);
}

void EchoServer::G() {
	hThread = CreateThread(NULL, 0, ThreadHandler, NULL, CREATE_SUSPENDED, 0);
	if (hThread == NULL) { closesocket(client_sock); }
	else { CloseHandle(hThread); }
}

DWORD WINAPI EchoServer::ThreadHandler(LPVOID lpRaram)
{
	EchoServer* Echo = (EchoServer *)lpRaram;
	SOCKET client_sock = Echo->client_sock;
	int retval;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE + 1];
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);

	while (1) {
		retval = recv(client_sock, buf, BUFSIZE, 0);
		if (retval == SOCKET_ERROR) {
			break;
		}
		else if (retval == 0)
			break;

		buf[retval] = '\0';
		printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
			ntohs(clientaddr.sin_port), buf);
		retval = send(client_sock, buf, retval, 0);
		if (retval == SOCKET_ERROR) {
			break;
		}
	}
	return 0;
}
