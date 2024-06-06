#include "Packet.h"

int main() {
	//float x, y, z;
	char buf[BUFSIZE];
	char message[BUFSIZE];
	//int type = 0;
	Packet pc = Packet();
	
	pc.SetMove("127.0.0.1", 100.0f, 100.0f, 100.0f);
	//pc.SetConnect("127.0.0.1");
	pc.Print();
	memcpy(buf, pc.GetBuf(), pc.GetSize());
	pc.~Packet();

	Packet opc = Packet();
	opc.RecvMsg(buf);
	opc.SendAllMove();
	opc.Print();
	opc.GetData(message);
	printf("%s\n", message);
	//opc.GetTrans(&x, &y, &z);
	opc.~Packet();
	//printf("\nx = %.2f, y = %.2f, z =%.2f\n", x, y, z);

	return 0;
}