// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "Menu.h"
//#include <boost/asio.hpp>
using namespace std;
//#include "Socket/chat_message.h"

//#include <deque>






DWORD WINAPI HackThread(HMODULE hModule)
{
	Menu::Init(hModule);
	Menu::HandleMsgs();


	FreeLibraryAndExitThread(hModule, 0);


	return 0;
}

static bool in = false;
int i = 0;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		DisableThreadLibraryCalls(hModule);
		HANDLE hObject = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, NULL);
		
		if (hObject) CloseHandle(hObject);
		break;
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}