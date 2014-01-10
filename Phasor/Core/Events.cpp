
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
	data->clientKey[8] = '\0';
	data->name[11] = L'\0';

	/*
		// now let scripts change the player's name
		std::string hash(data->hash, 32);
		std::string name = NarrowString(data->name);
		std::string new_name;	

		if (new_name.size()) 
		{
			std::wstring wname = WidenString(new_name).substr(0, 11);
			wcscpy_s(data->name, 12, wname.c_str());
		} 
		else 
		if (!allow_name) 
		{
			data->name[0] = L'\0';
		}
	*/

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
	// this was used to keep track of whether person is afk or not
	// adapted (also another call was onchat

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
		s_player_structure* player = GetPlayer(playerId);
		bool allow = true;		

		ident ii = make_ident(player->object_id.id);

		_TRACE("\r\n - OnTeamChange : [%d,%d] %S ii is [%d,%d]", player->object_id.id, player->object_id.slot, player->playerName, ii.id, ii.slot) 
		return allow;
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
	_TRACE("\r\n - OnObjectCreation [%d,%d]", m_objectId.id, m_objectId.slot) 
}

bool __stdcall OnObjectCreationAttempt(s_object_creation_disposition* creation_info)
{

	
		// original code, checkd scripts for allow, then an alternative tag was returned so and based on that
		// returned it.
		// i dotn think i need to modify anything.
		
		/*
			ident change_id;
			bool allow;

			//scripting::events::e_ident_or_bool r = scripting::events::OnObjectCreationAttempt(creation_info, change_id, allow);

			if (r == kBool) 
				return allow;

			else 
			{
			
				s_tag_entry* change_tag = LookupTag(change_id);
				s_tag_entry* default_tag = LookupTag(creation_info->map_id);

				if (change_tag && default_tag && change_tag->tagType == default_tag->tagType) {
					creation_info->map_id = change_id;
				}
				return true;
			}	
		*/

	//_TRACE("\r\n - OnObjectCreationAttempt") 
	return TRUE;
}

DWORD __stdcall OnWeaponAssignment(DWORD playerId, ident owningObjectId, s_object_info* curWeapon, DWORD order)
{
	// this method is also called to "arm" vehicles (at the start of the map)
	_TRACE("\r\n - OnWeaponAssignment [%s]: ", curWeapon->tagName) 

	ident weap_id = curWeapon->id;

	return weap_id;
	/*
		// original code.
		halo::s_player* player = game::getPlayer(playerId);
		ident weap_id = curWeapon->id, result_id;

		bool b = scripting::events::OnWeaponAssignment(player, owningObjectId, order, weap_id,
			result_id);

		// can return 0xFFFFFFFF to not assign a weapon
		if (b && (!result_id.valid() || LookupTag(result_id))) return result_id;
		else return weap_id;	
	*/
}

bool __stdcall OnObjectInteraction(DWORD playerId, ident m_ObjId)
{
	/*
		bool allow = true;
		halo::s_player* player = game::getPlayer(playerId);
		if (player) {
			objects::s_halo_object* obj = (objects::s_halo_object*)
				objects::GetObjectAddress(m_ObjId);

			allow = scripting::events::OnObjectInteraction(*player, m_ObjId, obj->map_id);		
		}
		return allow

	*/
	return TRUE;
}

void __stdcall OnChat(s_chat_data* chat)
{	

	ProcessChat(chat);

_TRACE("\r\n - OnChat %S", chat->msg) // 

	s_player_structure* s = GetPlayer(chat->player);

	 //ApplyCamo(s, 15); worked.
	 //return;

	s_halo_biped* dude = GetBiped(s->object_id);
	
	vect3d v3d = dude->cameraView;

	s_tag_entry* tag;

	size_t i = 0;	
	tag = LookupTag2("weapons\\pistol\\pistolweap\\");

	tag = LookupTag(make_ident(tag->id));

	DWORD parentId = 0;

	if (parentId == 0) 
		parentId = 0xFFFFFFFF;

	int respawnTime = -1;
	
	vect3d pos = dude->cameraView;

	pos.x = pos.x++;
	pos.y = pos.y++; //ReadNumber<float>(*args[i++]);
	//pos.z = ReadNumber<float>(*args[i++]);
		
	ident objid;
	CreateObject(tag->id, make_ident(s->object_id), -1, 0, &pos, &objid);
	AssignPlayerWeapon(s, objid);
	
}

