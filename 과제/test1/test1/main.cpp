#include "Packet.h"
#define SERVERIP   "127.0.0.1"

int main() {
	float x, y, z;
	char buf[BUFSIZE];
	char message[BUFSIZE];
	//int type = 0;
	Packet pc = Packet();
	
	pc.SetMove(SERVERIP, "100.0f,100.0f,100.0f");
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
	opc.GetPos(&x, &y, &z);
	opc.~Packet();
	printf("\nx = %.2f, y = %.2f, z =%.2f\n", x, y, z);

	return 0;
}