#pragma once

#include <windows.h>
#include "MemoryOperations.h"
#include "Addresses.h"
#include "vect3d.h"

#define NELEMS(x)  (sizeof(x) / sizeof(x[0]))
#define STR_CAT(a,b)            a##b
#define STR_CAT_DELAYED(a,b)   STR_CAT(a,b)
#define UNKNOWN(size) char STR_CAT_DELAYED(_unused_,__COUNTER__)[size]
#define UNKNOWN_BITFIELD(size) char STR_CAT_DELAYED(_unusedbf_, __COUNTER__) : size


enum e_command_result
{
	kGiveToHalo = 0,
	kProcessed
};
//------------------------------ combined definitions --------------------------------------



#pragma pack(push, 1)

	struct ident 
	{
		unsigned short slot;
		unsigned short id;		
		
		explicit ident() {
			slot = 0xFFFF;
			id = 0xFFFF;
		}

		bool operator<(const ident& other)
		{
			return (unsigned long)*this < (unsigned long)other;
		}

		bool operator>(const ident& other)
		{
			return (unsigned long)*this > (unsigned long)other;
		}

		operator unsigned long() const
		{
			unsigned long result = id << 16 | (slot & 0xffff);
			return result;
		}

		bool valid() const { return id != 0xFFFF || slot != 0xFFFF; }

	};
	static_assert(sizeof(ident) == 4, "bad");

	
	// Header for tables used throughout Halo (objects, players, loopobjects)
	struct s_table_header
	{
		char name[0x20];
		unsigned short max_size;
		unsigned short elem_size;
		UNKNOWN(4); // only seen as 1
		UNKNOWN(4); // d@t@
		UNKNOWN(2);
		unsigned short cur_items; // i think
		UNKNOWN(4);
		void* data;
	};
	static_assert(sizeof(s_table_header) == 0x38, "s_table_header size incorrect");

#pragma pack(pop)

	#pragma pack(push, 1)
	struct s_hash_data
	{
		DWORD id;
		char hash[0x20];
		BYTE unk[0x2c];
	};
	static_assert(sizeof(s_hash_data) == 0x50, "incorrect s_hash_data");

	struct s_hash_list
	{
		s_hash_data* data; // 0 for head of list
		s_hash_list* next;
		s_hash_list* prev; 
	};
	static_assert(sizeof(s_hash_list) == 0x0C, "incorrect s_hash_list");

	struct s_command_cache
	{
		char commands[8][0xFF];
		WORD unk;
		WORD count;
		WORD cur;
	};
	#pragma pack(pop)

	#pragma pack(push, 1)
		struct s_console_msg
		{
			char* msg_ptr;
			DWORD unk; // always 0
			char msg[0x50];

			s_console_msg(const char* text)
			{
				memset(msg, 0, 0x50);
				strcpy_s(msg, 0x50, text);
				unk = 0;
				msg_ptr = msg;
			}
		};
#pragma pack(pop)


	#pragma pack(push, 1)
	//! Represents Halo's connection information structure (ip, port, keys etc)
	struct s_connection_info
	{
		BYTE ip[4];
		WORD port;
		WORD pad;
		BYTE unk[0x148]; // handshake stuff etc, don't care atm.
	};
	static_assert(sizeof(s_connection_info) == 0x150, "incorrect s_connection_info");

	//! Represents an entry in Halo's machine table
	struct s_machine_info
	{
		s_connection_info*** con_info_ptr; 
		DWORD zero;
		DWORD zero1;
		WORD playerNum; // used for rcon etc
		WORD seven;
		BYTE unk[0x42];
		char key[10]; // only 7 chars long tho.. i think rest is padding
		DWORD machineNum; // 0 - 0xFFFFFFFF increased for each connection in server's life

		s_connection_info* get_con_info()
		{
			return **con_info_ptr;
		}
#ifdef PHASOR_CE
		char unk1[0x20];
		char ip[0x20];
		char cd_key_hash[0x20];
		char unk2[0x2c];
#endif
	};
	static_assert(sizeof(s_machine_info) == MACHINE_ENTRY_SIZE, "incorrect s_machine_info");

	//! Represents an entry in Halo's connection player table
	struct s_presence_item
	{
		wchar_t name[12];
		DWORD idk;
		BYTE machineId;
		BYTE status; // 1 = ok, 2 = invalid hash (or auth, or w/e)
		BYTE team;
		BYTE playerId;
	};
	static_assert(sizeof(s_presence_item) == 0x20, "incorrect s_presence_item");

	//! Server related items (name, gametype, players, machines)
	struct s_server_info
	{
		void* unk_ptr;
		WORD state; // 0 inactive, 1 game, 2 results
		WORD unk1;
		wchar_t server_name[0x42];
		char map_name[0x80];
		wchar_t gametype[0x18];
		BYTE unk2[0x69];
#ifdef PHASOR_CE
		BYTE unk_ce[0x40];
#endif
		BYTE max_players;//1e6 before this
		WORD unk3;
		BYTE cur_players;
		BYTE unk4;
		s_presence_item player_data[16];
		BYTE unk5[14];
		s_machine_info machine_table[16];
	};

	struct s_command_input {
		char password[9]; // don't forget to ensure null-termination
		char command[65];
	};
	static_assert(sizeof(s_command_input) == 74, "incorrect s_command_input");

	struct s_hash_validation {
		DWORD machineId;
		char hash[32];
		DWORD empty;
		DWORD requestId;
		UNKNOWN(0x0C);
		DWORD status; // 1 = valid, 2 = invalid
	};

	struct s_machinfo_info_partial_packet
	{
		char hash[32];
		char challenge[32];
		char clientKey[9]; // including null terminator
		UNKNOWN(1); // only seen 1, halo uses it for something 
		UNKNOWN(2); //probably padding
		wchar_t name[12]; // including null terminator		
	};

	#pragma pack(pop)

	struct s_phasor_mapcycle_entry
	{
		std::string map;
		std::wstring gametype;
		std::vector<std::string> scripts;
	};

