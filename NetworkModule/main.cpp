#include "ClientNetwork.h"
#include <process.h>
#include <ctime>

const int TIME_TO_DISCONNECT = 1;

void clientLoop1(void*);
void clientLoop2(void);
	ClientNetwork cN;
	//ClientNetwork cN2;
int main()
{	
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	char* ip = "192.168.1.141";
	bool result;
	
	cN.Initialize();
	//cN2.Initialize();
	//cN.Join(ip);
	clientLoop1(NULL);
	
	cN.Shutdown();
	// create thread with arbitrary argument for the run function
	//HANDLE serverThread = (HANDLE)_beginthread(clientLoop1, 0, (void*)12);

	system("pause");	

}

void clientLoop1(void*)
{
	std::clock_t start;
	double duration = 0;

	start = std::clock();

	while (duration < 100)
	{
		cN.Update();
		duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	}
	
	printf("Client loop stoped\n");
}

void clientLoop2()
{

}