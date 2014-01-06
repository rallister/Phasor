#include "Core.h"

void* GetObjectAddress(ident objectId)
{
	s_halo_object_table* object_table = *(s_halo_object_table**)ADDR_OBJECTBASE;
	if (objectId.slot >= object_table->header.max_size) return 0;

	s_halo_object_header* obj = &object_table->entries[objectId.slot];
	return obj->id == objectId.id ? obj->data : 0;
}

s_server_info* GetServerStruct()
{
	return (s_server_info*)ADDR_SERVERSTRUCT; 
}

s_halo_biped* GetBiped(ident objectId)
{
	return (s_halo_biped*)GetObjectAddress(objectId);
}

s_player_structure* GetPlayer(int index)
{
	if (index >= 0 && index < 16) {
		s_player_table* table = *(s_player_table**)ADDR_PLAYERBASE;
		if (table) return &table->players[index];
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
