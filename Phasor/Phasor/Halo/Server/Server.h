//! \file Server.h
#pragma once

#include "../../../Common/Types.h"
#include "MapLoader.h"

// declared in ../../Commands.h
enum e_command_result;

namespace halo { 
	struct s_player;
	struct s_player_structure;
namespace server
{
	//---------------------------------------------------------------------
	// DEFINITIONS
	// 


	struct PhasorMachine
	{
		s_machine_info* machine;
		bool hash_validated;

		PhasorMachine(s_machine_info* machine);
		~PhasorMachine();
	};

	class SayStreamRaw : public COutStream
	{
	protected:
		virtual bool Write(const std::wstring& str);

	public:
		SayStreamRaw() {}

		virtual std::unique_ptr<COutStream> clone() const override
		{
			return std::unique_ptr<COutStream>(new SayStreamRaw());
		}
	};

	// Stream for sending server messages with **SERVER** prepended
	class SayStream : public SayStreamRaw
	{
	protected:
		virtual bool Write(const std::wstring& str);

	public:
		SayStream() {}

		virtual std::unique_ptr<COutStream> clone() const override
		{
			return std::unique_ptr<COutStream>(new SayStream());
		}
	};

	//! Stream used for server messages.
	extern SayStream say_stream;
	extern SayStreamRaw say_stream_raw;

	void acceptInvalidHashes(bool state);
	bool getInvalidHashState();

	/*! \brief Send a chat message to the player
	 *	\param player The player to send the message to
	 *	\param str The message to send.	 */
	void MessagePlayer(const s_player& player, const std::wstring& str);
	
	/*! \brief Send a console message to the player
	 *	\param player The player to send the message to
	 *	\param str The message to send.	 */
	bool ConsoleMessagePlayer(const s_player& player, const std::wstring& str);
	
	/*! \brief Notifies the server that a player has changed team (syncs it)
	 *	\param player The player who changed team. */
	void NotifyServerOfTeamChange(const halo::s_player& player);

	void ExecuteServerCommand(const std::string& command, s_player* execute_as);

	// Gets the player's ip
	bool GetPlayerIP(const s_player& player, std::string* ip, WORD* port);
	bool GetMachineIP(s_machine_info& machine, std::string* ip, WORD* port);
	
	// Gets the player's hash
	bool GetPlayerHash(const s_player& player, std::string& hash);
	bool GetMachineHash(const s_machine_info& machine, std::string& hash);
	
	PhasorMachine* FindMachine(const s_player& player);
	PhasorMachine* FindMachineById(DWORD machineId);
	PhasorMachine* FindMachineByIndex(DWORD index);

	/*! \brief Get the player executing the current command
	 * \return The player executing the command, or NULL if no player. */
	halo::s_player* GetPlayerExecutingCommand();

	void SetExecutingPlayer(halo::s_player* player);

	/*! \brief Get the server struct
		\return The server struct.*/
	s_server_info* GetServerStruct();

	DWORD GetServerTicks();
	DWORD GetRespawnTicks();

	e_command_result sv_quit(void*, 
		commands::CArgParser&, COutStream&);

	// --------------------------------------------------------------------
	// Events
	// 


} }