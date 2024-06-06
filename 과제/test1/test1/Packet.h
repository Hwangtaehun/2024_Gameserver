#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string>
#define BUFSIZE 512
#define DATASIZE 506

using namespace std;

enum Type {
	req_con = 0,
	ack_con,
	req_move,
	ack_move,
	chat_string
};

class Packet {
private:
	short size;
	short type;
	short endmark;
	float m_x; 
	float m_y;
	float m_z;
	char buf[BUFSIZE + 1] = {};
	char data[DATASIZE + 1] = {};
	unsigned char m_size[2] = {};
	unsigned char m_type[2] = {};
	unsigned char m_end[2] = {};

	void Separate(char* ip, char* x, char* y, char* z);
	int Input(char* m_value, int i, int len);
public:
	Packet();
	~Packet();
	void SetConnect(char* ip);
	void SendAllConnect();
	void SetMove(char* ip, float x, float y, float z);
	void SendAllMove();
	void GetData(char* temp);
	char* GetBuf();
	void RecvMsg(char* str);
	int GetSize();
	void Print();
};