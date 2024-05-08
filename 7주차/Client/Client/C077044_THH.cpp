#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFSIZE 512

using namespace std;

class Packet {
private:
	int size;
	char type[8];
	char buf[BUFSIZE + 1];
	int flag;

public:
	void setConnect(char* ip);
	void sendAllConnect(char* ip);
	void setMove(char* ip, float x, float y, float z);
	void sendAllMove(char* mes);
	char *getBuf();
};

void Packet::setConnect(char* ip)
{
	sprintf_s(type, "req_con");
	sprintf_s(buf, "%s , connection success", ip);
	size = strlen(buf);
	flag = atoi("0xFF");
}

void Packet::sendAllConnect(char* ip)
{
	sprintf_s(type, "ack_con");
	sprintf_s(buf, "%s , connection completed", ip);
	size = strlen(buf);
	flag = atoi("0xFF");
}

void Packet::setMove(char* ip, float x, float y, float z)
{
	sprintf_s(type, "req_move");
	sprintf_s(buf, "%s, request to move %f, %f, %f", ip, x, y, z);
	size = strlen(buf);
	flag = atoi("0xFF");
}

void Packet::sendAllMove(char* mes)
{
	sprintf_s(type, "ack_move");
	sprintf_s(buf, mes);
	size = strlen(buf);
	flag = atoi("0xFF");
}

char * Packet::getBuf()
{
	return buf;
}
