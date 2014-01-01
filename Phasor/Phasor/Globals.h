#pragma once

#include "PhasorThread.h"
#include "Halo/HaloStreams.h"
#include "../Common/Timers.h"

// defined in main.cpp
extern PhasorThread g_Thread;
extern Timers g_Timers; 


extern void _DO_TRACE(const char* fmt...);
extern FILE * _TRACE_FILE;


#define _TRACE_CHAT(type, sender, message) _TRACE("[%S] %S: %S", (type), (sender), (message))
#define _TRACE_DEATH_GUARDIANS(victim) _TRACE("%S was killed by the guardians.", (victim))
#define _TRACE_DEATH_VEHICLE(victim) _TRACE("%S was killed by a vehicle.", (victim)) 
#define _TRACE_DEATH_DAMAGE(victim,mode) _TRACE("%S died (%i).", (victim), (mode))
#define _TRACE_DEATH_KILL(killer, victim) _TRACE("%S was killed by %S.", (killer), (victim))
#define _TRACE_DEATH_BETRAY(killer, victim) _TRACE("%S was betrayed by %S.", (killer), (victim))
#define _TRACE_DEATH_SUICIDE(player) _TRACE("%S killed himself", (player))

#define _TRACE_GAME_START(map) _TRACE("A new game has started on map %s", (map))
#define _TRACE_GAME_END_JUST_NOW() _TRACE("Game is ending score card showing")
#define _TRACE_GAME_END_CLEANUP() _TRACE("Game cleanup")

#define _TRACE_PLAYER_JOIN(player, hash, ip, port) _TRACE("%S (%S ip: %S:%i)", (player), (hash), (ip), (port))
#define _TRACE_PLAYER_JOIN_INVALID_INDEX(playerId) _TRACE("Player joined with invalid index?? (%d)", (playerId))
#define _TRACE_PLAYER_AFK_KICKED(playerName) _TRACE("%S has been kicked due to inactivity.")
#define _TRACE_PLAYER_LEFT(player, hash, ip, port) _TRACE("%S (%S ip: %S:%i)", (player), (hash), (ip), (port))
#define _TRACE_PLAYER_LEFT_INVALID_INDEX(playerId) _TRACE("Player left with invalid index?? (%d)", (playerId))

#define _TRACE_DEBUG_SCRIPT_OPEN(scriptName) _TRACE_DEBUG("Opening script %s", (scriptName))
#define _TRACE_DEBUG_SCRIPT_EVENT(msg) _TRACE_DEBUG("%S", msg)
#define _TRACE_PLAYER_REJECTED_BY_BAN_SCRIPT(hash, ip) _TRACE_DEBUG("Scripts rejected player. Hash %S , IP %s", (hash), (ip))

//full player
#define _TRACE_PLAYER_DUMP(player)

#define _TRACE_TEAM_CHANGE_BLOCKED(player) _TRACE("Blocked %S from changing team.", (player))

// dunno what it is
#define _TRACE_ON_HALO_PRINT(msg) _TRACE((msg))


// full player: hash is not very useful at all, ript it out.
#define _TRACE_PROCESS_COMMAND_ERR_NOTADMIN(exec_player) _TRACE("RCON: not admin: %S (%S) tried to execute %s",(exec_player->mem->playerName), (exec_player->hash), (commandName))
#define _TRACE_PROCESS_COMMAND_ERR_NOTALLOWED(exec_player) _TRACE("RCON: not allowed %S (%S) tried to execute %s",(exec_player->mem->playerName), (exec_player->hash), (commandName) )
#define _TRACE_PROCESS_COMMAND_OK(exec_player) _TRACE("RCON: OK : %S %s",(exec_player->mem->playerName), (commandName) )

#define _TRACE_ALIAS_ERR_CANT_CREATE_DB(errorMsg) _TRACE_DEBUG( "Cannot create alias database, error '%s'", (errorMsg))

#define _TRACE_ON_HASH_VALIDATION(info) _TRACE_DEBUG("HASH VALIDATION: %S, %d, %d, %d ", info->hash, info->machineId, info->requestId, info->status)

#define _TRACE_ON_HASH_REJECTED(machineId, status) _TRACE_DEBUG("Machine %d is being rejected because : %s", (machineId), (status))

#define _TRACE_DEBUG_APPLYING_VEHICLE_DAMAGE_NO_VEHICLE_HIT(mapName) _TRACE_DEBUG("Map %S  doesn't have globals\\vehicle_hit_environment", (mapName))

// if streams fail search for this.
#define _TRACE_STREAM_COMMAND_RESULT(str) _TRACE("%S", (str))


#define _TRACE_MAP_IGNORED_DUE_TO_PREVIOUS_ERROR() _TRACE("Map ignored due to previous error")

#define _TRACE_DEBUG_CANT_READ_GAMETYPE(gametype) _TRACE_DEBUG("Cannot read gametype data for %S ", gametype);

#define _TRACE(...) _DO_TRACE (__VA_ARGS__);
#define _TRACE_DEBUG _TRACE
#define _TRACE_CONSOLE _TRACE

