#pragma once

#include "MemoryOperations.h"
#include "Globals.h"
#include "MapLoader.h"
#include "tags.h"
#include "Core.h"

#include "Addresses.h"
#include "Types.h"

// D:\_git\Phasor_RA\Phasor\Phasor\Halo\Server\Server.h

void __stdcall OnMachineConnect(DWORD machineIndex);
void __stdcall OnMachineDisconnect(DWORD machineIndex);
void __stdcall OnMachineInfoFix(s_machinfo_info_partial_packet* data);
	
// Called for console events (exit etc)
/*! \brief Called for Windows related console events (ie closing the server)
	*	\param fdwCtrlType The type of event. */
void __stdcall ConsoleHandler(DWORD fdwCtrlType);

/*! \brief Called every cycle to read input from the user. I use it for timers. */
void __stdcall OnConsoleProcessing();

/*! \brief Called to process a server command, after the password has been validated.
	*	\param command The command being executed.
	*	\return Value indicating whether or not Halo should process the event.*/
e_command_result __stdcall ProcessCommand(char* command);

void __stdcall ProcessCommandAttempt(s_command_input* input, int playerNum);

/*! \brief Called when a new game starts.
	*	\param map The map the game is running.*/
void __stdcall OnNewGame(const char* map);

/*! \brief Called when a game stage ends.
	*	\param mode Specifies which stage is ending (game, post-game, scorecard) */
void __stdcall OnGameEnd(DWORD mode);

/*! \brief Called when a map is being loaded.
	*	\param loading_map The map being loaded, which can be changed.
	*	\return Boolean indicating whether or not the map was changed.*/
bool __stdcall OnMapLoad(s_mapcycle_entry* loading_map);

/*! \brief Called when halo wants to print a message to the console.*/
void __stdcall OnHaloPrint(char* msg);

/*! \brief Called when halo checks if the specified hash is banned.
	*	\param hash The hash being checked.
	*	\return Boolean indicating whether or not the player is allowed to join.*/
bool __stdcall OnHaloBanCheck(char* hash, s_machine_info* machine);

// Called once Halo has received the hash-checking response from gamespy
void __stdcall OnHashValidation(s_hash_validation* info, const char* status);

// Called when the server info is about to be broadcast
//bool __stdcall OnVersionBroadcast(DWORD arg1, DWORD arg2);

// Called when a player joins (after verification).
void __stdcall OnPlayerWelcome(DWORD playerId);

// Called when a player quits
void __stdcall OnPlayerQuit(DWORD playerId);

// Called when a player's team is being assigned
DWORD __stdcall OnTeamSelection(DWORD cur_team, s_machine_info* machine);

// Called when a player tries to change team
bool __stdcall OnTeamChange(DWORD playerId, DWORD team);

// Called when a player is about to spawn (object already created)
void __stdcall OnPlayerSpawn(DWORD playerId, ident m_objectId);

// Called after the server has been notified of a player spawn
void __stdcall OnPlayerSpawnEnd(DWORD playerId, ident m_objectId);

// Called when a weapon is created
void __stdcall OnObjectCreation(ident m_objectId);

bool __stdcall OnObjectCreationAttempt(s_object_creation_disposition* creation_info);

// Called when a weapon is assigned to an object
DWORD __stdcall OnWeaponAssignment(DWORD playerId, ident owningObjectId, s_object_info* curWeapon, DWORD order);

// Called when a player can interact with an object
bool __stdcall OnObjectInteraction(DWORD playerId, ident m_ObjId);

// Called when someone chats in the server
void __stdcall OnChat(s_chat_data* chat);

// Called when a player attempts to enter a vehicle
bool __stdcall OnVehicleEntry(DWORD playerId);

// Called when a player is being ejected from a vehicle
bool __stdcall OnVehicleEject(s_halo_biped* m_playerObject, bool forceEjected);

// Called when a player dies
bool __stdcall OnPlayerDeath(DWORD killerId, DWORD victimId, DWORD mode);

// Called when a player gets a double kill, spree etc
void __stdcall OnKillMultiplier(DWORD playerId, DWORD multiplier);

// Called when a weapon is reloaded
bool __stdcall OnWeaponReload(ident m_WeaponId); 
	
//objects D:\_git\Phasor_RA\Phasor\Phasor\Halo\Game\Objects.h
// Called when an object is being checked to see if it should respawn
int __stdcall VehicleRespawnCheck(ident m_objId, s_halo_vehicle* obj);

// This is called when weapons/equipment are going to be destroyed.
// todo: check ticks should be signed
bool __stdcall EquipmentDestroyCheck(int checkTicks, ident m_objId, s_halo_object* obj);

// Called when an object is being destroyed
void __stdcall OnObjectDestroy(ident m_objid);

// D:\_git\Phasor_RA\Phasor\Phasor\Halo\Game\Damage.h

// Called when an object's damage is being looked up
bool __stdcall OnDamageLookup(s_damage_info* dmg, ident* receiver);

// Called when damage is being applied to an object
bool __stdcall OnDamageApplication(const s_damage_info* dmg, ident receiver, s_hit_info* hit, bool backtap);

/*! \brief Called when a client sends its update packet (new pos, fire etc)
	*	\param m_player The memory address of the player who is updating.*/
void __stdcall OnClientUpdate(s_player_structure* m_player);



