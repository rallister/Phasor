#include "Core.h"




ident make_ident(unsigned long id) 
{
	ident out;
	out.slot = (unsigned short)(id & 0xffff);
	out.id = (unsigned short)((id & 0xffff0000) >> 16);
	return out;
}

void* GetObjectAddress(ident objectId)
{
	// get player returns reference to teleporter loop, weird aint it.

	s_halo_object_table* object_table = *(s_halo_object_table**)ADDR_OBJECTBASE;
	if (objectId.slot >= object_table->header.max_size) 
		return 0;

	s_halo_object_header* obj = &object_table->entries[objectId.slot];

	s_tag_entry* ss = LookupTag(objectId);
	_TRACE("\r\ntag name = %s", ss->tagName);


	return obj->id == objectId.id ? obj->data : 0;
}

s_server_info* GetServerStruct()
{
	return (s_server_info*)ADDR_SERVERSTRUCT; 
}

s_halo_biped* GetBiped(ident objectId)
{
	// get player returns reference to teleporter loop, 
	// need to double check again that everything is used correctly.
	return (s_halo_biped*)GetObjectAddress(objectId);
}

ident GetPlayerIdent(s_player_structure* player)
{
	s_server_info* ss = GetServerStruct();

	s_presence_item* player_entry = &ss->player_data[player->playerNum];
	return make_ident((player->playerJoinCount << 0x10) | player_entry->playerId);
}

s_player_structure* GetPlayer(unsigned long index)
{
	if (index >= 0 && index < 16) {
		s_player_table* table = *(s_player_table**)ADDR_PLAYERBASE;
		if (table) 
			return &table->players[index];
	}
	return 0;
}	

s_player_structure* GetPlayerFromObjectId(ident id)
{	

	for (int i = 0; i < 16; i++) 
	{
		s_player_structure* sp = GetPlayer(i);
		if(sp->object_id == id)
		{
			return sp;
		}
	}
	return NULL;
}

s_player_structure* GetPlayerExecutingCommand()
{
	DWORD execPlayerNumber = *(DWORD*)UlongToPtr(ADDR_RCONPLAYER);
	return GetPlayer(execPlayerNumber);
}

void SetExecutingPlayer(s_player_structure* player)
{
	if (!player) 
		*(DWORD*)UlongToPtr(ADDR_RCONPLAYER) = -1;
	else 
		*(DWORD*)UlongToPtr(ADDR_RCONPLAYER) = player->playerNum;
}

void ExecuteServerCommand(const std::string& command, s_player_structure* execute_as)
	{
		s_player_structure* old_exec_player = GetPlayerExecutingCommand();
		SetExecutingPlayer(execute_as);

		const char* cmd = command.c_str();
		__asm
		{
			pushad
			PUSH 0x2000
			mov edi, cmd
			call dword ptr ds: [FUNC_EXECUTESVCMD]
			add esp, 4		
			popad
		}

		SetExecutingPlayer(old_exec_player);
	}

bool DestroyObject(ident objid)
{
	if (!GetObjectAddress(objid)) 
		return false;

	__asm
	{
		pushad
		mov eax, objid
		call dword ptr ds:[FUNC_DESTROYOBJECT]
		popad
	}
	return true;
}

//void ChatVehicle(wstring msg, s_player_structure* from)
//{
//}
//
//void ChatTeam(wstring msg, s_player_structure* from)
//{
//}
//
//void ChatServer(wstring msg)
//{
//}

void ProcessChat(s_chat_data* chat)
{
	if(chat->player < 0 || chat->player > 16)
		return;

	s_player_structure* from = GetPlayer(chat->player);

	DispatchChat(chat->type, chat->msg, from, NULL);
}

