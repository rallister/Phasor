#pragma once

#include "../../../Common/Types.h"
#include "Objects.h"
#include "../Player.h"

namespace halo { 
	struct ident;
	struct s_object_info;
	struct s_tag_entry;

	namespace server {
		struct s_machine_info;
		namespace chat {
			struct s_chat_data;
		}
	}
	
	namespace game {
		struct s_chat_data;

	s_player* getPlayer(int index);
	s_player* getPlayerFromRconId(unsigned int playerNum);
	s_player* getPlayerFromAddress(s_player_structure* player);
	s_player* getPlayerFromObject(s_halo_biped* obj);
	s_player* getPlayerFromObjectId(ident id);
	s_player* getPlayerFromHash(const std::string& hash);

	void cleanupPlayers(bool notify_scripts);

	// --------------------------------------------------------------------
	// Events

	// Called when a player's position is updated
	void OnClientUpdate(s_player& player);
}}