#pragma once
#include "Packet.h"

class ClientPacket : public Packet
{
private:
	bool crt;

	int Input(char* msg, char* value, int i, int len);
	void Separate(char* msg, float* x, float* y, float* z);
public:
	ClientPacket();
	~ClientPacket();
	void SetConnect(char* ip);
	void SetMove(char* ip, char* msg);
	bool Check();
};