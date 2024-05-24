#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string>
#define BUFSIZE 512
#define DATASIZE 506

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
	char buf[DATASIZE + 1];
	unsigned char m_size[2] = {};
	unsigned char m_type[2] = {};
	unsigned char m_end[2] = {};

	void Separate(char* ip, char* x, char* y, char* z);
	int Input(char* m_value, int i, int len);
public:
	Packet();
	Packet(char* buf);
	~Packet();
	void SetConnect(char* ip);
	void SendAllConnect(char* ip);
	void SetMove(char* ip, float x, float y, float z);
	void SendAllMove(char* mes);
	void GetData();
	char* GetBuf();
	void Print();
};