#pragma once

#include "Directory.h"
#include "../Common/MyString.h"
#include "../Common/FileIO.h"
#include "Addresses.h"
#include "Globals.h"
#include "Gametypes.h"

#include <vector>

enum e_command_result;


	void Initialize();

	// Checks if the map, gametype and all scripts are valid.
	bool ValidateUserInput(const s_phasor_mapcycle_entry& entry);

	// Effectively executes sv_map to run a new game
	bool LoadGame(const s_phasor_mapcycle_entry& game);

	bool ReplaceHaloMapEntry(s_mapcycle_entry* old,	const s_phasor_mapcycle_entry& new_entry);

	// Checks if a map exists
	bool IsValidMap(const std::string& map);

	//Non-default map loading
	// --------------------------------------------------------------------
#ifdef PHASOR_PC
	// Returns the address of the loading buffer Halo should use
	char* GetLoadingMapBuffer();

	// Generates the map list
	//! \todo fix for ce
	void BuildMapList();

	// This function returns the address of our map table
	DWORD GetMapTable();

	// Called when a map is being loaded.
	void OnMapLoad(char* map);

	// Called to fix the loaded map name
	void OnNewGame();

	// Returns the base name for a map (ie bloodgulch1 -> bloodgulch)
	bool GetBaseMapName(const char* actual_map, const char** out);
#endif	

	// Get a pointer to the current map in the playlist
	s_mapcycle_entry* GetCurrentMapcycleEntry();

	//e_command_result sv_mapcycle_begin(void*, commands::CArgParser& args);
	e_command_result sv_mapcycle_begin(void*);

	//e_command_result sv_mapcycle_add(void*, 
	//	commands::CArgParser& args, COutStream& out);
	//e_command_result sv_mapcycle_del(void* exec_player, 
	//	commands::CArgParser& args, COutStream& out);
	//e_command_result sv_mapcycle(void*, 
	//	commands::CArgParser& args, COutStream& out);
	//e_command_result sv_map(void*, 
	//	commands::CArgParser& args, COutStream& out);
	//e_command_result sv_end_game(void*,
	//	commands::CArgParser& args, COutStream&);
