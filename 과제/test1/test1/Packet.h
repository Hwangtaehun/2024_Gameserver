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
	bool crt; //클라이언트
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
	void Separate(char* msg, float* x, float* y, float* z); //클라인언트
	int Input(char* value, int i, int len);
	int Input(char* msg, char* value, int i, int len); // 클라이언트
public:
	Packet();
	Packet(char* nickname);
	~Packet();
	void SetConnect(char* ip); //클라이언트
	void SendAllConnect(char* msg); //서버
	void SetMove(char* ip, char* msg); //클라이언트
	void SendAllMove(); //서버
	void RecvMsg(char* str); // 클라이언트 + 서버
	void SendMsg(char* msg); // 클라이언트
	void SChatMsg(char* name); // 서버
	void SetClose(char* msg); // 클라이언트 + 서버
	void GetData(char* temp); // 클라이언트 + 서버
	char* GetBuf(); // 클라이언트 + 서버
	int GetSize(); // 클라이언트 + 서버
	int GetType(); // 클라이언트 + 서버
	void GetPos(float* x, float* y, float* z); // 클라이언트 + 서버
	char* GetName(); // 클라이언트 + 서버
	bool Check(); // 클라이언트
};