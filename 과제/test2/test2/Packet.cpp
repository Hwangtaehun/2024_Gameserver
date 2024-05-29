#include "Packet.h"

Packet::Packet()
{
	size = 0;
	type = -1;
	endmark = 0xFF;
}

Packet::Packet(char* str)
{
	size = 0;
	type = -1;
	endmark = 0xFF;

	trans(str);
}

Packet::~Packet(){}

void Packet::trans(char* str)
{
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

void Packet::SendMsg(char* msg)
{
	type = PK_DATA;
	sprintf_s(data, "%s\0", msg);
	size = (short)(strlen(data) + 6);
}

char* Packet::GetData()
{
	switch (type)
	{
	case PK_DATA:
		return data;
		break;
	case -1:
	default:
		return "error";
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

void Packet::SetBuf(char* str)
{
	trans(str);
}

void Packet::Print()
{
	printf("%hd/%hd/%s/%hd\n", size, type, data, endmark);
}
