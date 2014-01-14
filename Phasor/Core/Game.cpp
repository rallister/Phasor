
#include "Game.h"

#include "Globals.h"


//===================================================================================

s_mapcycle_header * g_mapcycle_header = (s_mapcycle_header*)ADDR_MAPCYCLELIST;
s_mapcycle_entry* g_the_game;

//===================================================================================

s_blam* config_ctf(s_blam* blam)
{
	blam->GameType = GAMETYPE_CTF;
	blam->ScoreLimit = 3;
	return blam;
}

s_blam* config_slanoobie(s_blam* blam)
{
	blam->GameType = GAMETYPE_SLAYER;
	blam->ScoreLimit = 25;
	blam->WeaponSet = WEAPONSET_HEAVY;
	blam->NoShields = 0;
	blam->GameTimeLimit = 16000;
	blam->TeamPlay = 0;
	return blam;
}

s_blam* default_blam(s_blam* blam)
{	
	blam->GameType = GAMETYPE_CTF;
	blam->TeamPlay = 1; //ok		

	blam->PlayersOnRadar		=	1;
	blam->FriendIndicators	=	1;
	blam->InfiniteGrenades	=	0; //ok
	blam->NoShields			=	1; // 1 = off, 0 = on
	blam->Invisible			=	0;
	blam->StartEquipment	=	1;	// 0 Generic, 1 Custom	
	blam->FriendsOnRadar	=	1;
	blam->BallIndicator		=	1;

	blam->Indicator = 1;            // 0 Motion tracker, 1 Navpoints, 2 None
	blam->OddManOut = 0;            // 0 No, 1 Yes
	blam->RespawnTimeGrowth = 0;	// 0 Off, 30 units per second eg: 150(0x96) = 30*5 secs
	blam->RespawnTime = 30;
	blam->SuicidePenalty = 0;
	blam->NumOfLives = 0;			// 0 Unlimited, 1 1 life, 3 3 lives, 5 5 lives
	blam->HealthPercent = 1.0f;
	blam->ScoreLimit = 1;           // Captures for CTF, laps for RACE, kills for Slayer, minutes for King, etc
	blam->WeaponSet = WEAPONSET_SNIPE; 

	blam->RedCustom=8;
	blam->RedWarthog=4;
	blam->RedGhost=2;
	blam->RedScorpion=0;
	blam->RedRocketWarthog=1;
	blam->RedBanshee=1;		
	blam->RedTurret=0;
	blam->RedZero=0;
	blam->RedUnused=0xE;

	blam->BlueCustom=8; 
	blam->BlueWarthog=1;
	blam->BlueGhost=2;
	blam->BlueScorpion=3;
	blam->BlueRocketWarthog=4;
	blam->BlueBanshee=1;		
	blam->BlueTurret=2;
	blam->BlueZero=3;
	blam->BlueUnused=0xE;

	blam->VehicleRespawnTime = 1800;

	blam->FriendlyFire = 0;			// 0 Off, 1 On
	blam->TKPenalty = 0;
	blam->AutoTeamBalance = 0;		// 0 Off, 1 On
	blam->GameTimeLimit = 38000;

	blam->TypeFlags  = 0;			// Moving hill 0 Off; 1 On (KOTH)  Racetype 0 Normal; 1 Any order; 2 Rally (Race) Random start 0 No; 1 Yes (Oddball)
	blam->TeamScoring = 0;			// Team scoring 0 Minimum; 1 Maximum; 2 Sum (Race), Ballspeed 0 Slow; 1 Normal; 2 Fast (Oddball)								
	blam->AssaultTimeLimit = 0;		// 0 Two flags
	blam->Unused1 = 0;
	blam->TraitWithBall = 0;		// 0 None, 1 Invisible, 2 Extra damage, 3 Damage Resistent 
	blam->TraitWithoutBall = 0;		// 0 None, 1 Invisible, 2 Extra damage, 3 Damage Resistent
	blam->BallType = 0;             // 0 Normal, 1 Reverse Tag, 2 Juggernaut 
	blam->BallCountSpawn = 0;
	blam->One = 1;                    // Always 1 ( 0 if custom )
	blam->GameTypeNum = 0;            // # of the game in the game list ( 0000 - for a custom game type )

	blam->Unused2 = 0;
	//blam->Checksum = 0;

	return blam;
}

bool ReadBlam(BYTE* out, char* gametype)
{
	s_blam blam;
	memset(&blam, 0, sizeof (s_blam));

	wstring gname = WidenString(gametype);
	memcpy(blam.GameTypeName, gname.c_str(), min(MAX_GAMETYPE_NAME_LEN, gname.length() * 2)); 
	default_blam(&blam);

	if(strcmp(gametype,"ctf") == 0)
		config_ctf(&blam);
	else
		config_slanoobie(&blam);
		
	BYTE* bv = (BYTE*)(&blam);
	/*
	bv+=0x7c;
	*bv = 0xff;
	bv++;
	*bv = 0xff;
	bv++;
	*bv = 0xff;
	bv++;
	*bv = 0xff;
	*/


	bv += 176;

	//*bv = 0;

	FILE * ff = fopen("ctf_blam.lst","w");

	BYTE * blampr = (BYTE*)&blam;
	for(int i = 0; i < sizeof(s_blam);i++)
		fprintf(ff, "%c", blampr[i]);
		
	fclose(ff);
	_TRACE("blam size is %d", sizeof(s_blam));


	memcpy((char*)out, &blam ,sizeof(s_blam));
	return true;
}

void reset_mapcycle_entry(s_mapcycle_entry* g)
{
	if(!g)
		return;
	if(g->gametype)
		memset(g->gametype, 0, kMaxMapLength);
	if(g->map)
		memset(g->map, 0, kMaxMapLength);

	memset(g->gametype_data, 0, sizeof(s_blam));
}

int currentGame = 0;

bool StartNextGame()
{	
	s_mapcycle_entry * game = g_the_game; // just in case, there could be trouble.

	if(currentGame == 1)
	{
		currentGame = 0;
		game->gametype = "ctf";
		game->map = "bloodgulch";
	}
	else
	{
		currentGame = 1;
		game->gametype = "slanoobie";
		game->map = "prisoner";
	}

	ReadBlam(game->gametype_data, game->gametype);

	s_blam* blam = (s_blam*)&game->gametype_data;

	//memcpy(game->gametype, blam->GameTypeName,wcslen(blam->GameTypeName));

	//memcpy(game->map, "bloodgulch", strlen("bloodgulch")); // (char*)GlobalAlloc(GMEM_FIXED, kMaxMapLength);

	

	if(IsValidMap(game->map))
	{	
		StartGame(game->map);
	}

	return true;
}



bool StartServer()
{
	g_mapcycle_header = (s_mapcycle_header*)ADDR_MAPCYCLELIST;

	// allocate just one entry, see how that rolls, coz after it's loaded it is modifiable.
	g_the_game = (s_mapcycle_entry*)GlobalAlloc(GMEM_FIXED, sizeof(s_mapcycle_entry));

	g_the_game->gametype = (char*)GlobalAlloc(GMEM_FIXED, MAX_GAMETYPE_NAME_LEN);	
	g_the_game->map = (char*)GlobalAlloc(GMEM_FIXED, kMaxMapLength);

	reset_mapcycle_entry(g_the_game);

	g_mapcycle_header->games = g_the_game;
	g_mapcycle_header->allocated_count = 1;	
	g_mapcycle_header->cur_count = 1;
	g_mapcycle_header->current = 1;

	return true;
}