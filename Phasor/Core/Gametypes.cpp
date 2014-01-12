#include "Gametypes.h"
#include "Directory.h"
#include "Addresses.h"
#include "Globals.h"

#include "../Common/FileIO.h"
#include "../Common/MyString.h"


#include <map>
#include <stdio.h>
#include <string>
#include <vector>
#include <stack>
#include <iostream>

using namespace std;


	std::map<std::wstring, BYTE*> gametypes;

	void ClearGameList();

	std::wstring normalizeGametype(const std::wstring& gametype)
	{
		std::wstring lower = gametype;
		ToLowercase(lower);
		return lower;
	}

	bool BuildGametypeList2()
	{
		bool xx1 = ListFiles(g_ProfileDirectory + L"saved", L"blam.lst");
		bool xx2 = ListFiles(g_ProfileDirectory + L"savegames", L"blam.lst");
		return xx1||xx2;
	}

	bool ListFiles(wstring path, wstring mask) 
	{
		HANDLE hFind = INVALID_HANDLE_VALUE;
		WIN32_FIND_DATAW ffd;
		wstring spec;
		stack<wstring> directories;

		directories.push(path);		
/*
		while (!directories.empty()) {
			path = directories.top();
			spec = path + L"\\*";

			_TRACE(".... spec=%S\r\n", spec.c_str());

			directories.pop();

			hFind = FindFirstFileW(spec.c_str(), &ffd);
			if (hFind == INVALID_HANDLE_VALUE)  {
				_TRACE("handle invalid\r\n");				
				continue;
			} 
			
			do 
			{				
				if (wcscmp(ffd.cFileName, L".") != 0 && 
					wcscmp(ffd.cFileName, L"..") != 0) {
					if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
						directories.push(path + L"\\" + ffd.cFileName);
					}
					else 
					{						
						if(wcscmp(ffd.cFileName, L"blam.lst") == 0)
						{
							_TRACE("File found : %S\\%S\r\n", path, ffd.cFileName);
					
							CInFile file;
							if (!file.Open(path + L"\\" + ffd.cFileName)) 
							{
								_TRACE("Could not open %S\\%S\n\n", path, ffd.cFileName);
								continue;
							}

							// will change it later.
							DWORD read = 0;
							BYTE* gametypeData = (BYTE*)GlobalAlloc(GMEM_FIXED, file.GetFileSize());
							file.Read(gametypeData, file.GetFileSize(), &read); // all mine are 8k, does not match up with what was said about format.							
							file.Close();

							WCHAR* tf = (WCHAR*)(gametypeData);
							wstring gametypename(reinterpret_cast<wchar_t const*>(tf), 24);
							ToLowercase(gametypename);

							//s_blam* blam = (s_blam*)gametypeData;
							

							//blam->WeaponSet=1;
							
							
							//blam->fUnknown=22.0f;
							//blam->Checksum = 0;

							std::wstring wFilePath = path + L"\\" + ffd.cFileName;

							_TRACE("Found gametype %S, %d, %S\n", gametypename.c_str(), wFilePath.c_str());

							gametypes.insert(pair<wstring, BYTE*>(gametypename.c_str(), gametypeData));

							_TRACE("....gametype %S\r\n", gametypename.c_str());
						}
					}
				}
			} while (FindNextFileW(hFind, &ffd) != 0);

			if (GetLastError() != ERROR_NO_MORE_FILES) {
				FindClose(hFind);
				return false;
			}

			FindClose(hFind);
			hFind = INVALID_HANDLE_VALUE;

			
		}
		*/
		return true;
	}

	bool BuildGametypeList()
	{		
		ClearGameList();
		return BuildGametypeList2();
	}

	void ClearGameList()
	{
		map<wstring, BYTE*>::iterator iter;

		for (iter = gametypes.begin(); iter != gametypes.end(); ++iter) 
		{  
           GlobalFree(iter->second);
        }

		gametypes.clear(); 
	}

	

	bool ReadGametypeData(const std::wstring& gametype, BYTE* out, DWORD outSize)
	{
		/*
		auto itr = gametypes.find(normalizeGametype(gametype));
		if (itr == gametypes.end()) 
			return false;

		BYTE* bytes = itr->second;
	   */

		s_blam blam_global;
		memset(&blam_global, 0, sizeof (s_blam));

		s_blam* blam = &blam_global;

		memcpy(blam->GameTypeName, "m\0y\0c\0t\0f", 9);

		blam->GameType = 5;
		blam->TeamPlay = 0; //ok

		blam->PlayersOnRadar	=	1;
		blam->FriendIndicators	=	1;
		blam->InfiniteGrenades	=	0; //ok
		blam->NoShields			=	0; // 1 = off, 0 = on
		blam->Invisible			=	0;
		blam->StartEquipment	=	1;	// 0 Generic, 1 Custom	
		blam->FriendsOnRadar	=	1;
		blam->BallIndicator		=	1;

		blam->Indicator = 1;            // 0 Motion tracker, 1 Navpoints, 2 None
		blam->OddManOut = 0;            // 0 No, 1 Yes
		blam->RespawnTimeGrowth = 0;	// 0 Off, 30 units per second eg: 150(0x96) = 30*5 secs
		blam->RespawnTime = 1;
		blam->SuicidePenalty = 1;
		blam->NumOfLives = 0;			// 0 Unlimited, 1 1 life, 3 3 lives, 5 5 lives
		blam->HealthPercent = 1.0f;               // 1.0f
		blam->ScoreLimit = 1;           // Captures for CTF, laps for RACE, kills for Slayer, minutes for King, etc
		blam->WeaponSet = 4; 

		blam->RespawnTime = 1; // 1000 -> 38 seconds so it's not 1 to 1 mapping.
		blam->RespawnTimeGrowth = 0;

		blam->RedCustom=0xff;
		blam->RedWarthog=0xff;
		blam->RedGhost=0xff;
		blam->RedScorpion=0xff;
		blam->RedRocketWarthog=4;
		blam->RedBanshee=1;		
		blam->RedTurret=0xff;
		blam->RedZero=0xff;
		blam->RedUnused=0xff;

		blam->BlueCustom=0xff;
		blam->BlueWarthog=0xff;
		blam->BlueGhost=0xff;
		blam->BlueScorpion=0xff;
		blam->BlueRocketWarthog=4;
		blam->BlueBanshee=1;		
		blam->BlueTurret=0xff;
		blam->BlueZero=0;
		blam->BlueUnused=0;

		blam->VehicleRespawnTime = 1800;

		blam->FriendlyFire = 0;			// 0 Off, 1 On
		blam->TKPenalty = 0;
		blam->AutoTeamBalance = 0;		// 0 Off, 1 On
		blam->GameTimeLimit = 37000;
		blam->TypeFlags  = 1;			// Moving hill 0 Off; 1 On (KOTH)  Racetype 0 Normal; 1 Any order; 2 Rally (Race) Random start 0 No; 1 Yes (Oddball)
		blam->TeamScoring = 1;			// Team scoring 0 Minimum; 1 Maximum; 2 Sum (Race), Ballspeed 0 Slow; 1 Normal; 2 Fast (Oddball)
								
		blam->AssaultTimeLimit = 0;		// 0 Two flags
		blam->Unused1 = 0;
		blam->TraitWithBall = 1;		// 0 None, 1 Invisible, 2 Extra damage, 3 Damage Resistent 
		blam->TraitWithoutBall = 0;		// 0 None, 1 Invisible, 2 Extra damage, 3 Damage Resistent
		blam->BallType = 1;             // 0 Normal, 1 Reverse Tag, 2 Juggernaut 
		blam->BallCountSpawn = 1;
		blam->One = 1;                    // Always 1 ( 0 if custom )
		blam->GameTypeNum = 0;            // # of the game in the game list ( 0000 - for a custom game type )
		blam->Unused2 = 15;
		blam->Checksum = 0;

		memcpy((char*)out, &blam ,sizeof(s_blam));
		return true;
	}

	bool IsValidGametype(const std::wstring& gametype)
	{
		return gametypes.find(normalizeGametype(gametype)) != gametypes.end();
	}

