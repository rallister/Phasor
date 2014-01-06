#pragma once

#include <windows.h>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <deque>
#include <memory>

#ifdef PHASOR_PC
	#define CONST_MENTRY_SIZE				0x0A4 // size of each entry in map cycle
	#define GAMET_BUFFER_SIZE				0x098	
#elif PHASOR_CE
	#define CONST_MENTRY_SIZE				0x0E4
	#define GAMET_BUFFER_SIZE				0x0D8	
#endif
static_assert(GAMET_BUFFER_SIZE + 0x0C == CONST_MENTRY_SIZE, "incorrect mapcycle sizes");

// Offsets
#define OFFSET_CONSOLETEXT					0x0B4
#define OFFSET_RESPAWNTICKS					0x068
#ifdef PHASOR_PC
	#define MACHINE_ENTRY_SIZE				0x060
#elif PHASOR_CE
	#define MACHINE_ENTRY_SIZE				0x0EC
#endif	



// Memory commands
BOOL WriteBytes(DWORD dwAddress, const LPVOID lpBuffer, DWORD dwCount);
BOOL WriteString(DWORD dwAddress, const char* str);
BOOL ReadBytes(DWORD dwAddress, LPVOID lpBuffer, DWORD dwCount);
std::vector<DWORD> FindSignatures(LPBYTE sigBuffer, LPBYTE sigWildCard,
	DWORD sigSize, LPBYTE pBuffer, DWORD size);
bool FindSignature(LPBYTE sigBuffer, LPBYTE sigWildCard, 
	DWORD sigSize, LPBYTE pBuffer, DWORD size, DWORD occurance, DWORD& result);
BOOL CreateCodeCave(DWORD dwAddress, BYTE cbSize, VOID (*pFunction)());


void LocateAddresses();
void InstallHooks();
