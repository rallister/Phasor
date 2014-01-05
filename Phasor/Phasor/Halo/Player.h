#pragma once

#include "../../Common/Types.h"
#include "../../Common/Streams.h"
#include "../../Common/noncopyable.h"
#include "Halo.h"
#include "Game/Objects.h"
#include "HaloStreams.h"
#include <string>
#include <memory>

namespace halo
{
	namespace afk_detection { class CAFKDetection; }
	struct s_player;

	// ------------------------------------------------------------------

	struct s_player : private noncopyable
	{
		std::string hash, ip;
		WORD port;
		int memory_id;
		bool is_admin, authenticating_hash, sv_killed, force_entered;
		s_player_structure* mem;
		std::unique_ptr<afk_detection::CAFKDetection> afk;
		std::unique_ptr<PlayerConsoleStream> console_stream;
		std::unique_ptr<PlayerChatStream> chat_stream;

		// ----------------------------------------------------------------
		explicit s_player(int memory_id);
		~s_player();

		s_halo_biped* get_object() const;
		void Kick() const;
		void ChangeTeam(BYTE new_team, bool forcekill=true);
		void Kill();
		void ApplyCamo(float duration) const;
		void SetSpeed(float speed) const;
		void checkAndSetAdmin();
		bool InVehicle() const; 
		ident getPlayerIdent() const;
	};

	s_player_structure* GetPlayerMemory(int index);
	
	// -----------------------------------------------------------------

}