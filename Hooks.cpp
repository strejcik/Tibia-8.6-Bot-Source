#include "pch.h"
#include "Hooks.h"
#include "Menu.h"

namespace Hooks
{
	std::array< MagicWall, 128> magicWalls;
	int32_t timeVarComboBot = 0;
	uintptr_t moduleBase;
	ComboBotSpells comboBotSpells;
	bool bLookIds;
	bool bComboBot;
	bool bMwTimersDynamic = true;
	bool bMwTimersStatic;
	bool bMwTimersMilliseconds;
	tLookId hLookId;
	tPlaceObject hPlaceObject;

	int32_t* playerMovingX;
	int32_t* playerMovingY;
	
	tPrintText ohkPrintText;
	tSayHook hSayHookAssembly;

	sayFunc say;

	uintptr_t sayHook;
	uintptr_t sayHookOriginal;

	GameClient2* gc;



	uintptr_t lookIdHook;
	uintptr_t lookIdOriginal;

	uintptr_t printText1;
	uintptr_t printTextOriginal;

	uintptr_t placeObject;
	uintptr_t placeObjectOriginal;
	CSelfCharacter localPlayer;
	int32_t magicWallCount;
}




__declspec(naked) void inLookIdHookFunc()
{


	if (Hooks::bLookIds)
	{
		__asm {
			push ebp
			mov ebp, esp
			sub   esp, 0x7C
		}
		int32_t itemId = 0;
		char buf[10];
		_asm {
			mov itemId, eax
		}
		sprintf_s(buf, sizeof(buf), "%i", itemId);
		PacketSend::GetInstance().PrintWhiteText(buf);
		__asm {
			leave
			ret
		}
	}
	else
	{
		__asm
		{
			push ebp
			mov ebp, esp
			leave
			ret
		}
	}
}

__declspec(noinline) int32_t OUTmyPrintText(int32_t v1, int32_t v2, int32_t v3, int32_t v4, int32_t v5, int32_t v6, int32_t v7, const char* v8, int32_t v9)
{

	__asm {
		push v9 //[ebp + 0x20]
		push v8 //[ebp + 0x1C]
		push v7 //[ebp + 0x18]
		push v6 //[ebp + 0x14]
		push v5 //[ebp + 0x10]
		push v4 //[ebp + 0x0C]
		push v3 //[ebp + 0x08]
		push v2
		push v1
		call Hooks::ohkPrintText
	}
}


void Hooks::printMagicWallTimers()
{
	if (!MemReader::GetInstance().IsOnline()) return;
	
	gc = (GameClient2*)(mem::FindDMAAddy2(moduleBase + Offsets::gameClient2, { 0x44, 0x10, 0x1C }));
	CSelfCharacter selfCharacter;
	int32_t red, green, blue = 0;
	char buf[10];
	int32_t magicWallTime;
	int32_t timeNow = timeGetTime();
	double squareDimension;



	for (int32_t i = 0; i < magicWalls.size(); i++)
	{
		if (magicWalls[i].xPos == 0)	continue;

		if ((abs(magicWalls[i].xPos - localPlayer.xPos) <= 7) && (abs(magicWalls[i].yPos - localPlayer.yPos) <= 5) && (magicWalls[i].zPos == localPlayer.zPos))
		{
			if (bMwTimersMilliseconds)
			{
				magicWallTime = ((magicWalls[i].time - timeNow) / 100);
				sprintf_s(buf, sizeof(buf), "%0.1f", float(magicWallTime) / 10);
				magicWallTime /= 10;
			}
			else
			{
				magicWallTime = ((magicWalls[i].time - timeNow + 1000) / 1000);
				sprintf_s(buf, sizeof(buf), "%i", magicWallTime);
			}

			MemReader::GetInstance().ReadSelfCharacter(&selfCharacter);

			squareDimension = (double)(gc->WindowWidth) / 15;

			int32_t MoveX = ((squareDimension / 32) * (abs(selfCharacter.movingX)));
			int32_t MoveY = ((squareDimension / 32) * (abs(selfCharacter.movingY)));
			MoveX = (selfCharacter.movingX > 0) ? -MoveX : MoveX;
			MoveY = (selfCharacter.movingY > 0) ? -MoveY : MoveY;


			int32_t x = gc->WindowXPos + (int32_t)((squareDimension * (8 + -(selfCharacter.xPos - magicWalls[i].xPos + *playerMovingX))) - (squareDimension / 2)) + MoveX;
			int32_t y = gc->WindowYPos + (int32_t)((squareDimension * (6 + -(selfCharacter.yPos - magicWalls[i].yPos + *playerMovingY))) - (squareDimension / 2)) - 5 + MoveY;

			if (bMwTimersStatic)
			{
				red = 245;
				green = 255;
				blue = 0;

				if (magicWallTime < 1)
				{
					if (magicWalls[i].time <= timeNow)
					{
						magicWalls[i].xPos = 0;
						magicWalls[i].yPos = 0;
						magicWalls[i].zPos = 0;
						magicWalls[i].time = 0;
					}
					magicWallTime = 0;
				}

			}
			else
			{
				if (magicWallTime >= 10)
				{
					red = 202;
					green = 202;
					blue = 247;
				}
				else if (magicWallTime >= 5 && magicWallTime < 10)
				{
					red = 247;
					green = 255;
					blue = 0;
				}
				else
				{
					red = 255;
					green = 0;
					blue = 0;
				}
				if (magicWallTime < 1)
				{
					if (magicWalls[i].time <= timeNow)
					{
						magicWalls[i].xPos = 0;
						magicWalls[i].yPos = 0;
						magicWalls[i].zPos = 0;
						magicWalls[i].time = 0;
					}
					magicWallTime = 0;
				}
			}
			OUTmyPrintText(1, x, y, 2, red, green, blue, buf, 2);
		}
		else
		{
			if (magicWalls[i].time <= timeNow)
			{
				magicWalls[i].xPos = 0;
				magicWalls[i].yPos = 0;
				magicWalls[i].zPos = 0;
				magicWalls[i].time = 0;
			}
		}
	}
}





