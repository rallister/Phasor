
#include "Events.h"

// D:\_git\Phasor_RA\Phasor\Phasor\Halo\Server\Server.h
void __stdcall OnMachineConnect(DWORD machineIndex)
{
	s_server_info* ss = GetServerStruct();
	
	_TRACE("\r\n - OnMachineConnect %d", ss->cur_players) 
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
	// used only when closing server
	// dunno what it is for.
	_TRACE("\r\n - ConsoleHandler") 
}

void __stdcall OnConsoleProcessing()
{
	// i wonder what else does this thread do, i'd like to kill it.
	//_TRACE("\r\n - OnConsoleProcessing") 
}

void __stdcall OnClientUpdate(s_player_structure* m_player)
{
	//s_halo_biped* object =  (s_halo_biped*)GetObjectAddress(m_player->object_id);
	//if (object)	{
	//	
	//	vect3d new_camera = object->cameraView;
	//	
	//	// check if the camera has moved
	//	if (new_camera != m_player->oldCamera) // oldCamera added to unknowns 12 bytes.
	//	{	_TRACE("\r\n - %f, %f, %f", new_camera.x,new_camera.y, new_camera.z);
	//		m_player->oldCamera = new_camera;
	//	}

	//	// Check if the player is shooting/throwing nades etc, if so they are not afk
	//	if (object->actionFlags.melee || 
	//		object->actionFlags.primaryWeaponFire ||
	//		object->actionFlags.secondaryWeaponFire)
	//		;//MarkPlayerActive();
	//} 
}

e_command_result __stdcall ProcessCommand(char* input)
{
	_TRACE("\r\n - ProcessCommand") 
	// called from console.
	// and when parsin init.txt
	if(stricmp(input, "sv_mapcycle_begin") == 0)
	{		
		//return StartGame(NULL);
		s_phasor_mapcycle_entry game;
		game.gametype=L"ctf";
		game.map = "bloodgulch";

		LoadGame(game);
		return kProcessed;
	
	}
		return kGiveToHalo;
}

void __stdcall ProcessCommandAttempt(s_command_input* input, int playerNum)
{
	// called by rcon from game.

	_TRACE("\r\n - ProcessCommandAttempt") 
}

bool __stdcall OnMapLoad(s_mapcycle_entry* loading_map)
{
	_TRACE("\r\n - OnMapLoad") 
	return TRUE;
}

void __stdcall OnHaloPrint(char* msg)
{	
	//todo: set up heart beat.
}

bool __stdcall OnHaloBanCheck(char* hash, s_machine_info* machine)
{	
	_TRACE("\r\n - OnHaloBanCheck") 
	return TRUE;
}

void __stdcall OnHashValidation(s_hash_validation* info, const char* status)
{
	info->status = 1;
	_TRACE("\r\n - OnHashValidation %s ", status) 
}

void __stdcall  OnNewGame(const char* map)
{
	BuildTagCache();
	_TRACE("\r\n - OnNewGame %s", map) 
}

void __stdcall  OnGameEnd(DWORD mode)
{
	_TRACE("\r\n - OnGameEnd %8x", mode) 
}

void __stdcall OnPlayerWelcome(DWORD playerId)
{
	s_server_info* ss = GetServerStruct();
	
	_TRACE("\r\n - OnPlayerWelcome %S", ss->player_data[playerId].name) 
}

void __stdcall OnPlayerQuit(DWORD playerId)
{
	s_server_info* ss = GetServerStruct();
	_TRACE("\r\n - OnPlayerQuit %S", ss->player_data[playerId].name) 
}

DWORD __stdcall OnTeamSelection(DWORD cur_team, s_machine_info* machine)
{
	s_server_info* ss = GetServerStruct();
	
	_TRACE("\r\n - OnTeamSelection %d", cur_team) 
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
	ProcessChat(chat);



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
	//_TRACE("\r\n - VehicleRespawnCheck") 
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
	
	s_tag_entry* tag = LookupTag(dmg->tag_id);
	s_tag_entry* xs = LookupTag(*receiver);
	
	_TRACE("\r\n - OnDamageLookup %s -> [%s]",tag->tagName, xs->tagName)

	return true;
}


bool __stdcall OnDamageApplication(const s_damage_info* dmg, ident receiver, s_hit_info* hit, bool backtap)
{
	/*s_tag_entry* tag = LookupTag(dmg->tag_id);
	s_tag_entry* xs = LookupTag(receiver);
	
	_TRACE("\r\n - OnDamageApplication %s -> %s [%s]",tag->tagName, hit->desc, xs->tagName)*/

	s_player_structure* player = GetPlayerFromObjectId(receiver);
	if(player)
	{
		_TRACE("\r\n - OnDamageApplication %S", player->playerName)
	}


	return true; // setting to false => deathless.
}

