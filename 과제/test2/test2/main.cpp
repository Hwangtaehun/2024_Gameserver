#include "Packet.h"

int main() {
	char buf[DATASIZE];
	Packet pc = Packet();
	
	pc.SendMsg("�����ٶ󸶹ٻ�");
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