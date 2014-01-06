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
		bool xx1 =  ListFiles(g_ProfileDirectory + L"saved", L"blam.lst");
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

							std::wstring wFilePath = path + L"\\" + ffd.cFileName;

							// wchar breaks logging here for some reason. todo: remove all the fucking unicode.
							_TRACE("Found gametype %S, %S\n", gametypename.c_str(), wFilePath.c_str());

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

	bool ReadGametypeData(const std::wstring& gametype, BYTE* out,
		DWORD outSize)
	{
		auto itr = gametypes.find(normalizeGametype(gametype));
		if (itr == gametypes.end()) 
			return false;

		BYTE* bytes = itr->second;
		memcpy((char*)out, (char*)bytes ,outSize-4);
		return true;
	}

	bool IsValidGametype(const std::wstring& gametype)
	{
		return gametypes.find(normalizeGametype(gametype)) != gametypes.end();
	}

