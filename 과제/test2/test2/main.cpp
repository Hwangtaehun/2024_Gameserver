#include "Packet.h"

int main() {
	char buf[DATASIZE];
	Packet pc = Packet();
	
	pc.SendMsg("�ȳ��ϼ���.�׷���ȳ����Լ���.����������");
	pc.Print();
	sprintf_s(buf, "%s", pc.GetBuf());
	pc.~Packet();

	Packet opc = Packet(buf);
	opc.Print();
	opc.GetData();
	opc.~Packet();

	return 0;
}