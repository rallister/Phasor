#include <windows.h>
#include <stdio.h>
#include "main.h"

#include "Core/Directory.h"
#include "Core/Addresses.h"
#include "Core/MapLoader.h"
#include "Core/Gametypes.h"
//#include "Core/CrashHandler.h"
#include "Core/Globals.h"
#include "Core/MemoryOperations.h"
#include "Common/FileIO.h"
#include "Common/MyString.h"
#include "Core/Version.h"
#include <DbgHelp.h>

#define WAIT_AND_QUIT Sleep(1000); exit(1);

//================================================================================
FILE* _TRACE_FILE;

void _DO_TRACE(const char *fmt, ...) 
{	
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
//================================================================================


//=============================================== was in CrashHandler =================================
#pragma comment(lib, "DbgHelp.lib")
// Handler for all unhandled exceptions
LONG WINAPI OnUnhandledException(PEXCEPTION_POINTERS pExceptionInfo);
bool bPassOn = true;

DWORD halo_seh_ret = 0;

__declspec(naked) void HaloExceptionHandler()
{
	__asm
	{
		pop halo_seh_ret

		pushad
		push 0
		call OnUnhandledException
		popad

		PUSH EBP
		MOV EBP,ESP
		SUB ESP,0x8

		push halo_seh_ret
		ret
	}
}

// This installs the exception catches (globally and through hooking Halo's).
void InstallCatchers()
{
	SetUnhandledExceptionFilter(OnUnhandledException);

	// Hook Halo's exception handler
	CreateCodeCave(CC_EXCEPTION_HANDLER, 6, HaloExceptionHandler);
}

LONG WINAPI OnUnhandledException(PEXCEPTION_POINTERS pExceptionInfo)
{
	if (!bPassOn) // exception occurred while closing, force kill the server
		ExitProcess(1);

	bPassOn = false;

	MINIDUMP_EXCEPTION_INFORMATION ei;
	ei.ExceptionPointers = pExceptionInfo;
	ei.ThreadId = GetCurrentThreadId();
	ei.ClientPointers = FALSE;

	DWORD dwProcessId = GetCurrentProcessId();
	SYSTEMTIME stLocalTime;
	GetLocalTime(&stLocalTime);
	wchar_t CrashDumpW[1024];
	swprintf_s(CrashDumpW, NELEMS(CrashDumpW), 
			L"%s\\%s-%s-%s-%04X-%04d%02d%02d-%02d%02d%02d-%ld-%ld.dmp", 
			g_CrashDirectory.c_str(), L"Phasor", PHASOR_HALO_BUILD,
			PHASOR_MAJOR_VERSION_STR, 
			PHASOR_INTERNAL_VERSION, stLocalTime.wYear, stLocalTime.wMonth,
			stLocalTime.wDay, stLocalTime.wHour, stLocalTime.wMinute, 
			stLocalTime.wSecond, dwProcessId, ei.ThreadId);

	HANDLE hFile = CreateFileW(CrashDumpW, GENERIC_READ | GENERIC_WRITE,
		NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

#ifdef BUILD_DEBUG
	MINIDUMP_TYPE dwDumpType = MiniDumpWithFullMemory;
#else
	MINIDUMP_TYPE dwDumpType = MiniDumpScanMemory;
#endif

	MiniDumpWriteDump(GetCurrentProcess(), dwProcessId, hFile, 
		dwDumpType, &ei, NULL, NULL);
	CloseHandle(hFile);

	return EXCEPTION_EXECUTE_HANDLER;
}
//================================================================================

// Called when the dll is loaded
extern "C" __declspec(dllexport) void OnLoad()
{
	srand(GetTickCount());
	//g_PrintStream.reset(new halo::CHaloPrintStream());
	//*g_PrintStream << L"44656469636174656420746f206d756d2e2049206d69737320796f752e" << endl;

	try
	{
		InstallCatchers();

		SetupDirectories();
		
		LocateAddresses();		

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
	if(_TRACE_FILE)
		fclose(_TRACE_FILE);

	ExitProcess(0);
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