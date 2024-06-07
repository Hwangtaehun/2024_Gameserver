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
	void Separate(char* data, float* x, float* y, float* z);
	int Input(char* value, int i, int len);
	int Input(char* msg, char* value, int i, int len);
public:
	Packet();
	~Packet();
	void SetConnect(char* ip);
	void SendAllConnect();
	void SetMove(char* ip, char* msg);
	void SendAllMove();
	void RecvMsg(char* str);
	void SendMsg(char* msg);
	void GetData(char* temp);
	char* GetBuf();
	int GetSize();
	void GetPos(float* x, float* y, float* z);
	void Print();
};