#pragma pack(push, 1)
	struct s_script_list
	{
		DWORD count;
		char** script_names;
	};
	struct s_mapcycle_entry
	{
		char* map;
		char* gametype;
		s_script_list* scripts;
		BYTE gametype_data[GAMET_BUFFER_SIZE];
	};
	static_assert(sizeof(s_mapcycle_entry) == CONST_MENTRY_SIZE, 
		"sizeof(s_mapcycle_entry) != CONST_MENTRY_SIZE");

	struct s_mapcycle_header
	{
		s_mapcycle_entry* games;
		DWORD cur_count;
		DWORD allocated_count;
		DWORD current; // index of game being executed
	};
#pragma pack(pop)


	#pragma pack(push, 1)

	// Some structure issues were clarified thanks to the code at:
	// http://code.google.com/p/halo-devkit/source/browse/trunk/halo_sdk/Engine/objects.h
	// 

	struct s_halo_object // generic object header
	{
		ident map_id; // 0x0000
		UNKNOWN(12);
		UNKNOWN_BITFIELD(2);
		bool ignoreGravity : 1;
		UNKNOWN_BITFIELD(2);
		bool stationary : 1;
		UNKNOWN_BITFIELD(1);
		bool noCollision : 1; // has no collision box, projectiles etc pass right through
		UNKNOWN(3);
		unsigned long timer; // 0x0014
		UNKNOWN(0x44);// empty;
		vect3d location; // 0x005c
		vect3d velocity; // 0x0068
		vect3d rotation; // 0x0074
		vect3d someVector;
		UNKNOWN(sizeof(vect3d)); // some vector
		UNKNOWN(0x28);
		ident ownerPlayer; // 0x00c0 (index of owner (if has one))
		ident ownerObject; // 0x00c4 (object id of owner, if projectile is player id)
		UNKNOWN(0x18);
		float health; // 0x00e0
		float shield; // 0x00e4
		UNKNOWN(0x10);
		vect3d location1; // 0x00f8 set when in a vehicle unlike other one. best not to use tho (isnt always set)
		UNKNOWN(0x10);
		ident veh_weaponId; // 0x0114
		ident player_curWeapon; // 0x0118
		ident vehicleId; // 0x011C
		BYTE bGunner; // 0x0120
		short unkShort; // 0x0121
		BYTE bFlashlight; // 0x0123
		UNKNOWN(4);
		float shield1; // 0x0128 (same as other shield)
		float flashlightCharge; // 0x012C (1.0 when on)
		UNKNOWN(4);
		float flashlightCharge1; // 0x0134
		UNKNOWN(0xBC);

		//immune to backtap: 0xB4 (boolean)
		//	0x107 | 8 == immune to all damage
	};
	static_assert (sizeof(s_halo_object) == 0x1f4, "bad");

	struct s_halo_biped
	{
		s_halo_object base;
		UNKNOWN(0x10); // 0x1f4
		long invisible; // 0x204 (0x41 inactive, 0x51 active. probably bitfield but never seen anything else referenced)
		
		struct // 0x208
		{
			// these are action flags, basically client button presses
			// these don't actually control whether or not an event occurs
			
			bool crouching : 1; // 0
			bool jumping : 1; // 1
			UNKNOWN_BITFIELD(2);
			bool flashlight : 1; // 4
			UNKNOWN_BITFIELD(1);
			bool actionPress : 1; // 6 think this is just when they initially press the action button
			bool melee : 1; // 7
			UNKNOWN_BITFIELD(2);
			bool reload : 1; // 10
			bool primaryWeaponFire : 1; // 11 right mouse
			bool secondaryWeaponFire : 1; // 12 left mouse
			bool secondaryWeaponFire1 : 1; // 13
			bool actionHold : 1; // 14 holding action button
			UNKNOWN_BITFIELD(1);
		} actionFlags;
		UNKNOWN(0x26); // 0x020A
		vect3d cameraView; // 230
		UNKNOWN(0x68); // 0x23c
		BYTE bodyState; // 0x2A4 (2 = standing, 3 = crouching, 0xff = invalid, like in vehicle)
		UNKNOWN(0x53); // 0x2A5
		ident primaryWeaponId; // 0x2F8
		ident secondaryWeaponId; // 0x2FC
		ident tertiaryWeaponId; // 0x300
		ident quartaryWeaponId; // 0x304
		UNKNOWN(0x16); // 0x308
		BYTE frag_nade_count; // 0x31e
		BYTE plasma_nade_count; //0x31f
		BYTE zoomLevel; // 0x320 (0xff - no zoom, 0 - 1 zoom, 1 - 2 zoom etc)
		BYTE zoomLevel1; // 0x321
		UNKNOWN(0x1AA); // 0x322
		BYTE isAirbourne; // 0x4CC (from jumping/falling, not when flying in vehicle)
		UNKNOWN(0x33); // 0x4cd - 0x500
		UNKNOWN(0x0c);
		float crouch_percent;

		// The rest of bipd is composed of other tags, such as
		// mod2\characters\cyborg\cyborg
		// with these tags its size is 0xDEC
	};
	static_assert(sizeof(s_halo_biped) == 0x510, "bad");

	struct s_halo_weapon
	{
		s_halo_object base;
		UNKNOWN(0xC2); // 1f4
		WORD ammo_pack; // 2b6 reserve ammo
		WORD ammo_clip; // 2b8
		UNKNOWN(0x96);

		void SetAmmo(WORD pack, WORD clip);
		static void SyncAmmo(ident weaponId);

		// rest of weap is composed on other tags, with these tags
		// its size is 0x684
	};
	static_assert(sizeof(s_halo_weapon) == 0x350, "bad");

	struct s_halo_vehicle
	{
		s_halo_object base;
		UNKNOWN(0x3b8);
		DWORD idle_timer;
		UNKNOWN(0x010);
		// rest of vehi is composed of other tags, with these tags
		// its size is 0xE00
	};
	static_assert(sizeof(s_halo_vehicle) == 0x5c0, "bad");

	struct s_object_creation_disposition
	{
		ident map_id;
		UNKNOWN(4);
		ident player_ident;
		ident parent;
		UNKNOWN(8);
		vect3d pos;
		// rest unknown.. 
	};
	#pragma pack(pop)

	struct view_vector
	{
		vect3d pos;
		vect3d dir;
	};


	#pragma pack(push, 1)
	
	struct s_damage_info {
		ident tag_id;
		unsigned long flags;
		ident player_causer;
		ident causer; // obj of causer
		UNKNOWN(0x30);
		float modifier; // 1.0 = max dmg, < 0 decreases dmg.
		float modifier1; // 1.0 default > 1.0 increases dmg.
		UNKNOWN(8);
	};
	static_assert(sizeof(s_damage_info) == 0x50, "bad");

	struct s_hit_info {
		char desc[0x20];
		UNKNOWN(0x28); 	// doesn't seem to be that useful, mostly 0s with a few 1.0 floats.	
	};
	static_assert(sizeof(s_hit_info) == 0x48, "bad");

	struct s_damage_amount
	{
		float min_damage;
		float max_damage_min;
		float max_damage_max;

		s_damage_amount()
			: min_damage(0), max_damage_max(0), max_damage_min(0)
		{
		}

		s_damage_amount(float min_damage, float max_damage_min, float max_damage_max)
			: min_damage(min_damage), max_damage_min(max_damage_min), 
			  max_damage_max(max_damage_max)
		{
		}
	};
	static_assert(sizeof(s_damage_amount) == 0x0c, "bad");

	struct s_damage_tag
	{
		float min_range;
		float max_range;
		UNKNOWN(0x1BE);
		short damage_id;
		UNKNOWN(0x08);
		s_damage_amount amount;
		UNKNOWN(0xC4);
	};

	static_assert(sizeof(s_damage_tag) == 0x2A0, "bad");
	
	#pragma pack(pop)
	namespace damage_flags {
		static const int kNone =		0x00;
		static const int kRespawn =		0x02;
		static const int kInstantKill = 0x04;
	}

	struct damage_script_options
	{
		ident receiver;
		ident causer; // obj id
		ident causer_player; // player id
		ident tag;
		int flags;
		float modifier;
	};

	#pragma pack(push, 1)
	struct s_player_structure
	{
		WORD playerJoinCount; // 0x0000
		WORD localClient; // 0x0002 always FF FF on a dedi in Halo is 00 00 if its your player
		wchar_t playerName[12]; //0x0004
		UNKNOWN(4); // 0x001C only seen FF FF FF FF
		BYTE team; // 0x0020
		UNKNOWN(3); // padding?
		ident m_interactionObject; // 0x0024 ie Press E to enter THIS vehicle
		WORD interactionType; // 0x0028 8 for vehicle, 7 for weapon
		WORD interactionSpecifier; // 0x002A which seat of car etc
		DWORD respawnTimer; // 0x002c
		UNKNOWN(4); // 0x0030 only seen empty
		ident object_id; // 0x0034
		ident old_object_id; // 0x0038
		UNKNOWN(4); // 0x003C sometimes changes as you move, fuck idk
		UNKNOWN(4); // 0x0040 always FF FF FF FF, never accessed
		UNKNOWN(4); // 0x0044 changes when the player shoots
		wchar_t playerNameAgain[12]; // 0x0048
		WORD playerNum_NotUsed; // 0x0060 seems to be the player number.. never seen it accessed tho
		WORD empty1; // 0x0062 byte alignment
		BYTE playerNum; // 0x0064 player number used for rcon etc (ofc this is 0 based tho)
		BYTE unk_PlayerNumberHigh; // 0x0065 just a guess
		BYTE team_Again; // 0x0066
		UNKNOWN(5);
		float speed; // 0x006C
		UNKNOWN(0x2c); 
		WORD kills; // 0x009C
		UNKNOWN(6);
		WORD assists; // 0x00A4
		UNKNOWN(6);
		WORD betrayals; // 0x00AC
		WORD deaths; // 0x00AE
		WORD suicides; // 0x00B0
		// cbf with the rest
		vect3d oldCamera;
		UNKNOWN(0x142); // without old camera was 14e
	};
	static_assert(sizeof(s_player_structure) == 0x0200, "bad");
	#pragma pack(pop)

	

	// entry in the object table
	struct s_halo_object_header
	{
		WORD id;
		char flags; // 0x44 by default, dunno what they're for.
		char type;
		UNKNOWN(2);
		WORD size;
		union
		{
			void*					data;
			s_halo_object*	base;
			s_halo_biped*			biped;
		};
	};
	static_assert(sizeof(s_halo_object_header) == 0x0c, "s_halo_object_entry incorrect");

	struct s_halo_object_table
	{
		s_table_header header;
		s_halo_object_header entries[0x800];
	};	

	enum e_chat_types
	{
		// important: don't reorder!
		kChatAll = 0,
		kChatTeam,
		kChatVehicle,
		kChatServer, // phasor only
		kChatPrivate, // phasor only
	};

	#pragma pack(push, 1)
	struct s_chat_data
	{
		e_chat_types type;
		unsigned long player;
		const wchar_t* msg;
	};

