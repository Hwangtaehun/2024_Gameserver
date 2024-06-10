#include "ServerPacket.h"

ServerPacket::ServerPacket()
{
}

ServerPacket::~ServerPacket()
{
}

//서버
void ServerPacket::SendAllConnect(char* msg)
{
	type = ack_con;
	sprintf_s(data, "%s\0", msg);
	size = (short)strlen(data) + 6;
}

//서버
void ServerPacket::SendAllMove()
{
	type = ack_move;
	size = (short)strlen(data) + 6;
}

//서버
void ServerPacket::SChatMsg(int port)
{
	char temp[DATASIZE] = {};

	RecvMsg(buf);
	sprintf(temp, "%d-%s\0", port, data);
	sprintf(data, "%s\0", temp);
	size = (short)strlen(data) + 6;
}