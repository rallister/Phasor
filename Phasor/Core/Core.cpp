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
	//return make_ident((player->playerJoinCount << 0x10) | player_entry->playerId);

	//s_presence_item* player_entry = &ss->player_data[player->playerNum];
	// if (player_entry->playerId == memory_id)
	// original
	//return make_ident((player->playerJoinCount << 0x10) | player->memory_id);
	return make_ident((player->playerJoinCount << 0x10) | player->object_id);
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
				/*for (int i = 0; i < 16; i++) 
				{
					s_player_structure* player = GetPlayer(i);

					if (player && player->team == from->team) 
						AddPacketToPlayerQueue(player->playerNum, PACKET_QUEUE_PARAMS);
				}*/

				s_server_info * server_info = GetServerStruct();

				for (int i = 0; i < 16; i++) 
				{   // retrieving as above does not distinguish between team 0 and team not present
					// i assume here it's a lil different coz there is some garbage with respect to mem init.
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

// you have to make a weapon first (?) 
// the weapon idents which are set up there they are on the map
// so essentially, either make it, or find them by going through tags
// weapon list which would be funny (if it works) but creating them is the way to go.
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
		DWORD mask = make_ident((DWORD)player);

		/*
			ident gg;
			gg.id = obj->id;
			gg.slot = 90; 0 - 90 fine, owner id is always 0 => point to use it to check for player ident correctness.
		*/


		__asm
		{
			pushad
			push 1
			mov eax, weaponid
			mov ecx, [playerObj]
			call dword ptr ds:[FUNC_PLAYERASSIGNWEAPON] 
			add esp, 4
			mov bSuccess, al
			cmp al, 1
			jnz ASSIGNMENT_FAILED
			mov ecx, gg // <-- this, works same as DWORD mask = make_ident((DWORD)player); still wrong coz. Also does not look like it cares about slot part.
			mov edi, weaponid
			push -1
			push -1
			push 7
			push 1
			call DWORD PTR ds:[FUNC_NOTIFY_WEAPONPICKUP] // notification happened, but weapon did not switch (picking up ammo for pistol, but it did not appear).
			add esp, 0x10
ASSIGNMENT_FAILED:
			popad
		}

		_TRACE("\r\n...weapon id=%d, slot=%d, player=[%d,%d]", weaponid.id, weaponid.slot, weapon->base.ownerPlayer.id, weapon->base.ownerPlayer.slot)	
	}

	
		

	return bSuccess;
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