void DispatchChat(e_chat_types type, const wchar_t* msg, s_player_structure* from, s_player_structure* to)
{	
	s_chat_data chat_data;
	chat_data.type = type;
	chat_data.player = -1;
	chat_data.msg = msg;

	if (type < kChatAll || type > kChatVehicle) 
		return;

	if (wcslen(msg) > 64) 
		return;

	switch (type)
	{
	case kChatServer:
		{
			chat_data.player = -1;
		} break;

	case kChatAll:
	case kChatTeam:
	case kChatVehicle:
		{
			if(from != NULL)
				chat_data.player = from->playerNum;
		} break;
	default:
		{
			//assert(0);
		} break;
	}
		
	// Build the packet
	BYTE buffer[8192];		
	DWORD d_ptr = (DWORD)&chat_data; // Gotta pass a pointer to the struct
	DWORD retval = BuildPacket(buffer, 0, 0x0F, 0, (LPBYTE)&d_ptr, 0, 1, 0);
		
	// Add the packet to the appropriate queue
	#define PACKET_QUEUE_PARAMS buffer, retval, 1, 1, 0, 1, 3

	if (to) // send to specific player
		AddPacketToPlayerQueue(to->playerNum, PACKET_QUEUE_PARAMS);
	else { // apply normal processing if a dest player isn't specified
		switch (type)
		{
		case kChatServer:
		case kChatAll:
			{
				AddPacketToGlobalQueue(PACKET_QUEUE_PARAMS);
			} 
			break;

		case kChatTeam:
			{
				/*
					// retrieving as above does not distinguish between team 0 and team not present
					// i assume here it's a lil different coz there is some garbage with respect to mem init.
					for (int i = 0; i < 16; i++) 
					{
						s_player_structure* player = GetPlayer(i);

						if (player && player->team == from->team) 
							AddPacketToPlayerQueue(player->playerNum, PACKET_QUEUE_PARAMS);
					}
				*/

				s_server_info * server_info = GetServerStruct();

				for (int i = 0; i < 16; i++) 
				{  
					s_presence_item* player_entry = &server_info->player_data[i];
					if (player_entry->team == from->team) 
						AddPacketToPlayerQueue(player_entry->playerId, PACKET_QUEUE_PARAMS);											
				}
			} 
			break;
		case kChatVehicle:
			{
				// Check if the sender is in a vehicle
				s_halo_biped* from_obj =  GetBiped(from->object_id);

				if (from_obj && from_obj->base.vehicleId.valid()) 
				{
					// send to players in this vehicle
					for (int i = 0; i < 16; i++) 
					{
						s_player_structure* player = GetPlayer(i);
						if (!player) 
							continue;

						s_halo_biped* obj = GetBiped(player->object_id);
						
						if (obj && obj->base.vehicleId == from_obj->base.vehicleId)
							AddPacketToPlayerQueue(player->playerNum, PACKET_QUEUE_PARAMS);		
					}
				}

			} break;
		}
	}
}



/*! \todo parse these structures */
DWORD GetRespawnTicks()
{
	return *(DWORD*)(ADDR_GAMETYPE + OFFSET_RESPAWNTICKS);
}

DWORD GetServerTicks()
{
	DWORD server_base = *(DWORD*)ADDR_SERVERINFO;
	DWORD ticks = *(DWORD*)(server_base + 0x0C);
	return ticks;
}


