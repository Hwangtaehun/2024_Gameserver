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

Packet::Packet(char* str)
{
	size = 0;
	type = -1;
	endmark = 0xFF;
	m_x = 0.0f;
	m_y = 0.0f;
	m_z = 0.0f;

	int j = 0;
	int len = strlen(str);

	for (int i = 0; i < 2; i++) {
		m_size[i] = str[i];
	}
	for (int i = 2; i < 4; i++) {
		m_type[i - 2] = str[i];
	}
	for (int i = 4; i < len - 1; i++) {
		data[i - 4] = str[i];
		if (len - 2 == i) {
			data[i - 4] = '\0';
		}
	}
	for (int i = len - 2; i < len; i++) {
		m_end[j] = str[i];
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

void Packet::SendAllConnect(char* ip)
{
	type = ack_con;
	sprintf_s(data, "%s\0", ip);
	size = (short)strlen(data) + 6;
}

void Packet::SetMove(char* ip, float x, float y, float z)
{
	type = req_move;
	sprintf_s(data, "%s,%.2f,%.2f,%.2f\0", ip, x, y, z);
	size = (short)strlen(data) + 6;
}

void Packet::SendAllMove(char* mes)
{
	type = ack_move;
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
		m_x = stof(x);
		m_y = stof(y);
		m_z = stof(z);
		printf("%s, request to move %.2f, %.2f, %.2f", ip, m_x, m_y, m_z);
		break;
	case -1:
		printf("error");
	}
}

void Packet::GetTrans(float* x, float* y, float* z)
{
	*x = m_x;
	*y = m_y;
	*z = m_z;
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
