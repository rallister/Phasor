#include "Directory.h"
#include "../Common/MyString.h"
#include "../Common/FileIO.h"
#include <windows.h>
#include <vector>

using namespace std;

wstring g_ProfileDirectory;
wstring g_DataDirectory;
wstring g_CrashDirectory;
wstring g_ScriptsDirectory;
wstring g_LogsDirectory;
wstring g_OldLogsDirectory;
wstring g_MapDirectory;

void CreateSubDirectory(const wstring& name, wstring& out, 
	const wstring& relative = g_ProfileDirectory)
{
	out = relative + name;
	if (!NDirectory::CreateDirectory(out)) {
		std::string err = "The " + NarrowString(name) + " directory (" + 
			NarrowString(out) +	") couldn't be created.";
		throw std::exception(err.c_str());
	}
	NDirectory::NormalizeDirectory(out);
}

void SetupDirectories()
{
	string cmdline = NarrowString(GetCommandLineW());
	vector<string> tokens = TokenizeArgs(cmdline);

	string exePath = tokens[0]; 

	string curDir = exePath.substr(0, exePath.find_last_of('\\') + 1); // haloceded.exe
	
	string profilePath = curDir + "profile\\";
	string dataPath = profilePath + "\\data\\";
	string mapPath = profilePath + "\\maps\\";
	string scriptPath = profilePath + "\\scripts\\";

	for(size_t i = 1; i < tokens.size(); i++)
	{
		if(tokens[i] == "-path")
			profilePath = (i+1) < tokens.size()? tokens[++i] : profilePath;
		else
		if(tokens[i] == "-mappath")
			mapPath = (i+1) < tokens.size()? tokens[++i] : mapPath;
		else
		if(tokens[i] == "-scriptpath")
			scriptPath = (i+1) < tokens.size()? tokens[++i] : scriptPath;	
	}

	g_ProfileDirectory = WidenString(profilePath);
	NDirectory::NormalizeDirectory(g_ProfileDirectory);
	g_MapDirectory = WidenString(mapPath);
	g_ScriptsDirectory = WidenString(scriptPath);
	g_DataDirectory = WidenString(dataPath);

	NDirectory::NormalizeDirectory(g_MapDirectory);
	if (g_ScriptsDirectory.size()) 
		NDirectory::NormalizeDirectory(g_ScriptsDirectory);

	CreateSubDirectory(L"data", g_DataDirectory);
	if (!g_ScriptsDirectory.size()) 
		CreateSubDirectory(L"scripts", g_ScriptsDirectory);

	CreateSubDirectory(L"logs", g_LogsDirectory);
	CreateSubDirectory(L"old", g_OldLogsDirectory, g_LogsDirectory);
	CreateSubDirectory(L"crash", g_CrashDirectory);
}
