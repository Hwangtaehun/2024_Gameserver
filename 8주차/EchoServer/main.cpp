#define _CRT_SECURE_NO_WARNINGS
#include "EchoServer.h"
int main(int argc, char* argv[])
{
	EchoServer Echo;
	Echo.Init();
	Echo.A();
	Echo.B();
	Echo.C();
	Echo.D();
	Echo.E();
	return 0;
}