int32_t myPrintText(int32_t nSurface, int32_t nX, int32_t nY, int32_t nFont, int32_t nRed, int32_t nGreen, int32_t nBlue, const char* lpText, int32_t nAlign)
{
	OUTmyPrintText(1, 5, 40, 2, 12, 56, 100, R"(__________)", 0);
	OUTmyPrintText(1, 5, 50, 2, 12, 56, 100, R"(    \_____        \)", 0);
	OUTmyPrintText(1, 5, 60, 2, 12, 56, 100, R"(      |             |     _/)", 0);
	OUTmyPrintText(1, 5, 70, 2, 12, 56, 100, R"(      |             |        \)", 0);
	OUTmyPrintText(1, 5, 80, 2, 12, 56, 100, R"(      | _____      /)", 0);
	OUTmyPrintText(1, 5, 90, 2, 12, 56, 100, R"(                        \ /)", 0);

	if ((Hooks::bMwTimersDynamic || Hooks::bMwTimersStatic))
	{
		Hooks::printMagicWallTimers();
	}
	return OUTmyPrintText(nSurface, nX, nY, nFont, nRed, nGreen, nBlue, lpText, nAlign);
}

__declspec(naked) void INmyPrintText()
{
	__asm {
		push ebp
		mov ebp, esp
		sub esp, 0x10
		push[ebp + 0x28]
		push[ebp + 0x24]
		push[ebp + 0x20]
		push[ebp + 0x1C]
		push[ebp + 0x18]
		push[ebp + 0x14]
		push[ebp + 0x10]
		push[ebp + 0x0C]
		push[ebp + 0x08]
		call myPrintText
		leave
		ret
	}
}

__declspec(naked) void OUTsayHook(int32_t a, const char* message)
{
	__asm {
		push ebp
		mov ebp, esp
		push message
		push a
		call Hooks::hSayHookAssembly
		leave
		ret
	}
}

__declspec(naked) void sayHookFunction(int32_t a, const char* message)
{

	if (Hooks::bComboBot)
	{
		char* sender = nullptr;
		__asm
		{
			sub esp, 0x10
			mov sender, edi
		}


		if (sender)
		{
			if ((strcmp(sender, Hooks::comboBotSpells.leaderName) == 0) && (strcmp(message, Hooks::comboBotSpells.leaderSpell) == 0))
			{
				if (Util::isNotExhausted(Hooks::timeVarComboBot, Cooldowns::GetInstance().COMBO_BOT))
				{
					Hooks::say(1, Hooks::comboBotSpells.mySpell);
				}
			}
		}
		__asm {
			push message
			push a
			call OUTsayHook
			leave
			ret
		}
	}
	else
	{
		__asm
		{
			push ebp
			mov ebp, esp
			push message
			push a
			call OUTsayHook
			leave
			ret
		}
	}
}

__declspec(naked) void INmySayHook()
{
	__asm {
		push ebp
		mov ebp, esp
		push[ebp + 0x0C]
		push[ebp + 0x08]
		call sayHookFunction
		leave
		ret
	}
}

