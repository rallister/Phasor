#pragma once

#include "Logging.h"
#include "PhasorThread.h"

class CLogThreadEvent;

class CThreadedLogging : public CLoggingStream
{
private:
	CRITICAL_SECTION cs;
	DWORD id;
	PhasorThread& thread;
	std::list<std::wstring> lines;

	void Initialize();
	void LogLinesLocked();

protected:
	virtual bool Write(const std::wstring& str);

public:
	CThreadedLogging(const std::wstring& dir, const std::wstring& file,
		PhasorThread& thread);
	CThreadedLogging(const std::wstring& file, PhasorThread& thread);
	CThreadedLogging(const CLoggingStream& stream, PhasorThread& thread);
	virtual ~CThreadedLogging();

	friend class CLogThreadEvent;
};

class CLogThreadEvent : public PhasorThreadEvent
{
private:
	friend class CThreadLogging;
	CThreadedLogging& owner;

	CLogThreadEvent(CThreadedLogging& owner);

public:
	virtual void OnEventAux(PhasorThread& thread);

	friend class CThreadedLogging;
};