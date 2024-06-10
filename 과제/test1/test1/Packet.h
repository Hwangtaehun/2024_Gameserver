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
	chat_string,
	req_dis
};

class Packet {
private:
	int Input(char* value, int i, int len);
	void Separate(char* ip, char* x, char* y, char* z);
protected:
	short size;
	short type;
	short endmark;
	float m_x; 
	float m_y;
	float m_z;
	char buf[BUFSIZE + 1] = {};
	char data[DATASIZE + 1] = {};
public:
	Packet();
	~Packet();
	void RecvMsg(char* str);
	void SendMsg(char* msg);
	void SetClose(char* msg);
	void GetData(char* temp);
	char* GetBuf();
	int GetSize();
	int GetType();
	void GetPos(float* x, float* y, float* z);
};