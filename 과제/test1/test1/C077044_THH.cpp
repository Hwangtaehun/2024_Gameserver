#include "C077044_THH.h"

Packet::Packet()
{
	size = 0;
	type = -1;
	data[0] = '\0';
}

Packet::Packet(char* buf)
{
	int j = 0;
	int len = strlen(buf);

	size = 0;
	type = -1;
	data[0] = '\0';

	for (int i = 0; i < 2; i++) {
		m_size[i] = buf[i];
	}
	for (int i = 2; i < 4; i++) {
		m_type[i - 2] = buf[i];
	}
	for (int i = 4; i < len - 1; i++) {
		data[i - 4] = buf[i];
		if (len - 2 == i) {
			data[i - 4] = '\0';
		}
	}
	for (int i = len - 1; i < len + 1; i++) {
		m_end[j] = buf[i];
		j++;
	}
	

	size = (static_cast<unsigned char>(m_size[1]) << 8) | static_cast<unsigned char>(m_size[0]);
	type = (static_cast<unsigned char>(m_type[1]) << 8) | static_cast<unsigned char>(m_type[0]);
	endmark = (static_cast<unsigned char>(m_end[1]) << 8) | static_cast<unsigned char>(m_end[0]);
}

Packet::~Packet(){}

void Packet::separate(char* ip, char* x, char* y, char* z)
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
			i = input(ip, i, len);
			break;
		case 1:
			i = input(x, i, len);
			break;
		case 2:
			i = input(y, i, len);
			break;
		case 3:
			i = input(z, i, len);
			break;
		}

		cnt++;
	}
}

void Packet::makeBuf()
{
	m_size[0] = size & 0xFF;
	m_size[1] = (size >> 8) & 0xFF;
	m_type[0] = type & 0xFF;
	m_type[1] = (type >> 8) & 0xFF;
	m_end[0] = endmark & 0xFF;
	m_end[1] = (endmark >> 8) & 0xFF;
	printf("m_end¿« ≈©±‚: %d\n", strlen(m_end));
	sprintf_s(buf, "%2s%2s%s%2s\0", m_size, m_type, data, m_end);
}

int Packet::input(char* m_value, int i, int len)
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

void Packet::setConnect(char* ip)
{
	type = 0;
	sprintf_s(data, "%s\0", ip);
	size = (short)strlen(data) + 6;
	makeBuf();
}

void Packet::sendAllConnect(char* ip)
{
	type = 1;
	sprintf_s(data, "%s\0", ip);
	size = (short)strlen(data) + 6;
	makeBuf();
}

void Packet::setMove(char* ip, float x, float y, float z)
{
	type = 2;
	sprintf_s(data, "%s,%f,%f,%f\0", ip, x, y, z);
	size = (short)strlen(data) + 6;
	makeBuf();
}

void Packet::sendAllMove(char* mes)
{
	type = 3;
	sprintf_s(data, mes);
	size = (short)strlen(data) + 6;
	makeBuf();
}

void Packet::getData()
{
	switch (type)
	{
	case req_con:
		printf("%s, connection success", data);
		break;
	case ack_con:
		printf("%s, connection completed", data);
		break;
	case req_move:
	case ack_move:
		char ip[16], x[16], y[16], z[16];
		separate(ip, x, y, z);
		printf("%s, request to move %s, %s, %s", ip, x, y, z);
		break;
	case -1:
		printf("error");
	}
}

void Packet::Print()
{
	printf("%hd/%hd/%s/%hd\n", size, type, data, endmark);
}
