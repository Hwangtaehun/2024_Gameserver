#include "C077044_THH.h"

int main() {
	int type = 0;
	Packet pc = Packet();

	pc.setMove("127.0.0.1", 100.0f, 100.0f, 100.0f);
	printf("%s, %d", pc.buf, strlen(pc.buf));

	Packet m_pc = Packet(pc.buf);
	m_pc.Print();

	return 0;
}