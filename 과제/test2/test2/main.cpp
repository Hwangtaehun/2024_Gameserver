#include "Packet.h"

int main() {
	char buf[BUFSIZE] = {};
	Packet pc = Packet();
	
	pc.SendMsg("�����ٶ󸶹ٻ�");
	pc.Print();
	memcpy(buf, pc.GetBuf(), pc.size);
	pc.~Packet();

	Packet opc = Packet();
	opc.RecvMsg(buf);
	opc.Print();
	opc.~Packet();

	return 0;
}