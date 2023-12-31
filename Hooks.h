#pragma once
typedef void(__stdcall* tLookId)(int32_t a);
//using tPrintText = int(__stdcall*)(int32_t nSurface, int32_t nX, int32_t nY, int32_t nFont, int32_t nRed, int32_t nGreen, int32_t nBlue, const char* lpText, int32_t nAlign);
typedef int(__stdcall* tPrintText)(int32_t nSurface, int32_t nX, int32_t nY, int32_t nFont, int32_t nRed, int32_t nGreen, int32_t nBlue, const char* lpText, int32_t nAlign);
typedef void(__stdcall* tSayHook)(int32_t a, const char* message);
typedef void(__stdcall* sayFunc)(int32_t a, const char* message);
typedef void(__stdcall* tPlaceObject)(int nX, int nY, int nZ, int nID, int nCount, int u1);
namespace Hooks
{
	extern bool bComboBot;

	extern uintptr_t moduleBase;
	extern ComboBotSpells comboBotSpells;
	extern bool bLookIds;
	extern bool bComboBot;
	extern bool bMwTimersMilliseconds;
	extern bool bMwTimersDynamic;
	extern bool bMwTimersStatic;
	extern tLookId hLookId;
	extern tPrintText ohkPrintText;
	extern tSayHook hSayHookAssembly;
	extern sayFunc say;

	extern uintptr_t lookIdHook;
	extern uintptr_t lookIdOriginal;

	extern uintptr_t printText1;
	extern uintptr_t printTextOriginal;

	extern int32_t timeVarComboBot;

	extern CSelfCharacter localPlayer;
	extern GameClient2* gc;
	extern std::array< MagicWall, 128> magicWalls;
	extern uintptr_t placeObject;
	extern uintptr_t placeObjectOriginal;
	extern int32_t magicWallCount;


	void InitHooks();
	void UnHook();
	void printMagicWallTimers();
};