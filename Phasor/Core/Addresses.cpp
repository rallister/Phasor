

// This file is used to store all memory addresses Phasor uses
// ------------------------------------------------------------------------
// 
// Memory addresses
//
unsigned long ADDR_CONSOLEINFO = 0x0063BC1C;
unsigned long ADDR_RCONPLAYER = ADDR_CONSOLEINFO + 0x10;
unsigned long ADDR_TAGTABLE = 0x00460678;
unsigned long ADDR_PLAYERINFOBASE = 0x0069B91C;
unsigned long ADDR_OBJECTBASE = 0x00744C18;
unsigned long ADDR_PLAYERBASE = 0x0075ECE4;
unsigned long ADDR_MAPCYCLEINDEX = 0x00614B58;
unsigned long ADDR_MAPCYCLELIST = 0x00614B4C;
unsigned long ADDR_MAPCYCLECOUNT = 0x00614B50;
unsigned long ADDR_NEWGAMEMAP = 0x006713D8;
unsigned long ADDR_CURMAPCOUNT = 0x00692480;
unsigned long ADDR_MAXMAPCOUNT = 0x00692484;
unsigned long ADDR_SOCKETREADY = 0x0069B91C;
unsigned long ADDR_GAMEREADY = 0x00698DC8;
unsigned long ADDR_PREPAREGAME_FLAG = 0x00694528;
unsigned long ADDR_CMDCACHE = 0x0063FED4;
unsigned long ADDR_CMDCACHE_INDEX = 0x006406CE;
unsigned long ADDR_CMDCACHE_CUR = 0x006406D0;
unsigned long ADDR_GAMETYPE = 0x00671340;
unsigned long ADDR_PORT = 0x00625230;
unsigned long ADDR_SERVERNAME = 0x006265B0;
unsigned long ADDR_CONSOLEREADY = 0x0063BC28;
unsigned long ADDR_SERVERINFO = 0x00671420;

// ------------------------------------------------------------------------
//
// Function addresses
//
unsigned long FUNC_HALOGETHASH			= 0x0059BBB0;
unsigned long FUNC_EXECUTESVCMD			= 0x004EB7E0;
unsigned long FUNC_ONPLAYERDEATH		= 0x00490050;
unsigned long FUNC_ACTIONDEATH_1		= 0x00524410;
unsigned long FUNC_ACTIONDEATH_2		= 0x0057D510;
unsigned long FUNC_ACTIONDEATH_3		= 0x00495A10;
unsigned long FUNC_DOINVIS				= 0x0049AAA0;
unsigned long FUNC_PLAYERJOINING		= 0x00517290;
unsigned long FUNC_TEAMSELECT			= 0x00490940;
unsigned long FUNC_GETMAPPATH			= 0x0045FC20;
unsigned long FUNC_VALIDATEGAMETYPE		= 0x00481830;
unsigned long FUNC_BUILDPACKET			= 0x00522E30;
unsigned long FUNC_ADDPACKETTOQUEUE		= 0x00516610;
unsigned long FUNC_ADDPACKETTOPQUEUE	= 0x00516460;
unsigned long FUNC_AFTERSPAWNROUTINE	= 0x00498920;
unsigned long FUNC_EXECUTEGAME			= 0x0047F0E0;
unsigned long FUNC_PREPAREGAME_ONE		= 0x004ED240;
unsigned long FUNC_PREPAREGAME_TWO		= 0x005193F0;
unsigned long FUNC_BANPLAYER			= 0x00518890;
unsigned long FUNC_SAVEBANLIST			= 0x00518270;
unsigned long FUNC_UPDATEAMMO			= 0x004E83E0;
unsigned long FUNC_CREATEOBJECTQUERY	= 0x0052C4F0;
unsigned long FUNC_CREATEOBJECT			= 0x0052C600;
unsigned long FUNC_DESTROYOBJECT		= 0x0052CD20;
unsigned long FUNC_PLAYERASSIGNWEAPON	= 0x00582C60;
unsigned long FUNC_NOTIFY_WEAPONPICKUP	= 0x00499EF0;
unsigned long FUNC_ENTERVEHICLE			= 0x0049A2A0;
unsigned long FUNC_EJECTVEHICLE			= 0x00580B00;
unsigned long FUNC_HALOEXCEPTIONHANDLER = 0x005B036C;

