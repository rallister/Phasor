
#include "../Common/Common.h"
#include "../Common/Globals.h"
#include "../Hooks/MapLoader.h"

#include "Events.h"

// D:\_git\Phasor_RA\Phasor\Phasor\Halo\Server\Server.h
void __stdcall OnMachineConnect(DWORD machineIndex)
{
	_TRACE("\r\n - OnMachineConnect") 
}

void __stdcall OnMachineDisconnect(DWORD machineIndex)
{
	_TRACE("\r\n - OnMachineDisconnect") 
}

void __stdcall OnMachineInfoFix(s_machinfo_info_partial_packet* data)
{
	_TRACE("\r\n - OnMachineInfoFix") 
}

void __stdcall ConsoleHandler(DWORD fdwCtrlType)
{
	_TRACE("\r\n - ConsoleHandler") 
}

void __stdcall OnConsoleProcessing()
{
	_TRACE("\r\n - OnConsoleProcessing") 
}

void __stdcall OnClientUpdate(s_player_structure* m_player)
{
	_TRACE("\r\n - OnClientUpdate") 
}

e_command_result __stdcall ProcessCommand(char* input)
{
	_TRACE("\r\n - ProcessCommand") 

	if(stricmp(input, "sv_mapcycle_begin") == 0)
	{
		//return StartGame(NULL);
		s_phasor_mapcycle_entry game;
		game.gametype=L"ctf";
		game.map = "ratrace";

		LoadGame(game);
		return kProcessed;
	
	}
		return kGiveToHalo;
}

void __stdcall ProcessCommandAttempt(s_command_input* input, int playerNum)
{
	_TRACE("\r\n - ProcessCommandAttempt") 
}

bool __stdcall OnMapLoad(s_mapcycle_entry* loading_map)
{
	_TRACE("\r\n - OnMapLoad") 
	return TRUE;
}

void __stdcall OnHaloPrint(char* msg)
{
	_TRACE("\r\n - OnHaloPrint") 
}

bool __stdcall OnHaloBanCheck(char* hash, s_machine_info* machine)
{
	_TRACE("\r\n - OnHaloBanCheck") 
	return TRUE;
}

void __stdcall OnHashValidation(s_hash_validation* info, const char* status)
{
	_TRACE("\r\n - OnHashValidation") 
}

void __stdcall  OnNewGame(const char* map)
{
	_TRACE("\r\n - OnNewGame") 
}

void __stdcall  OnGameEnd(DWORD mode)
{
	_TRACE("\r\n - OnGameEnd") 
}

void __stdcall OnPlayerWelcome(DWORD playerId)
{
	_TRACE("\r\n - OnPlayerWelcome") 
}

void __stdcall OnPlayerQuit(DWORD playerId)
{
	_TRACE("\r\n - OnPlayerQuit") 
}

DWORD __stdcall OnTeamSelection(DWORD cur_team, s_machine_info* machine)
{
	_TRACE("\r\n - OnTeamSelection") 
	return 0;
}

bool __stdcall OnTeamChange(DWORD playerId, DWORD team)
{
	_TRACE("\r\n - OnTeamChange") 
	return true;
}

void __stdcall OnPlayerSpawn(DWORD playerId, ident m_objectId)
{
	_TRACE("\r\n - OnPlayerSpawn") 
}

void __stdcall OnPlayerSpawnEnd(DWORD playerId, ident m_objectId)
{
	_TRACE("\r\n - OnPlayerSpawnEnd") 
}


void __stdcall OnObjectCreation(ident m_objectId)
{
	//_TRACE("\r\n - OnObjectCreation") 
}

bool __stdcall OnObjectCreationAttempt(s_object_creation_disposition* creation_info)
{
	//_TRACE("\r\n - OnObjectCreationAttempt") 
	return TRUE;
}

DWORD __stdcall OnWeaponAssignment(DWORD playerId, ident owningObjectId, s_object_info* curWeapon, DWORD order)
{
	_TRACE("\r\n - OnWeaponAssignment") 
	ident weap_id = curWeapon->id, result_id; // something.

	return weap_id;
}

bool __stdcall OnObjectInteraction(DWORD playerId, ident m_ObjId)
{
	//_TRACE("\r\n - OnObjectInteraction") 
	return TRUE;
}

void __stdcall OnChat(s_chat_data* chat)
{
	_TRACE("\r\n - OnChat") 
}

bool __stdcall OnVehicleEntry(DWORD playerId)
{
	_TRACE("\r\n - OnVehicleEntry") 
	return TRUE;
}

bool __stdcall OnVehicleEject(s_halo_biped* m_playerObject, bool forceEjected)
{
	_TRACE("\r\n - OnVehicleEject") 
	return TRUE;
}

bool __stdcall OnPlayerDeath(DWORD killerId, DWORD victimId, DWORD mode)
{
	_TRACE("\r\n - OnPlayerDeath") 
	return TRUE;
}

void __stdcall OnKillMultiplier(DWORD playerId, DWORD multiplier)
{
	_TRACE("\r\n - OnKillMultiplier") 
}


bool __stdcall OnWeaponReload(ident m_WeaponId)
{
	_TRACE("\r\n - OnWeaponReload") 
	return TRUE;
}
 

int __stdcall VehicleRespawnCheck(ident m_objId, s_halo_vehicle* obj)
{
	_TRACE("\r\n - VehicleRespawnCheck") 
	return TRUE;
}


bool __stdcall EquipmentDestroyCheck(int checkTicks, ident m_objId, s_halo_object* obj)
{
	//_TRACE("\r\n - EquipmentDestroyCheck") 

	// in game/objects.
	int objTicks = *(int*)((LPBYTE)obj + 0x204);
	return (checkTicks > objTicks);
	
}


void __stdcall OnObjectDestroy(ident m_objid)
{
	_TRACE("\r\n - OnObjectDestroy") 	
}


bool __stdcall OnDamageLookup(s_damage_info* dmg, ident* receiver)
{
	_TRACE("\r\n - OnDamageLookup") 
	return TRUE;
}


bool __stdcall OnDamageApplication(const s_damage_info* dmg, ident receiver, s_hit_info* hit, bool backtap)
{
	_TRACE("OnDamageApplication")
	return TRUE;
}

