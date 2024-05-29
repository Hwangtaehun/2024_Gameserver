#include "Packet.h"

int main() {
	char buf[DATASIZE];
	Packet pc = Packet();
	
	pc.SendMsg("가나다라마바사");
	pc.Print();
	sprintf_s(buf, "%s", pc.GetBuf());
	pc.~Packet();

	Packet opc = Packet(buf);
	opc.Print();
	sprintf_s(buf, "%s", opc.GetData());
	opc.~Packet();
	printf("buf = %s\n", buf);

	return 0;
}