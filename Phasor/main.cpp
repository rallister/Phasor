#include <windows.h>
#include <stdio.h>
#include "main.h"

#include "Core/Directory.h"
#include "Core/Addresses.h"
#include "Core/MapLoader.h"
#include "Core/Gametypes.h"
#include "Core/CrashHandler.h"
#include "Core/Globals.h"
#include "Core/MemoryOperations.h"
#include "Common/FileIO.h"
#include "Common/MyString.h"

#define WAIT_AND_QUIT Sleep(1000); exit(1);

void LocateDirectories();

FILE* _TRACE_FILE;

void _DO_TRACE(const char *fmt, ...) {
	
	if(!_TRACE_FILE)
	{
		_TRACE_FILE = fopen("tracefile.log", "a");
		if(!_TRACE_FILE)
			return;
	}
	
    va_list args;
    va_start(args, fmt);
	vprintf(fmt, args);
    vfprintf(_TRACE_FILE, fmt, args);
    va_end(args);
	fflush(_TRACE_FILE);
}


// Called when the dll is loaded
extern "C" __declspec(dllexport) void OnLoad()
{
	srand(GetTickCount());
	//g_PrintStream.reset(new halo::CHaloPrintStream());
	//*g_PrintStream << L"44656469636174656420746f206d756d2e2049206d69737320796f752e" << endl;

	LocateDirectories();

	try
	{
		LocateAddresses();		

		CrashHandler::InstallCatchers();

		Initialize();

		BuildGametypeList();			

		InstallHooks();
			
	}
	catch (std::exception& e)
	{		
		_TRACE("\r\n%s", e.what())
		WAIT_AND_QUIT
	}
	catch (...)
	{	
		_TRACE("\r\nfail dunno")
		WAIT_AND_QUIT
	}
}

void OnServerClose()
{
	
	//halo::game::cleanupPlayers(true);

	if(_TRACE_FILE)
		fclose(_TRACE_FILE);

	ExitProcess(0);
}

// Locate and create all directories Phasor will use. If an error occurs
// this function never returns and the process is terminated.
void LocateDirectories()
{
	try 
	{
		SetupDirectories();
	}
	catch (std::exception & e)
	{
		_TRACE("\r\n%s", e.what())
		WAIT_AND_QUIT
	}	
}

// Windows entry point
BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		// Disable calls for DLL_THREAD_ATTACH and DLL_THREAD_DETACH
		DisableThreadLibraryCalls(hModule);
	}

	return TRUE;
}