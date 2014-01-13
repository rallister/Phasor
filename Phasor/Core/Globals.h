#pragma once

// defined in main.cpp

extern void _DO_TRACE(const char* fmt...);
extern FILE * _TRACE_FILE;


#define _TRACE(...) _DO_TRACE (__VA_ARGS__);
#define _TRACE_DEBUG(...) _DO_TRACE (__VA_ARGS__);
#define _TRACE_CONSOLE(...) _DO_TRACE (__VA_ARGS__);

