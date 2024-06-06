#include "Packet.h"

int main() {
	char buf[BUFSIZE] = {};
	Packet pc = Packet();
	
	pc.SendMsg("가나다라마바사");
	pc.Print();
	memcpy(buf, pc.GetBuf(), pc.size);
	pc.~Packet();

	Packet opc = Packet();
	opc.RecvMsg(buf);
	opc.Print();
	opc.~Packet();

	return 0;
}