bool CreateObject(ident mapid, ident parentId, int respawnTime, bool bRecycle, vect3d* location, ident* out_objid)
{
	// creates a managed object so if using that need to implement
	// managing of those obejcts. actually not really, it will just disappear 
	// if not used (i wonder with regard to max number,


	s_tag_entry* tag = LookupTag(mapid);
	if (!tag) 
		return false;

	if (!parentId) 
		parentId = make_ident(-1);

	// Build the request data for <halo>.CreateObject
	DWORD mapId = tag->id;
	BYTE query[0x100] = {0}; // i think max ever used is 0x90
	__asm
	{
		pushad
		push parentId
		push mapId
		lea eax, dword ptr ds:[query]
		call DWORD PTR ds:[FUNC_CREATEOBJECTQUERY]
		add esp, 8
		popad
	}

	s_object_creation_disposition* creation_disposition = (s_object_creation_disposition*)query;
	//creation_disposition->map_id, creation_disposition->parent.id,creation_disposition->player_ident.id
	// Set the spawn coordinates (if supplied)
	if (location)
		creation_disposition->pos = *location;
	else 
		location = &creation_disposition->pos;

	DWORD objid;
	// Create the object
	__asm
	{
		pushad
		push 0
		lea eax, dword ptr ds:[query]
		push eax
		call DWORD PTR ds:[FUNC_CREATEOBJECT]
		add esp, 8
		mov objid, eax
		popad
	}
	*out_objid = make_ident(objid);
	if (!out_objid->valid())	
		return false;

	// resolve the respawn timer
	if (respawnTime == -1) // use gametype's value
	{
		// weapons/equipment don't respawn, they're destroyed and
		// so the default value is handled in the processing func
		if (tag->tagType == TAG_EQIP || tag->tagType == TAG_WEAP)
			respawnTime = -1;
		else
			respawnTime = GetRespawnTicks();
	}
	else
		respawnTime *= 30;

	/*
	managedList.insert(std::pair<ident, s_phasor_managed_obj>
		(out_objid, 
			s_phasor_managed_obj(out_objid, bRecycle, *location, respawnTime,
		creation_disposition)));
	*/
	return true;
}


// best method to check that ident is working.
void ApplyCamo(s_player_structure* player, float duration)
{
	
	ident playerObj = player->object_id;
	// DWORD mask = make_ident(player->object_id.id); // this makes no sense but appeared closest to result
	DWORD mask = GetPlayerIdent(player);

	DWORD count = 0x8000; // count >= 0x8000 == infinite

	if (duration != 0)
		count = (DWORD)(duration * 30); // 30 ticks per second

	// Make the player invisible
	__asm
	{
		mov eax, playerObj
		mov ebx, mask
		push count
		push 0
		call dword ptr ds:[FUNC_DOINVIS]
		add esp, 8
	}
}

// player has maximum of 4 weapons
// so need to get rid of one if assigning too many times.
bool AssignPlayerWeapon(s_player_structure* player, ident weaponid)
{

	bool bSuccess = false;

	s_halo_biped* biped = GetBiped(player->object_id);

	if (!biped) 
		return false;

	// can't be in vehicle
	if (!biped->base.vehicleId.valid())	
	{
		
		// make sure they passed a weapon
		s_halo_weapon* weapon = (s_halo_weapon*)GetObjectAddress(weaponid);

		if (!weapon) 
			return false;
		
		s_player_structure* xx = GetPlayerFromObjectId(weapon->base.ownerPlayer);
		_TRACE("\r\nweapon id=%d, slot=%d, player=[%d,%d] ammoclip=%d", weaponid.id, weaponid.slot, player->object_id.slot, player->object_id.id, weapon->ammo_clip)		
		
		ident playerObj = player->object_id; 
		
		s_halo_object_table* object_table = *(s_halo_object_table**)ADDR_OBJECTBASE;	
		s_halo_object_header* obj = &object_table->entries[playerObj.slot];
		DWORD mask = GetPlayerIdent(player); // this makes no sense 

	
		__asm
		{
			pushad
			push 1
			mov eax, weaponid
			mov ecx, playerObj
			call dword ptr ds:[FUNC_PLAYERASSIGNWEAPON] 
			add esp, 4
			mov bSuccess, al
			cmp al, 1
			jnz ASSIGNMENT_FAILED
			mov ecx, mask
			mov edi, weaponid
			push -1
			push -1
			push 7
			push 1
			call DWORD PTR ds:[FUNC_NOTIFY_WEAPONPICKUP]
			add esp, 0x10
ASSIGNMENT_FAILED:
			popad
		}

		_TRACE("\r\n...weapon id=%d, slot=%d, player=[%d,%d]", weaponid.id, weaponid.slot, weapon->base.ownerPlayer.id, weapon->base.ownerPlayer.slot)	
	}

	
		

	return bSuccess;
}

