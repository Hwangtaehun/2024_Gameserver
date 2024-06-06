#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string>
#define BUFSIZE 512
#define DATASIZE 506

using namespace std;

enum Type {
	PK_DATA = 0
};

class Packet {
private:
	short type;
	short endmark;
	char buf[BUFSIZE + 1] = {};
	char data[DATASIZE + 1] = {};
	unsigned char m_size[2] = {};
	unsigned char m_type[2] = {};
	unsigned char m_end[2] = {};
public:
	short size;
	Packet();
	~Packet();
	void RecvMsg(char* str);
	void SendMsg(char* msg);
	void GetData();
	char* GetBuf();
	void Print();
};