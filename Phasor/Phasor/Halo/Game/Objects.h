#pragma once

#include "../../../Common/Types.h"
#include "../../../Common/vect3d.h"
#include "../Halo.h"

namespace halo { 
	struct s_player;
	namespace objects
{
	
	
	void* GetObjectAddress(ident objectId);
	bool DestroyObject(ident objid);

	void ClearManagedObjects();

	bool CreateObject(ident mapid, ident parentId, int respawnTime, bool bRecycle,
		const vect3d* location, ident& out_objid);

	bool AssignPlayerWeapon(s_player& player, ident weaponid);

	// Forces a player into a vehicle
	// Seat numbers: 0 (driver) 1 (passenger) 2 (gunner)
	bool EnterVehicle(s_player& player, ident m_vehicleId, DWORD seat);

	// Forces a player to exit a vehicle
	bool ExitVehicle(s_player& player);

	void MoveObject(s_halo_object& object, const vect3d& pos);
	
	// Finds an intersection between view and the environment. 
	// Returns true if object intersected, false otherwise.
	bool FindIntersection(const view_vector& view, const ident& ignore_obj,
		vect3d& hit_pos, ident& hit_obj);

	// --------------------------------------------------------------------
	// Events

}}