void Kill(s_player_structure* player)
{
	//sv_killed = true; // used later for detecting what killed the player
	
	if (player->object_id.valid()) {
		DWORD mask = GetPlayerIdent(player);
		DWORD playerObj = player->object_id; // it's an ident above inother functions.
		__asm
		{
			pushad

			PUSH 0
			PUSH -1
			PUSH -1
			PUSH -1
			MOV EAX,playerObj
			call DWORD PTR ds:[FUNC_ONPLAYERDEATH]
			add esp, 0x10
			mov eax, playerObj
			call DWORD PTR ds:[FUNC_ACTIONDEATH_1]
			mov eax, mask
			call DWORD PTR ds:[FUNC_ACTIONDEATH_2]
			push mask
			call DWORD PTR ds:[FUNC_ACTIONDEATH_3]
			add esp, 4

			popad
		}
	}

	//sv_killed = false;
}

void ChangeTeam(s_player_structure* player, BYTE new_team, bool forcekill)
{	
	s_server_info* server_info = GetServerStruct();

	for (int i = 0; i < 16; i++) {
		s_presence_item* player_entry = &server_info->player_data[i];

		ident memory_id = GetPlayerIdent(player);

		if (player_entry->playerId == memory_id)
		{
			BYTE old_team = player->team;

			// update teams in memory
			player->team = new_team;
			player->team_Again = new_team;
			player_entry->team = new_team;

			if (forcekill) 
				Kill(player);

			NotifyServerOfTeamChange(player);
				
			// scripting::events::OnTeamChange(*this, false, old_team, new_team);
			break;
		}
	}
}

bool ConsoleMessagePlayer(const s_player_structure* player, const std::wstring& str)
{
	if (str.size() >= 0x50) 
		return false;

	std::string str_narrow = NarrowString(str);

	s_console_msg d(str_narrow.c_str());

	BYTE buffer[8192]; 

#ifdef PHASOR_PC
	DWORD size = BuildPacket(buffer, 0, 0x37, 0, (LPBYTE)&d, 0,1,0);
#elif  PHASOR_CE 
	DWORD size = BuildPacket(buffer, 0, 0x38, 0, (LPBYTE)&d, 0,1,0);
#endif
	AddPacketToPlayerQueue(player->playerNum, buffer, size, 1,1,0,1,3);
		
	return true;
}

void NotifyServerOfTeamChange(s_player_structure* player)
{
	// build the packet that notifies the server of the team change
	ident memory_id = GetPlayerIdent(player);
	BYTE d[4] = {(BYTE)memory_id, (BYTE)player->team, 0x18, 0};

	// Gotta pass a pointer to the data
	DWORD d_ptr = (DWORD)&d;
	BYTE buffer[8192];
	DWORD retval = BuildPacket(buffer, 0, 0x1A, 0, (LPBYTE)&d_ptr, 0, 1, 0);
	AddPacketToGlobalQueue(buffer, retval, 1, 1, 0, 1, 3);
}

// Forces a player into a vehicle
// Seat numbers: 0 (driver) 1 (passenger) 2 (gunner)
bool EnterVehicle(s_player_structure* player, ident m_vehicleId, DWORD seat)
{
	s_halo_vehicle* vehicle = (s_halo_vehicle*)GetObjectAddress(m_vehicleId);
	if (!vehicle) return false; 

	// set interaction info
	player->m_interactionObject = m_vehicleId;
	player->interactionType = 8; // vehicle interaction
	player->interactionSpecifier = (WORD)seat;

	// this does not exist on normal player
	// player->force_entered = true; // so scripts cant stop them entering

	DWORD mask = GetPlayerIdent(player);						//todo: again double check this.

	// enter the vehicle (if seat is free)
	__asm
	{
		pushad
		push mask
		call dword ptr ds:[FUNC_ENTERVEHICLE]
		add esp, 4
		popad
	}

	// player.force_entered = false;

	return true;
}

