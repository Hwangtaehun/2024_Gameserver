#include "Packet.h"

int main() {
	char buf[DATASIZE];
	int type = 0;
	Packet pc = Packet();
	
	pc.SetMove("127.0.0.1", 100.0f, 100.0f, 100.0f);
	pc.Print();
	sprintf_s(buf, "%s", pc.GetBuf());

	Packet opc = Packet(buf);
	opc.Print();
	opc.GetData();

	return 0;
}