#pragma once

#include "Addresses.h"
#include "Types.h"
#include "Packet.h"

using namespace std;

void* GetObjectAddress(ident objectId);
s_server_info* GetServerStruct();
s_player_structure* GetPlayer(int index);
s_player_structure* GetPlayerFromObjectId(ident id);

void ProcessChat(s_chat_data* chat);
void DispatchChat(e_chat_types type,  const wchar_t* msg, s_player_structure* from, s_player_structure* to);