//ident GetVehicleId(s_player_structure* player)
//{
//	s_halo_biped* from_obj =  GetBiped(player->object_id);
//
//
//	if (from_obj && from_obj->base.vehicleId.valid()) 
//		return from_obj->base.vehicleId;
//
//	return 0;
//}

// Forces a player to exit a vehicle
// does not work if called from onvehicleentry
// biped does not indicate he is in valid vehicle.
bool ExitVehicle(s_player_structure* player)
{
	// works fine even if called when player not vehicle
	// so this check below appear redundant.
	// s_halo_biped* from_obj =  GetBiped(player->object_id);
	// if (!from_obj || !from_obj->base.vehicleId.valid()) 
	//	return false;
	
	DWORD playerObj = player->object_id;
	__asm
	{
		pushad
		mov eax, playerObj
		call dword ptr ds:[FUNC_EJECTVEHICLE]
		popad
	}
}

void MoveObject(s_halo_object object, const vect3d& pos)
{
	// this should beinteresting.
	object.location = pos;
	object.stationary = false;
}

bool FindIntersection(const view_vector& view, ident& ignore_obj, vect3d& hit_pos, ident& hit_obj)
{
	struct s_intersection_output
	{
		BYTE mode; // only seen 2 (hit obj) 3 (didn't)
		UNKNOWN(0x0f);
		BYTE hit; // 0 = no hit, else hit.. i think
		UNKNOWN(7);
		vect3d hit_pos;
		UNKNOWN(0x14);
		ident hit_obj;
		UNKNOWN(0x28);
	};

	static_assert(sizeof(s_intersection_output) == 0x64, "bad s_intersection_test");

	const vect3d* dir = &view.dir, *pos = &view.pos;
	s_intersection_output result;
	bool hit;
	DWORD ignore = ignore_obj;

	__asm {
		pushad

		lea eax, result
		push eax
		push ignore
		push dir
		push pos
		push 0x1000E9
		call dword ptr ds:[FUNC_INTERSECT]
		mov hit, al
		add esp, 0x14

		popad
	}

	hit_pos = result.hit_pos;

	if (result.mode == 3) 
		hit_obj = result.hit_obj;

	return hit;
}


// --------------------------------------------------------------------
// dunno about these 2.
void s_halo_weapon::SetAmmo(WORD pack, WORD clip)
{
	ammo_pack = pack;
	ammo_clip = clip;
}

void s_halo_weapon::SyncAmmo(ident weaponId)
{
	DWORD id = weaponId;
	__asm
	{
		pushad
			mov ebx, 0
			mov ecx, id
			call dword ptr ds:[FUNC_UPDATEAMMO]
		popad
	}
}

//--------------------------------------------------------------------

