#include "Packet.h"

Packet::Packet()
{
	size = 0;
	type = -1;
	endmark = 0xFF;
	m_x = 0.0f;
	m_y = 0.0f;
	m_z = 0.0f;
	crt = true;
}

Packet::Packet(char* nickname)
{
	size = 0;
	type = -1;
	endmark = 0xFF;
	m_x = 0.0f;
	m_y = 0.0f;
	m_z = 0.0f;
	crt = true;
	sprintf(name, "%s", nickname);
}

Packet::~Packet(){}

void Packet::Separate(char* ip)
{
	int len = 0, i = 0, cnt = 0;

	for (i = 0; i <= DATASIZE; i++) {
		if (data[i] == '\0') {
			len = i;
			i = 0;
			break;
		}
	}

	while (cnt < 2) {
		switch (cnt)
		{
		case 0:
			i = Input(name, i, len);
			break;
		case 1:
			i = Input(ip, i, len);
			break;
		}

		cnt++;
	}
}

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

void Packet::Separate(char* msg, float* x, float* y, float* z) 
{
	char c_x[16] = {}, c_y[16] = {}, c_z[16] = {};
	int len = 0, i = 0, cnt = 0;

	for (i = 0; i <= DATASIZE; i++) {
		if (msg[i] == '\0') {
			len = i;
			i = 0;
			break;
		}
	}

	while (cnt < 3) {
		switch (cnt)
		{
		case 0:
			i = Input(msg, c_x, i, len);
			break;
		case 1:
			i = Input(msg, c_y, i, len);
			break;
		case 2:
			i = Input(msg, c_z, i, len);
			break;
		}

		cnt++;
	}

	if (atoi(c_x) && atoi(c_y) && atoi(c_z)) {
		*x = stof(c_x);
		*y = stof(c_y);
		*z = stof(c_z);
		crt = true;
	}
	else {
		crt = false;
	}
	
}

int Packet::Input(char* value, int i, int len)
{
	int j = 0;

	for (i; i < len; i++) {
		if (data[i] == ',') {
			value[j] = '\0';
			i++;
			break;
		}
		else {
			value[j] = data[i];
		}
		j++;
	}

	if (i == len) {
		value[j] = '\0';
	}

	return i;
}

int Packet::Input(char* msg, char* value, int i, int len)
{
	int j = 0;

	for (i; i < len; i++) {
		if (msg[i] == ',') {
			value[j] = '\0';
			i++;
			break;
		}
		else {
			value[j] = msg[i];
		}
		j++;
	}

	if (i == len) {
		value[j] = '\0';
	}

	return i;
}

//클라이언트가 서버 연결
void Packet::SetConnect(char* ip)
{
	type = req_con;
	sprintf_s(data, "%s,%s\0", name, ip);
	size = (short)strlen(data) + 6;
}

//서버에 연결된 클라이언트 ip를 모두에게 전달
void Packet::SendAllConnect(char* msg)
{
	type = ack_con;
	sprintf_s(data, "%s\0", msg);
	size = (short)strlen(data) + 6;
}

void Packet::SetMove(char* ip, char* msg)
{
	float x = 0, y = 0, z = 0;

	type = req_move;
	Separate(msg, &x, &y, &z);
	sprintf_s(data, "%s,%.2f,%.2f,%.2f\0", ip, x, y, z);
	size = (short)strlen(data) + 6;
}

void Packet::SendAllMove()
{
	type = ack_move;
	size = (short)strlen(data) + 6;
}

void Packet::RecvMsg(char* str) //비직렬화
{
	int len;

	memset(data, 0, sizeof(data));
	memcpy(&size, str, sizeof(size));
	memcpy(&type, str + sizeof(size), sizeof(type));
	len = (int)size - sizeof(size) - sizeof(type) - sizeof(endmark);
	memcpy(data, str + sizeof(size) + sizeof(type), len);
	memcpy(&endmark, str + sizeof(size) + sizeof(type) + len, sizeof(endmark));
	memcpy(buf, str, size);
}

void Packet::SendMsg(char* msg)
{
	type = chat_string;
	sprintf_s(data, "%s\0", msg);
	size = (short)strlen(data) + 6;
}

void Packet::SChatMsg(char* name)
{
	char temp[DATASIZE] = {};

	RecvMsg(buf);
	sprintf(temp, "[%s]%s\0", name, data);
	sprintf(data, "%s\0", temp);
	size = (short)strlen(data) + 6;
}

void Packet::SetClose(char* msg)
{
	type = req_dis;
	sprintf_s(data, "%s\0", msg);
	size = (short)strlen(data) + 6;
}

void Packet::GetData(char* temp)
{
	char ip[16], x[16], y[16], z[16], m_temp[DATASIZE];

	switch (type)
	{
	case req_con:
		Separate(ip);
		sprintf(data, "%s", ip);
		sprintf(temp, "%s, connection success", data);
		break;
	case ack_con:
		sprintf(temp, "%s, connection completed", data);
		break;
	case req_move:
		Separate(ip, x, y, z);
		m_x = stof(x);
		m_y = stof(y);
		m_z = stof(z);
		sprintf(temp, "%s, request to move %.2f, %.2f, %.2f", ip, m_x, m_y, m_z);
		break;
	case ack_move:
		Separate(ip, x, y, z);
		m_x = stof(x);
		m_y = stof(y);
		m_z = stof(z);
		sprintf(temp, "%s, move to %.2f, %.2f, %.2f", ip, m_x, m_y, m_z);
		break;
	case chat_string:
		sprintf(temp, "%s", data);
		break;
	case req_dis:
		sprintf(temp, "%s, disconnected", data);
		break;
	case -1:
	default:
		sprintf(temp, "error");
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

int Packet::GetSize()
{
	return (int)size;
}

int Packet::GetType()
{
	return (int)type;
}

void Packet::GetPos(float* x, float* y, float* z)
{
	*x = m_x;
	*y = m_y;
	*z = m_z;
}

char* Packet::GetName()
{
	return name;
}

bool Packet::Check()
{
	return crt;
}
