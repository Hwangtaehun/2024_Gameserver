#include "C077044_THH.h"

int main() {
	char buf[DATASIZE];
	int type = 0;
	Packet pc = Packet();
	
	pc.setMove("127.0.0.1", 100.0f, 100.0f, 100.0f);
	pc.Print();
	sprintf_s(buf, "%s", pc.getBuf());

	Packet opc = Packet(buf);
	opc.Print();
	opc.getData();

	return 0;
}