// ------------------------------------------------------------------------
//
// Codecave addresses
//
unsigned long CC_PHASORLOAD = 0x00595A12;
unsigned long CC_CONSOLEPROC = 0x004EB325;
unsigned long CC_CONSOLEHANDLER = 0x004B9FF0;
unsigned long CC_SERVERCMD = FUNC_EXECUTESVCMD;
unsigned long CC_GAMEEND = 0x00486B80;
unsigned long CC_PLAYERWELCOME = 0x0051692B;
unsigned long CC_CHAT = 0x004CEBC7;
unsigned long CC_MAPLOADING = 0x00483376;
unsigned long CC_TEAMSELECTION = 0x00513BB4;
unsigned long CC_NEWGAME = 0x0047B3B0;
unsigned long CC_PLAYERQUIT = 0x00494780;
unsigned long CC_TEAMCHANGE = 0x00490AD2;
unsigned long CC_DEATH = 0x00480168;
unsigned long CC_KILLMULTIPLIER = 0x004800D9;
unsigned long CC_OBJECTINTERACTION = 0x0049977B;
unsigned long CC_PLAYERSPAWN = 0x0049909A;
unsigned long CC_PLAYERSPAWNEND = 0x004990DF;
unsigned long CC_VEHICLEENTRY = 0x0049A395;
unsigned long CC_WEAPONRELOAD = 0x004E8303;
unsigned long CC_DAMAGELOOKUP = 0x00524fd0;
unsigned long CC_DAMAGEAPPLICATION = 0x00525864;
unsigned long CC_WEAPONASSIGN = 0x005827AC;
//unsigned long CC_WEAPONCREATION = 0x004E5F3F;
//unsigned long CC_WEAPONCREATION = 0x0052CA29;
unsigned long CC_OBJECTCREATION = 0x0052CA1B; // all objects??
unsigned long CC_MAPCYCLEADD = 0x005191CE;
unsigned long CC_CLIENTUPDATE = 0x00578D6D;
unsigned long CC_EXCEPTION_HANDLER = 0x005B036C;
unsigned long CC_VEHICLERESPAWN = 0x00586D81;
unsigned long CC_EQUIPMENTDESTROY = 0x0047E61C;
unsigned long CC_VEHICLEFORCEEJECT = 0x0056E6CD;
unsigned long CC_VEHICLEUSEREJECT = 0x0056E107;
unsigned long CC_HALOPRINT = 0x004BA3F0;
unsigned long CC_HALOBANCHECK = 0x00518820;
unsigned long CC_OBJECTDESTROY = FUNC_DESTROYOBJECT + 6;

// ------------------------------------------------------------------------
//
// Patch addresses
//
unsigned long PATCH_ALLOCATEMAPNAME = 0x005191C4;
unsigned long PATCH_MAPTABLEALLOCATION = 0x004B82D1;
unsigned long PATCH_MAPTABLE = 0x0069247C;
unsigned long PATCH_MAPLOADING = 0x0047EF22;
unsigned long PATCH_NOMAPPROCESS = 0x00483280;
unsigned long PATCH_TEAMSELECTION = 0x00513BAE;

// TO FIND SIGNATURES FOR
unsigned long ADDR_BROADCASTVERSION = 0x005df840;
unsigned long ADDR_HASHLIST = 0x006A2AE4;
unsigned long ADDR_SERVERSTRUCT = 0x00745BA0;
unsigned long CC_OBJECTCREATIONATTEMPT = 0x52c600;
unsigned long ADDR_RCONPASSWORD = 0x0069ba5c;
unsigned long CC_SERVERCMDATTEMPT = 0x0051a26a;
unsigned long PATCH_SERVERNAME1 = 0x00517d59;
unsigned long PATCH_SERVERNAME2 = PATCH_SERVERNAME1 + 0x12;
unsigned long PATCH_CURRENTVERSION =  0x005152e3;
unsigned long PATCH_ANYVERSIONCHECK1 = PATCH_CURRENTVERSION + 4;
unsigned long PATCH_ANYVERSIONCHECK2 = PATCH_ANYVERSIONCHECK1 + 0x0B;
unsigned long CC_HASHVALIDATE = 0x0059BD97;
unsigned long CC_HASHVALIDATE_VALID = CC_HASHVALIDATE - 0x2f;
unsigned long FUNC_VERIFYMAP_CE = 0x0048d980;
unsigned long FUNC_VEHICLERESPAWN1 = 0x0052C310;
unsigned long FUNC_VEHICLERESPAWN2 = 0x0052C2B0;
unsigned long CC_MACHINECONNECT = 0x0051596c;
unsigned long CC_MACHINEDISCONNECT = 0x00515bd9;
unsigned long CC_MACHINEINFOFIX = 0x00516e39;
unsigned long FUNC_INTERSECT = 0x0053d8d0;


	