// not working
void ApplyDamage(ident receiver, ident causer, const s_tag_entry& dmg_tag, float mult, int flags)
{
	//if (!receiver.valid() || objects::GetObjectAddress(receiver) == NULL || (causer.valid() && objects::GetObjectAddress(causer) == NULL))
	//	return false;

	s_damage_info dmg;
	/* 
	compiler says no such thing _unused etc.
	memset(dmg._unused_4, 0, sizeof(dmg._unused_4));
	memset(dmg._unused_5, 0, sizeof(dmg._unused_5));
	*(WORD*)dmg._unused_4 = 0xFFFF;
	*(WORD*)(dmg._unused_4 + 8) = 0xFFFF;
	*(WORD*)(dmg._unused_5 + 4) = 0xFFFF;
	*/
	dmg.tag_id = dmg_tag.id;
	dmg.causer = causer;
	dmg.flags = flags;
	dmg.modifier = 1;
	dmg.modifier1 = mult;

	s_player_structure* player_causer = GetPlayerFromObjectId(causer);
	dmg.player_causer = player_causer ? player_causer->object_id : ident(); //hmm.

	//0018DBF8  5B 07 D1 E8 84 00 00 00 FF FF FF FF FF FF FF FF  [Ñè„...ÿÿÿÿÿÿÿÿ
	//0018DC08  FF FF 00 00 00 00 00 00 FF FF 00 00 00 00 00 00  ÿÿ......ÿÿ......
	//0018DC18  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
	//0018DC28  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
	//0018DC38  00 00 80 3F 00 00 80 3F 00 00 00 00 FF FF 00 00  ..€?..€?....ÿÿ..

	__asm {
		pushad

		push 0
		push -1
		push -1
		push -1
		push receiver
		lea edi, dmg
		push edi
		call CC_DAMAGELOOKUP
		add esp, 0x18

		popad
	}
}

	
// Temporarily modifies globals\vehicle_hit_environment and uses it to apply damage.
bool ApplyDamage(ident receiver, ident causer, float dmg, int flags)
{
	s_tag_entry* dmg_tag = LookupTag(s_tag_type("jpt!"), "globals\\vehicle_hit_environment");
	if (!dmg_tag) {
			
		s_server_info* info = GetServerStruct();
		
		return false;
	}

	// Temporarily modify the tag
	static const char* phasor_dmg = "phasor_damage";
	DamageModifier dmgMod(dmg_tag);
	dmg_tag->tagName = phasor_dmg;

	s_damage_tag* data = (s_damage_tag*)dmg_tag->metaData;
	data->amount = s_damage_amount(1,1,1);//dmg,dmg,dmg);

	ApplyDamage(receiver, causer, *dmg_tag, dmg, flags);
	return true;
}


//============================= version related==================================
/* this will not be here, just for reference.

	static const std::map<std::string, versionInfo> versionList = []() -> std::map<std::string, versionInfo>
	{
		std::map<std::string, halo::server::misc::versionInfo> versions;
#ifdef PHASOR_PC
		versions["100"] = ("01.00.00.0564", 0x00006);
		versions["101"] = ("01.00.01.0580", 0x8d9a0);
		versions["102"] = ("01.00.02.0581", 0x8dd88);
		versions["103"] = ("01.00.03.0605", 0x93b48);
		versions["104"] = ("01.00.04.0607", 0x94318);
		versions["105"] = (01.00.05.0610", 0x956a0);
		versions["106"] = (01.00.06.0612", 0x956a0);
		versions["107"] = ("01.00.07.0613", 0x956a0);
		versions["108"] = ("01.00.08.0616", 0x96640);
		versions["109"] = ("01.00.09.0620", 0x96640);
#else
		versions["100"] = ("01.00.00.0609", 0x94ecf);
		versions["107"] = ("01.00.07.0613", 0x5bc42f);
		versions["108"] = ("01.00.08.0616", 0x5bcfe7);
		versions["109"] = ("01.00.09.0620", 0x96a27);
	
#endif
		return versions;
	}();
*/

e_command_result setVersionCheck(bool enabled)
{
		
	if (enabled) 
	{
		BYTE jmp[] = {0xEB};
		WriteBytes(PATCH_ANYVERSIONCHECK1, &jmp, sizeof(jmp));
		WriteBytes(PATCH_ANYVERSIONCHECK2, &jmp, sizeof(jmp));		
	} 
	else
	{
		BYTE jge[] = {0x7D}, jle[] = {0x7e};
		WriteBytes(PATCH_ANYVERSIONCHECK1, &jge, sizeof(jge));
		WriteBytes(PATCH_ANYVERSIONCHECK2, &jle, sizeof(jle));
	}
		
	return kProcessed;
}

bool setVersionServer(DWORD versionid)
{
	WriteBytes(PATCH_CURRENTVERSION, &versionid, 4);
	return true;
}

