#pragma once

#include <string>

namespace halo { 
	struct s_player;
	namespace server { namespace chat {

	



	void DispatchChat(e_chat_types type, const wchar_t* msg, 
		const s_player* from=NULL, const s_player* to=NULL);
}}}

