#include "Packet.h"

Packet::Packet()
{
	size = 0;
	type = -1;
	endmark = 0xFF;
	m_x = 0.0f;
	m_y = 0.0f;
	m_z = 0.0f;
}

Packet::~Packet(){}

void Packet::Separate(char* ip, char* x, char* y, char* z)
{
	int len = 0, i = 0, cnt = 0;

	for (i = 0; i <= DATASIZE; i++) {
		if (data[i] == '\0') {
			len = i;
			i = 0;
			break;
		}
	}

	while (cnt < 4) {
		switch (cnt)
		{
		case 0:
			i = Input(ip, i, len);
			break;
		case 1:
			i = Input(x, i, len);
			break;
		case 2:
			i = Input(y, i, len);
			break;
		case 3:
			i = Input(z, i, len);
			break;
		}

		cnt++;
	}
}

int Packet::Input(char* m_value, int i, int len)
{
	int j = 0;

	for (i; i < len; i++) {
		if (data[i] == ',') {
			m_value[j] = '\0';
			i++;
			break;
		}
		else {
			m_value[j] = data[i];
		}
		j++;
	}

	if (i == len) {
		m_value[j] = '\0';
	}

	return i;
}

void Packet::SetConnect(char* ip)
{
	type = req_con;
	sprintf_s(data, "%s\0", ip);
	size = (short)strlen(data) + 6;
}

void Packet::SendAllConnect()
{
	type = ack_con;
	size = (short)strlen(data) + 6;
}

void Packet::SetMove(char* ip, float x, float y, float z)
{
	type = req_move;
	sprintf_s(data, "%s,%.2f,%.2f,%.2f\0", ip, x, y, z);
	size = (short)strlen(data) + 6;
}

void Packet::SendAllMove()
{
	type = ack_move;
	size = (short)strlen(data) + 6;
}

void Packet::GetData(char* temp)
{
	switch (type)
	{
	case req_con:
		sprintf(temp, "%s, connection success", data);
		break;
	case ack_con:
		sprintf(temp, "%s, connection completed", data);
		break;
	case req_move:
	case ack_move:
		char ip[16], x[16], y[16], z[16];
		Separate(ip, x, y, z);
		m_x = stof(x);
		m_y = stof(y);
		m_z = stof(z);
		sprintf(temp, "%s, request to move %.2f, %.2f, %.2f", ip, m_x, m_y, m_z);
		break;
	case chat_string:
		sprintf(temp, "%s", data);
	case -1:
		sprintf(temp, "error");
	}
}

char* Packet::GetBuf() //Á÷·ÄÈ­
{
	memset(buf, 0, sizeof(buf));
	memcpy(buf, &size, sizeof(size));
	memcpy(buf + sizeof(size), &type, sizeof(type));
	memcpy(buf + sizeof(size) + sizeof(type), data, strlen(data));
	memcpy(buf + sizeof(size) + sizeof(type) + strlen(data), &endmark, sizeof(endmark));

	return buf;
}

void Packet::RecvMsg(char* str)
{
	int len;
	memcpy(&size, str, sizeof(size));
	memcpy(&type, str + sizeof(size), sizeof(type));
	len = (int)size - sizeof(size) - sizeof(type) - sizeof(endmark);
	memcpy(data, str + sizeof(size) + sizeof(type), len);
	memcpy(&endmark, str + sizeof(size) + sizeof(type) + len, sizeof(endmark));
	memcpy(buf, str, size);
}

int Packet::GetSize()
{
	return (int)size;
}

void Packet::Print()
{
	printf("%hd/%hd/%s/%hd\n", size, type, data, endmark);
}