bool setVersionBroadcast(char* version)
{
	char* halo_version = (char*)ADDR_BROADCASTVERSION;	
	WriteString(ADDR_BROADCASTVERSION, version);
	return true;
}

bool GetMachineIP(s_machine_info& machine, std::string* ip, WORD* port)
{
	s_connection_info* con = machine.get_con_info();
	if (!con) 
		return false;
	if (ip) 
	{
		BYTE* ip_data = con->ip;
		*ip = m_sprintf("%d.%d.%d.%d", ip_data[0], ip_data[1], ip_data[2], ip_data[3]);
	}

	if (port) 
		*port = con->port;

	return true;
}

bool GetMachineHash(const s_machine_info& machine, std::string& hash)
{
	s_hash_list* hash_list = (s_hash_list*)ADDR_HASHLIST;

	hash_list = hash_list->next;

	while (hash_list && hash_list->data) 
	{	
		if (hash_list->data->id == machine.machineNum) 
		{
			hash = hash_list->data->hash;
			return true;
		}

		hash_list = hash_list->next;
	}
	return false;
}


//====================================== accessign console ============================================
#ifdef PHASOR_PC
	const std::wstring MSG_PREFIX = L"** SERVER ** ";
#elif PHASOR_CE
	const std::wstring MSG_PREFIX;
#endif

// this will most certainly not be necessary keepin it for reference.
bool WriteToConsole(const std::wstring& str)// str usually has endl appended
{
	if (str.size() == 0) return true;
		
	bool ready = *(bool*)UlongToPtr(ADDR_CONSOLEREADY);

	if (!ready) 
	{		
		return true;
	}

	// Prepare for writing the string
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD written = 0;

	CONSOLE_SCREEN_BUFFER_INFO info;
	SHORT oldX = 0; // used to set cursor back to old position

	// Get current console position info
	GetConsoleScreenBufferInfo(hConsole, &info);
	oldX = info.dwCursorPosition.X;

	// Set cursor to start of the last row (where we want to start writing)
	info.dwCursorPosition.X = 0;
	info.dwCursorPosition.Y = 299;
	SetConsoleCursorPosition(hConsole, info.dwCursorPosition);

	FillConsoleOutputCharacterA(hConsole, ' ', 95, info.dwCursorPosition, &written);
	FillConsoleOutputAttribute(hConsole, 7, 95, info.dwCursorPosition, &written);

	// Write the text
	WriteConsoleW(hConsole, str.c_str(), str.size(), &written, NULL);
	//WriteConsoleW(hConsole, L"\n", 1, &written, NULL);

	// Get the current text in the console
	LPBYTE ptr = (LPBYTE)ADDR_CONSOLEINFO;

	if (*ptr != 0) {
		// Build current command input
		std::string formatted = "halo( ";

		formatted += (char*)UlongToPtr(*(DWORD*)ptr + OFFSET_CONSOLETEXT); // current text

		// Rewrite the data to console
		GetConsoleScreenBufferInfo(hConsole, &info);
		FillConsoleOutputCharacterA(hConsole, ' ', 95, info.dwCursorPosition, &written);
		WriteConsoleOutputCharacterA(hConsole, formatted.c_str(), formatted.size(), info.dwCursorPosition, &written);

		// Set the cursor to its old position
		GetConsoleScreenBufferInfo(hConsole, &info);
		info.dwCursorPosition.X = oldX;
		SetConsoleCursorPosition(hConsole, info.dwCursorPosition);
	}
	return true;
}


//========================================================================================

/*	
	// old code from Server to
	// save the command for memory (arrow keys)
	if (exec_player == NULL && result == kProcessed) 
	{
		
		s_command_cache* cache = (s_command_cache*)ADDR_CMDCACHE;
		cache->count = (cache->count + 1) % 8;
		strcpy_s(cache->commands[cache->count], sizeof(cache->commands[cache->count]), cmd);
		cache->cur = 0xFFFF;
	}
*/