#pragma pack(pop)




	enum e_tag_types
	{
		TAG_ACTV = 'actv',
		TAG_ANT  = 'ant!',
		TAG_ANTR = 'antr',
		TAG_BIPD = 'bipd',
		TAG_BTIM = 'bitm',
		TAG_COLL = 'coll',
		TAG_COLO = 'colo',
		TAG_CONT = 'cont',
		TAG_DECA = 'deca',
		TAG_DELA = 'DeLa',
		TAG_DEVC = 'devc',
		TAG_EFFE = 'effe',
		TAG_EQIP = 'eqip',
		TAG_FLAG = 'flag',
		TAG_FONT = 'font',
		TAG_FOOT = 'foot',
		TAG_GRHI = 'grhi',
		TAG_HMT = 'hmt ',
		TAG_HUD = 'hud#',
		TAG_HUDG = 'hudg',
		TAG_ITMC = 'itmc',
		TAG_JPT = 'jpt!',
		TAG_LENS = 'lens',
		TAG_LIGH = 'ligh',
		TAG_LSND = 'lsnd',
		TAG_MATG = 'matg',
		TAG_METR = 'metr',
		TAG_MGS2 = 'mgs2',
		TAG_MOD2 = 'mod2',
		TAG_PART = 'part',
		TAG_PCTL = 'pctl',
		TAG_PHYS = 'phys',
		TAG_PPHY = 'pphy',
		TAG_PROJ = 'proj',
		TAG_SBSP = 'sbsp',
		TAG_SCEN = 'scen',
		TAG_SCEX = 'scex',
		TAG_SCHI = 'schi',
		TAG_SCNR = 'scnr',
		TAG_SENV = 'senv',
		TAG_SGLA = 'sgla',
		TAG_SKY = 'sky ',
		TAG_SMET = 'smet',
		TAG_SND = 'snd!',
		TAG_SNDE = 'snde',
		TAG_SOUL = 'Soul',
		TAG_SPLA = 'spla',
		TAG_SSCE = 'ssce',
		TAG_STR = 'str#',
		TAG_TAGC = 'tagc',
		TAG_TRAK = 'trak',
		TAG_UDLG = 'udlg',
		TAG_UNHI = 'unhi',
		TAG_USTR = 'ustr',
		TAG_VEHI = 'vehi',
		TAG_WEAP = 'weap',
		TAG_WIND = 'wind',
		TAG_WPHI = 'wphi'
	};
	
	#pragma pack(push, 1)
	struct s_tag_type
	{
		unsigned long val;
		explicit s_tag_type(const char* str);

		bool operator==(const s_tag_type& other);
		bool operator==(e_tag_types type);
		bool operator!=(const s_tag_type& other);
		bool operator!=(e_tag_types type);
		char* GetString(char out[5]);
	};
	static_assert(sizeof(s_tag_type) == 0x4, "bad");

	// Structure of the tag header (entry in tag table)
	struct s_tag_entry
	{
		s_tag_type tagType; // ie weap
		UNKNOWN(8);
		ident id; // unique id for map
		const char* tagName; // name of tag
		void* metaData; // data for this tag
		UNKNOWN(8);
	};
	static_assert(sizeof(s_tag_entry) == 0x20, "bad");

	// Structure of tag index table
	struct s_tag_index_table_header
	{
		s_tag_entry* next_ptr;
		unsigned long starting_ident; // ??
		UNKNOWN(4);
		unsigned long entityCount;
		UNKNOWN(4);
		unsigned long readOffset;
		UNKNOWN(8);
		unsigned long readSize;
		UNKNOWN(4);
	};
	static_assert(sizeof(s_tag_index_table_header) == 0x28, "bad");

	struct s_object_info
	{
		s_tag_type tagType;
		char* tagName;
		unsigned long empty;
		ident id;
	};

