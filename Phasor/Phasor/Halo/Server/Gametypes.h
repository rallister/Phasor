#pragma once

#include <string>
#include "../../../Common/Types.h"

namespace halo { namespace server { namespace gametypes {

	bool IsValidGametype(const std::wstring& gametype);
	bool BuildGametypeList();
	bool ReadGametypeData(const std::wstring& gametype, BYTE* out,
		DWORD outSize);

	bool ListFiles(std::wstring path, std::wstring mask);

}}}