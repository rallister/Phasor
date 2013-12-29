#pragma once

#include "Logging.h"
#include "GameLogging.h"
#include "PhasorThread.h"
#include "Halo/HaloStreams.h"
#include "../Common/Timers.h"

// defined in main.cpp
extern PhasorThread g_Thread;
extern Timers g_Timers; 
extern std::unique_ptr<CGameLog> g_GameLog;
extern std::unique_ptr<CScriptsLog> g_ScriptsLog;
extern std::unique_ptr<CPhasorLog> g_PhasorLog;
extern std::unique_ptr<CRconLog> g_RconLog;
extern std::unique_ptr<halo::CHaloPrintStream> g_PrintStream;

extern void _DO_TRACE(const char* fmt...);
extern FILE * _TRACE_FILE;

#ifndef _TRACE

#define _TRACE(...) _DO_TRACE (__VA_ARGS__);

#endif // !__DO_TRACE
