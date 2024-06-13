#include "ClientPacket.h"
#include "ServerPacket.h"
#define SERVERIP   "127.0.0.1"

//int main() {
//	float x, y, z;
//	char buf[BUFSIZE];
//	char message[BUFSIZE];
//	//int type = 0;
//	Packet pc = Packet();
//	
//	pc.SetMove(SERVERIP, "100.0f,100.0f,100.0f");
//	//pc.SetConnect("127.0.0.1");
//	pc.Print();
//	memcpy(buf, pc.GetBuf(), pc.GetSize());
//	pc.~Packet();
//
//	Packet opc = Packet();
//	opc.RecvMsg(buf);
//	opc.SendAllMove();
//	opc.Print();
//	opc.GetData(message);
//	printf("%s\n", message);
//	opc.GetPos(&x, &y, &z);
//	opc.~Packet();
//	printf("\nx = %.2f, y = %.2f, z =%.2f\n", x, y, z);
//
//	return 0;
//}

//int main() {
//	Packet pc = Packet();
//	char buf[BUFSIZE];
//
//	pc.SendMsg("가나다");
//	pc.Print();
//	memcpy(buf, pc.GetBuf(), pc.GetSize());
//	pc.~Packet();
//
//	Packet opc = Packet();
//	opc.RecvMsg(buf);
//	opc.GetData(buf);
//	printf("%s\n", buf);
//
//	return 0;
//}

int main() {
	char buf[BUFSIZE] = {};
	char buf2[BUFSIZE] = {};
	ClientPacket cp = ClientPacket("AA");
	ServerPacket sp = ServerPacket();

	//접속
	cp.SetConnect(SERVERIP);
	memcpy(buf, cp.GetBuf(), cp.GetSize());

	sp.RecvMsg(buf);
	sp.GetData(buf2);
	printf("연결: %s\n", buf2);

	//클라이언트 접속 전체 전송
	sp.SendAllConnect(buf2);
	memcpy(buf2, sp.GetBuf(), sp.GetSize());

	cp.RecvMsg(buf2);
	cp.GetData(buf);
	printf("전체 연결: %s\n", buf);

	//클라이언트 이동
	cp.SetMove(SERVERIP, "100,100,100");
	memcpy(buf, cp.GetBuf(), cp.GetSize());

	sp.RecvMsg(buf);
	sp.GetData(buf2);
	printf("클라이언트 이동: %s\n", buf2);

	//서버가 모든 클라이언트에게 전송
	sp.SendAllMove();
	memcpy(buf2, sp.GetBuf(), sp.GetSize());

	cp.RecvMsg(buf2);
	cp.GetData(buf);
	printf("전체 이동: %s\n", buf);

	return 0;
}