__declspec(noinline) void outPlaceObject(int nX, int nY, int nZ, int nID, int nCount, int u1)
{
	__asm {
		push u1 //[ebp + 0x2C]
		push nCount //[ebp + 0x28]
		push nID //[ebp + 0x24]
		push nZ//[ebp + 0x1C]
		push nY //[ebp + 0x18]
		push nX //[ebp + 0x14]
		call Hooks::hPlaceObject
		add esp, 0x1C
	}
}

void placeObject(int nX, int nY, int nZ, int nID, int nCount, int u1)
{
	MemReader::GetInstance().ReadSelfCharacter(&Hooks::localPlayer);
	int32_t x = -(8 - nX);
	int32_t y = -(6 - nY);

    int32_t z = (Hooks::localPlayer.zPos <= 7) ? Hooks::localPlayer.zPos - (7 - nZ) : -(2 - nZ);

	if (MemReader::GetInstance().IsMagicWall(nCount))
	{
		for (int32_t index = 0; index < Hooks::magicWalls.size(); index++)
		{
			if ((Hooks::magicWalls[index].xPos == Hooks::localPlayer.xPos + x) && (Hooks::magicWalls[index].yPos == Hooks::localPlayer.yPos + y) && (Hooks::magicWalls[index].zPos == Hooks::localPlayer.zPos + z))
			{
				return outPlaceObject(nX, nY, nZ, nID, nCount, u1);
			}
		}
		if (Hooks::localPlayer.zPos <= 7)
		{
			Hooks::magicWalls[Hooks::magicWallCount].zPos = (7 - nZ);
		}
		else
		{
			Hooks::magicWalls[Hooks::magicWallCount].zPos = Hooks::localPlayer.zPos + (2 - nZ);
		}
		Hooks::magicWalls[Hooks::magicWallCount].xPos = Hooks::localPlayer.xPos + x + z;
		Hooks::magicWalls[Hooks::magicWallCount].yPos = Hooks::localPlayer.yPos + y + z;
		if (nCount == 10181 || nCount == 2128 || nCount == 2129)
			Hooks::magicWalls[Hooks::magicWallCount].time = timeGetTime() + 20000;
		else
			Hooks::magicWalls[Hooks::magicWallCount].time = timeGetTime() + 45000;
		Hooks::magicWallCount = (Hooks::magicWallCount < Hooks::magicWalls.size() - 1) ? Hooks::magicWallCount + 1 : 0;
	}
	outPlaceObject( nX, nY, nZ, nID, nCount, u1);
}

__declspec(naked) void inPlaceObject() //(int32_t v1, int32_t v2, int32_t v3, int32_t v4, int32_t v5, int32_t v6, int32_t v7, char* v8<ecx>, int32_t v9)
{
	__asm {
		push ebp
		mov ebp, esp
		push[ebp + 0x1C]
		push[ebp + 0x18]
		push[ebp + 0x14]
		push[ebp + 0x10]
		push[ebp + 0x0C]
		push[ebp + 0x08]
		call placeObject
		leave
		ret
	}
}


void Hooks::InitHooks()
{

	bComboBot = false;
	bLookIds = false;
	moduleBase = (uintptr_t)GetModuleHandle(NULL);

	sayHook = (moduleBase + 0x14AAD0);
	sayHookOriginal = (moduleBase + 0x144220);

	say = (sayFunc)(moduleBase + 0x73F0);

	lookIdHook = (uintptr_t)(moduleBase + 0x7264);
	lookIdOriginal = (uintptr_t)(moduleBase + 0xF8700);

	printText1 = (uintptr_t)(moduleBase + 0xF5823);
	printTextOriginal = (uintptr_t)(moduleBase + 0xB4DD0);

	placeObject = (uintptr_t)(moduleBase + 0xE6CA);
	placeObjectOriginal = (uintptr_t)(moduleBase + 0xE1EF0);


	playerMovingX = (int32_t*)(Offsets::playerMovingX);
	playerMovingY = (int32_t*)(Offsets::playerMovingY);


	mem::DetourCall(placeObject, (uintptr_t)&inPlaceObject, (uintptr_t*)&hPlaceObject);
	mem::DetourCall(lookIdHook, (uintptr_t)&inLookIdHookFunc, (uintptr_t*)&hLookId);
	mem::DetourCall(printText1, (uintptr_t)&INmyPrintText, (uintptr_t*)&ohkPrintText);
	mem::DetourCall(sayHook, (uintptr_t)&INmySayHook, (uintptr_t*)&hSayHookAssembly);
}

void Hooks::UnHook()
{
	mem::DetourCall(printText1, printTextOriginal, 0);
	mem::DetourCall(placeObject, placeObjectOriginal, 0);

	mem::DetourCall(lookIdHook, lookIdOriginal, 0);
	mem::DetourCall(sayHook, sayHookOriginal, 0);
}

