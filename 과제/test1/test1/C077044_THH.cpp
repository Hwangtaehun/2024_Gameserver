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
	for (int i = len - 2; i < len; i++) {
		m_end[j] = buf[i];
		j++;
	}

	size = short(m_size[1]);
	type = short(m_type[1]);
	endmark = short(m_end[1]);
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
	type = 0;
	sprintf_s(data, "%s\0", ip);
	size = (short)strlen(data) + 6;
}

void Packet::SendAllConnect(char* ip)
{
	type = 1;
	sprintf_s(data, "%s\0", ip);
	size = (short)strlen(data) + 6;
}

void Packet::SetMove(char* ip, float x, float y, float z)
{
	type = 2;
	sprintf_s(data, "%s,%f,%f,%f\0", ip, x, y, z);
	size = (short)strlen(data) + 6;
}

void Packet::SendAllMove(char* mes)
{
	type = 3;
	sprintf_s(data, mes);
	size = (short)strlen(data) + 6;
}

void Packet::GetData()
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
		Separate(ip, x, y, z);
		printf("%s, request to move %s, %s, %s", ip, x, y, z);
		break;
	case -1:
		printf("error");
	}
}

char* Packet::GetBuf()
{
	int j = 0;

	m_size[0] = ' ';
	m_size[1] = (char)size;
	m_type[0] = ' ';
	m_type[1] = (char)type;
	m_end[0] = ' ';
	m_end[1] = (char)endmark;

	short x, y, z;
	x = (short)m_size[1];
	y = (short)m_type[1];
	z = (short)m_end[1];

	for (int i = 0; i < 2; i++) {
		buf[i] = m_size[i];
	}
	for (int i = 2; i < 4; i++) {
		buf[i] = m_type[i - 2];
	}
	for (int i = 4; i < size - 2; i++) {
		buf[i] = data[i - 4];
	}
	for (int i = size - 2; i < size; i++) {
		buf[i] = m_end[j];
		j++;
	}

	buf[size] = '\0';

	return buf;
}

void Packet::Print()
{
	printf("%hd/%hd/%s/%hd\n", size, type, data, endmark);
}
