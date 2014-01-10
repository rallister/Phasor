#pragma once



#include "Addresses.h"
#include "Types.h"
#include "Packet.h"
#include "tags.h"

#include "Globals.h"
#include <stdio.h>

using namespace std;

s_halo_biped* GetBiped(ident objectId);
void* GetObjectAddress(ident objectId);
s_server_info* GetServerStruct();
s_player_structure* GetPlayer(unsigned long index);
s_player_structure* GetPlayerFromObjectId(ident id);
ident GetPlayerIdent(s_player_structure* player);
ident make_ident(unsigned long id); 
/*! \todo parse these structures */
DWORD GetRespawnTicks();
DWORD GetServerTicks();
bool DestroyObject(ident objid);

void ProcessChat(s_chat_data* chat);
void DispatchChat(e_chat_types type,  const wchar_t* msg, s_player_structure* from, s_player_structure* to);

bool CreateObject(ident mapid, ident parentId, int respawnTime, bool bRecycle,	vect3d* location, ident* out_objid);
bool AssignPlayerWeapon(s_player_structure* player, ident weaponid);
bool EnterVehicle(s_player_structure* player, ident m_vehicleId, DWORD seat);
bool ExitVehicle(s_player_structure* player);
void MoveObject(s_halo_object& object, const vect3d& pos);
bool FindIntersection(const view_vector& view, const ident& ignore_obj,vect3d& hit_pos, ident& hit_obj);

// dodgies.
void SetAmmo(s_halo_weapon* weapong, WORD pack, WORD clip);
void SyncAmmo(ident weaponId);

void NotifyServerOfTeamChange(s_player_structure* player);