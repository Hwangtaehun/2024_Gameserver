#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string>
#define BUFSIZE 512
#define DATASIZE 506

using namespace std;

enum Type {
	PK_DATA = 1
};

class Packet {
private:
	short size;
	short type;
	short endmark;
	char buf[BUFSIZE + 1] = {};
	char data[DATASIZE + 1] = {};
	unsigned char m_size[2] = {};
	unsigned char m_type[2] = {};
	unsigned char m_end[2] = {};

	void trans(char* str);
public:
	Packet();
	Packet(char* buf);
	~Packet();
	void SendMsg(char* msg);
	char* GetData();
	char* GetBuf();
	void SetBuf(char* str);
	void Print();
};