bool __stdcall OnVehicleEntry(DWORD playerId)
{
	
	// cant immediately force him out coz he is not in vehicle yet.
	// for some reason.
	// s_player_structure* player = GetPlayer(playerId);
	// ExitVehicle(player);

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
	/*
	// no real need for sv_killed there, it just sets up the mode 
	// to be passed to the scripting engine.
	// could be used for spamming rules.

	s_player_structure* victim = GetPlayer(victimId);
	s_player_structure* killer = GetPlayer(killerId);

	if (!victim) 
		return true;

	// log the death based on type
	switch (mode)
	{
	case 1: // fall dmg or server kill
		{
			if (victim->sv_killed)	
				mode = 0;			
					
			_TRACE_DEATH_DAMAGE(victim->playerName, mode)

		} break;

	case 2: // killed by guardians
		{
			_TRACE_DEATH_GUARDIANS(victim->playerName)
		} break;

	case 3: // killed by a vehicle
		{
			_TRACE_DEATH_VEHICLE(victim->playerName)

		} break;

	case 4: // killed by another player
		{
			if (killer) 
			{				
				_TRACE_DEATH_KILL(killer->playerName,victim->playerName)
			}
		} break;

	case 5: // betrayed
		{				
			if (killer) {
				_TRACE_DEATH_BETRAY(killer->playerName, victim->playerName)
			}
		} break;

	case 6: // suicide
		{
			_TRACE_DEATH_SUICIDE(victim->playerName)
			
		} break;
	}
			
	return OnPlayerKill(*victim, killer, mode);
	*/
}

void __stdcall OnKillMultiplier(DWORD playerId, DWORD multiplier)
{
	_TRACE("\r\n - OnKillMultiplier") 
}


bool __stdcall OnWeaponReload(ident m_WeaponId)
{
	/* returning true is sufficient for standard way */
	s_halo_weapon* weapon = (s_halo_weapon*)GetObjectAddress(m_WeaponId);
	if (!weapon) 
		return true;


	s_player_structure* player = GetPlayer(weapon->base.ownerPlayer.slot);
	
	// todo: just testing if memory is being picked up, remove.
	//AssignPlayerWeapon(player, m_WeaponId);

	_TRACE("\r\n - OnWeaponReload: id=%d, slot=%d", m_WeaponId.id, m_WeaponId.slot) 
	s_player_structure* xx = GetPlayerFromObjectId(weapon->base.ownerPlayer);

	_TRACE("\r\nweapon id=%d, slot=%d, ownerid1=[%d,%d], ownerid2=[%d,%d] ammoclip=%d"
			, m_WeaponId.id, m_WeaponId.slot
			, weapon->base.ownerPlayer.id
			, weapon->base.ownerPlayer.slot
			, xx? xx->object_id.id : -1
			, xx? xx->object_id.slot: -1
			, weapon->ammo_clip)

	DWORD mask = make_ident(player->object_id.id);
	_TRACE("\r\nMASK = [%dL]", mask)
	return TRUE;

	/* original:
		objects::s_halo_weapon* weap = (objects::s_halo_weapon*)objects::GetObjectAddress(m_WeaponId);
		if (!weap) return true;
		halo::s_player* player = getPlayer(weap->base.ownerPlayer.slot);
		return scripting::events::OnWeaponReload(player, m_WeaponId);

		i noticed that when i used make_ident using player object_id object id and slot was actualy reversed
		so will need to look at it again.
	*/
}
 

