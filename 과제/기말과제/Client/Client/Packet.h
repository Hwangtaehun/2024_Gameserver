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
	bool crt;
	short size;
	short type;
	short endmark;
	float m_x; 
	float m_y;
	float m_z;
	char name[21] = {};
	char buf[BUFSIZE + 1] = {};
	char data[DATASIZE + 1] = {};

	void Separate(char* ip);
	void Separate(char* ip, char* x, char* y, char* z);
	void Separate(char* data, float* x, float* y, float* z);
	int Input(char* value, int i, int len);
	int Input(char* msg, char* value, int i, int len);
public:
	Packet();
	Packet(char* nickname);
	~Packet();
	void SetConnect(char* ip);
	void SendAllConnect(char* msg);
	void SetMove(char* ip, char* msg);
	void SendAllMove();
	void RecvMsg(char* str);
	void SendMsg(char* msg);
	void SChatMsg(int port);
	void SetClose(char* msg);
	void GetData(char* temp);
	char* GetBuf();
	int GetSize();
	int GetType();
	void GetPos(float* x, float* y, float* z);
	char* GetName();
	bool Check();
};