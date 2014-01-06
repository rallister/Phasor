#pragma once

#include <string>
#include "Types.h"



	bool IsValidGametype(const std::wstring& gametype);
	bool BuildGametypeList();
	bool ReadGametypeData(const std::wstring& gametype, BYTE* out,	DWORD outSize);

	bool ListFiles(std::wstring path, std::wstring mask);