#pragma pack(pop)

// ----------------------------------------------------------------
	// Structures used for map processing
	// ----------------------------------------------------------------
	#pragma pack(push, 1)

	// Structure of map header
	struct hMapHeader
	{
		DWORD integrity; // should be 'head' if not corrupt
		DWORD type; // 7 for halo PC, ce is 0x261
		DWORD filesize;
		DWORD highSize;
		DWORD offset1;
		BYTE unk1[12]; // Unknown atm
		char name[32]; // map name
		char version[32]; // version the map was built at
		DWORD mapType; // 0 campaign, 1 multiplayer, 2 data (ui.map)
		BYTE unk2[0x798]; // first few bytes have data, rest are 00
		DWORD integrity2; // should be 'foot' if not corrupt
	};

	struct mapTableInfo
	{
		char* map;
		DWORD index;
		DWORD empty;
	};

	#pragma pack(pop)

	static const size_t kMaxMapLength = 0x20;

	struct s_player_table
	{
		s_table_header header;
		s_player_structure players[16];
	};


///====================
// Phasor type
struct DamageModifier {
		s_tag_entry* tag;
		s_tag_entry orig_tag;
		s_damage_amount orig_dmg;
		s_damage_tag* data;

		explicit DamageModifier(s_tag_entry* tag)
			: tag(tag), orig_tag(*tag)
		{
			data = (s_damage_tag*)tag->metaData;
			orig_dmg = data->amount;
		}

		~DamageModifier() {
			*tag = orig_tag;
			data->amount = orig_dmg;
		}
	};