int __stdcall VehicleRespawnCheck(ident m_objId, s_halo_vehicle* obj)
{
	// Called when an object is being checked to see if it should respawn
	// Return values:	0 - don't respawn
	//					1 - respawn
	//					2 - object destroyed
	// object destoryed, i see destroy obvject called manually there
	// so, if created custom object then need to return 2? 

	if (obj->idle_timer == 0xffffffff) 
		return 0; // still active

	DWORD respawn_ticks = GetRespawnTicks();
	DWORD server_ticks = GetServerTicks();

	if (respawn_ticks != 0) 
	{ // default processing if not managed object
		DWORD expiration = obj->idle_timer + respawn_ticks;
		if (expiration < server_ticks)					
			return 1;		
	}
	
	return 0;

	/* original code:

	if (obj->idle_timer == 0xffffffff) 
		return 0; // still active

	DWORD respawn_ticks = GetRespawnTicks();
	DWORD server_ticks = GetServerTicks();

	int result = 0;

	auto itr = managedList.find(m_objId);

	if (itr != managedList.end()) {
		s_phasor_managed_obj* phasor_obj = &itr->second;
		DWORD expiration = obj->idle_timer + phasor_obj->respawnTicks;
		if (expiration < server_ticks) {
			if (phasor_obj->bRecycle) {
				void* v1 = &phasor_obj->other, *rotation = &phasor_obj->rotation,
					*position = &phasor_obj->pos;

				__asm
				{
					pushad
					push m_objId
					call dword ptr ds:[FUNC_VEHICLERESPAWN2] // set flags to let object fall, reset velocities etc
					add esp, 4
					push v1
					push rotation
					push m_objId
					mov edi, position
					call dword ptr ds:[FUNC_VEHICLERESPAWN1] // move the object (proper way)
					add esp, 0x0c
					popad
				}

				// set last interacted to be now
				obj->idle_timer = server_ticks;
			} else { // destroy
				// destroy object will erase obj from managed list, so
				// itr and phasor_obj will be invalid.
#ifdef BUILD_DEBUG
				phasor_obj = 0;
				itr = managedList.end();
#endif
				DestroyObject(m_objId);
				result = 2;					
			}
		}
	} 
	
	else if (respawn_ticks != 0) 
	{ // default processing
		DWORD expiration = obj->idle_timer + respawn_ticks;
		if (expiration < server_ticks) 
			result = 1;
	}
	return result;

	*/
}


bool __stdcall EquipmentDestroyCheck(int checkTicks, ident m_objId, s_halo_object* obj)
{
	//_TRACE("\r\n - EquipmentDestroyCheck") 	

	// in game/objects.
	int objTicks = *(int*)((LPBYTE)obj + 0x204);
	return (checkTicks > objTicks);

	/*
		original code:

		// This is called when weapons/equipment are going to be destroyed.
		// todo: check ticks should be signed

		s_tag_entry* tag =  LookupTag(obj->map_id);

		bool bDestroy = false;

		int objTicks = *(int*)((LPBYTE)obj + 0x204);

		auto itr = managedList.find(m_objId);
		if (itr != managedList.end()) {
			s_phasor_managed_obj* phasor_obj = &itr->second;

			// respawn ticks are treated as expiration ticks
			if (phasor_obj->respawnTicks > 0)
			{
				if (phasor_obj->respawnTicks + (int)phasor_obj->creationTicks < checkTicks)
					bDestroy = true;
			}
			else if (phasor_obj->respawnTicks == -1) // use default value
			{
				if (checkTicks > objTicks)
					bDestroy = true;
			}
		} else {
			if (checkTicks > objTicks) 
				bDestroy = true;
		}

		return bDestroy;
	*/
	
}


void __stdcall OnObjectDestroy(ident m_objid)
{
	// this is called whenevre somone interract with nades/overshields ammo etc
	// so destroy in this context means someone picked up an item.

	/*
		auto itr = managedList.find(m_objid);
		if (itr != managedList.end()) 
			managedList.erase(itr);
	*/

	_TRACE("\r\n - OnObjectDestroy") 
	s_player_structure * p = GetPlayer(m_objid);
	if(p)
		_TRACE("player destroyed???");
	
}


bool __stdcall OnDamageLookup(s_damage_info* dmg, ident* receiver)
{	
	
	s_tag_entry* tag = LookupTag(dmg->tag_id);
	s_tag_entry* xs = LookupTag(*receiver);
	
	_TRACE("\r\n - OnDamageLookup %s -> [%s]",tag->tagName, xs->tagName)

	/*
		// Original code:
		s_tag_entry* tag = LookupTag(dmg->tag_id);
		damage_script_options opts;
		bool allow = scripting::events::OnDamageLookup(dmg, tag->metaData, *receiver, opts);

		if (allow) {
			dmg->causer = opts.causer;
			dmg->player_causer = opts.causer_player;
			dmg->flags = opts.flags;
			dmg->tag_id = opts.tag;
			dmg->modifier1 = opts.modifier;
			*receiver = opts.receiver;

		}
		return allow;

	*/
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

