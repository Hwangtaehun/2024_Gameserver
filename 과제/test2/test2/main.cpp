#include "Packet.h"

int main() {
	char buf[DATASIZE];
	Packet pc = Packet();
	
	pc.SendMsg("안녕하세요.그러면안녕히게세요.ㅋㅋㅋㅋㅋ");
	pc.Print();
	sprintf_s(buf, "%s", pc.GetBuf());
	pc.~Packet();

	Packet opc = Packet(buf);
	opc.Print();
	opc.GetData();
	opc.~Packet();

	return 0;
}