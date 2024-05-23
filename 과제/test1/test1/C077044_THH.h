#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DATASIZE 512

using namespace std;

enum Type {
	req_con,
	ack_con,
	req_move,
	ack_move
};

class Packet {
private:
	short size;
	short type;
	short endmark = 0xFF;
	char data[DATASIZE + 1];
	char m_size[2] = {};
	char m_type[2] = {};
	char m_end[2] = {};

	void separate(char* ip, char* x, char* y, char* z);
	void makeBuf();
	int input(char* m_value, int i, int len);
public:
	char buf[DATASIZE + 1];

	Packet();
	Packet(char* buf);
	~Packet();
	void setConnect(char* ip);
	void sendAllConnect(char* ip);
	void setMove(char* ip, float x, float y, float z);
	void sendAllMove(char* mes);
	void getData();
	void Print();
};