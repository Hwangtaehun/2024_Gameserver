#pragma once
#define _CRT_SECURE_NO_WARNINGS
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
public:
	Packet();
	~Packet();
	void RecvMsg(char* str);
	void SendMsg(char* msg);
	void GetData(char* msg);
	char* GetBuf();
	void Print();
};