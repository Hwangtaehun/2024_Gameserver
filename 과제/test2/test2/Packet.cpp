#include "Packet.h"

Packet::Packet()
{
	size = 0;
	type = -1;
	endmark = 0xFF;
}

Packet::~Packet(){}

void Packet::SendMsg(char* msg)
{
	type = PK_DATA;
	sprintf_s(data, "%s\0", msg);
	size = (short)(strlen(data) + 6);
}

void Packet::GetData()
{
	switch (type)
	{
	case PK_DATA:
		printf("%s\n", data);
		break;
	case -1:
	default:
		printf("error");
	}
}

char* Packet::GetBuf() //직렬화
{
	memset(buf, 0, sizeof(buf));
	memcpy(buf, &size, sizeof(size));
	memcpy(buf + sizeof(size), &type, sizeof(type));
	memcpy(buf + sizeof(size) + sizeof(type), data, strlen(data));
	memcpy(buf + sizeof(size) + sizeof(type) + strlen(data), &endmark, sizeof(endmark));

	return buf;
}

void Packet::RecvMsg(char* str) //비직렬화
{
	memcpy(&size, str, sizeof(size));
	memcpy(&type, str + sizeof(size), sizeof(type));
	memcpy(data, str + sizeof(size) + sizeof(type), size - 6);
	memcpy(&endmark, str + sizeof(size) + sizeof(type) + size - 6, sizeof(endmark));
}

void Packet::Print()
{
	printf("%hd/%hd/%s/%hd\n", size, type, data, endmark);
}
