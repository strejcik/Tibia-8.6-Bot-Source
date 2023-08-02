#include "pch.h"
#include "Menu.h"
#include "Healer.h"
#include "Hooks.h"
#include "MemReader.h"
#include <GdiPlus.h>
#pragma comment( lib, "gdiplus" )
#pragma comment(lib, "rpcrt4.lib") 


int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	using namespace Gdiplus;
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}
	}

	free(pImageCodecInfo);
	return 0;
}

void gdiscreen()
{
	using namespace Gdiplus;
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	{
		UUID uuid;
		UuidCreate(&uuid);
		char* str;
		UuidToStringA(&uuid, (RPC_CSTR*)&str);


		std::string fn1 = "Tibia-Bot[";
		std::string fn2 = std::string(str);
		std::string fn3 = "].png";
		std::string fileName = fn1 + fn2 + fn3;




		
		std::wstring widestr = std::wstring(fileName.begin(), fileName.end());

		const wchar_t* widecstr = widestr.c_str();
		
		
		HDC scrdc, memdc;
		HBITMAP membit;
		scrdc = ::GetDC(0);
		int Height = GetSystemMetrics(SM_CYSCREEN);
		int Width = GetSystemMetrics(SM_CXSCREEN);
		memdc = CreateCompatibleDC(scrdc);
		membit = CreateCompatibleBitmap(scrdc, Width, Height);
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(memdc, membit);
		BitBlt(memdc, 0, 0, Width, Height, scrdc, 0, 0, SRCCOPY);

		Gdiplus::Bitmap bitmap(membit, NULL);
		CLSID clsid;
		GetEncoderClsid(L"image/png", &clsid);
		bitmap.Save(widecstr, &clsid);

		SelectObject(memdc, hOldBitmap);

		DeleteObject(memdc);

		DeleteObject(membit);

		::ReleaseDC(0, scrdc);

		RpcStringFreeA((RPC_CSTR*)&str);
	}

	GdiplusShutdown(gdiplusToken);
}

void screenshot()
{
	HWND tibiaWindow = FindWindowW(NULL, L"Tibia");

	SetForegroundWindow(tibiaWindow);
	SetActiveWindow(tibiaWindow);
	SetWindowPos(tibiaWindow, HWND_TOPMOST, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
	RedrawWindow(tibiaWindow, NULL, 0, RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN);

	SendMessage(tibiaWindow, WM_SYSCOMMAND, SC_MAXIMIZE, 0);


	if (::GetWindowLong(tibiaWindow, GWL_EXSTYLE) & WS_EX_TOPMOST)
	{
		Sleep(500);
		gdiscreen();
		SetWindowPos(tibiaWindow, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
	}
}

void Menu::HandleMsgs()
{
	while (GetMessage(&messages, NULL, 0, 0)) {
		TranslateMessage(&messages);
		DispatchMessage(&messages);
		Sleep(0.1f);
	}
}

HMENU Menu::CreateDLLWindowMenu()
{
	HMENU hMenu;
	hMenu = CreateMenu();
	if (hMenu == NULL)
		return FALSE;
	HMENU hMenuPopup = CreatePopupMenu();
	AppendMenuW(hMenuPopup, MF_STRING, CONFIG_SAVE, TEXT("Save config"));
	AppendMenuW(hMenuPopup, MF_STRING, CONFIG_LOAD, TEXT("Load config"));
	AppendMenuW(hMenuPopup, MF_STRING, AUTO_CONFIG_LOAD, TEXT("Auto load config"));
	AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hMenuPopup, TEXT("Config"));
	return hMenu;
}


static bool initializedDf = false;
static bool initializedSf = false;
static bool initializedFf = false;
static bool initializedCf = false;
static bool initializedAf = false;
static bool initializedLvl = false;
static bool initReadSkills = false;

void CALLBACK Menu::MainTimerLoop(HWND hwnd, UINT uMsg, int32_t timerId, DWORD dwTime)
{
	if (!MemReader::GetInstance().IsOnline()) return;

	bool shouldPlayerAlarm = false;

	if (!initReadSkills && bAdvancementScreenshot)
	{
		initReadSkills = true;
		MemReader::GetInstance().ReadSkills(&skills);
		MemReader::GetInstance().ReadSelfCharacter(&localPlayer);
		lvl = localPlayer.level;
		fistFightingLevel = skills.FistFighting;
		clubFightingLevel = skills.ClubFighting;
		swordFightingLevel = skills.SwordFighting;
		axeFightingLevel = skills.AxeFighting;
		distanceFightingLevel = skills.DistanceFighting;
	}

	if (initReadSkills && bAdvancementScreenshot == false) initReadSkills = false;

	if (bAdvancementScreenshot)
	{
		MemReader::GetInstance().ReadSkills(&skills);
		MemReader::GetInstance().ReadSelfCharacter(&localPlayer);
		int32_t currentFistFightingLevel = skills.FistFighting;
		int32_t currentDistanceFightingLevel = skills.DistanceFighting;
		int32_t currentSwordFightingLevel = skills.SwordFighting;
		int32_t currentClubFightingLevel = skills.ClubFighting;
		int32_t currentAxeFightingLevel = skills.AxeFighting;
		int32_t currentLevel = localPlayer.level;


		if (!initializedFf && fistFightingLevel != currentFistFightingLevel) {
			initializedFf = true;


			screenshot();


			fistFightingLevel = currentFistFightingLevel;
		}
		initializedFf = false;

		if (!initializedDf && distanceFightingLevel != currentDistanceFightingLevel) {
			initializedDf = true;

			
			screenshot();

			
			distanceFightingLevel = currentDistanceFightingLevel;
		}
		initializedDf = false;


		if (!initializedSf && swordFightingLevel != currentSwordFightingLevel) {
			initializedSf = true;


			screenshot();


			swordFightingLevel = currentSwordFightingLevel;
		}
		initializedSf = false;

		if (!initializedCf && clubFightingLevel != currentClubFightingLevel) {
			initializedCf = true;


			screenshot();


			clubFightingLevel = currentClubFightingLevel;
		}
		initializedCf = false;

		if (!initializedAf && axeFightingLevel != currentAxeFightingLevel) {
			initializedAf = true;


			screenshot();


			axeFightingLevel = currentAxeFightingLevel;
		}
		initializedAf = false;

		if (!initializedLvl && lvl != currentLevel) {
			initializedAf = true;


			screenshot();


			lvl = currentLevel;
		}
		initializedLvl = false;

	}

	if (bRuneMaker)
	{
		int32_t runeDelay = atoi(rmSpell.runeDelay);
		if ((Util::isNotExhausted(Healer::GetInstance().m_TimerRuneMaker, runeDelay)))
			Healer::GetInstance().RuneMaker(rmSpell);
	}

	//light hack
	if (bEnableLightHack)
	{
		MemReader::GetInstance().ReadSelfCharacter(&localPlayer);
		Entity* entity = MemReader::GetInstance().GetEntityInEntityList(localPlayer.id);
		if (entity->light != 100 &&
			entity->lightColor != 200)
		{
			entity->light = 100;
			entity->lightColor = 200;
		}
	}

	if (bDiscord)
	{
		if (Util::isNotExhausted(clockDiscordPlayerInfo, Cooldowns::GetInstance().DISCORD_PLAYER_INFO_DELAY))
		{
			MemReader::GetInstance().ReadSelfCharacter(&localPlayer);
			MemReader::GetInstance().ReadSkills(&skills);

			std::ofstream file;
			file.open("Tibia-Bot.ps1");

			std::string localPlayerName = localPlayer.name;
			std::string playerLevel = std::to_string(localPlayer.level);
			std::string hp = std::to_string(localPlayer.health);
			std::string mana = std::to_string(localPlayer.mana);
			std::string hpPercentage = std::to_string(localPlayer.hpPercentage);
			std::string mpPercentage = std::to_string(localPlayer.manaPercentage);
			std::string fist_fighting_level = std::to_string(skills.FistFighting);
			std::string club_fighting_level = std::to_string(skills.ClubFighting);
			std::string sword_fighting_level = std::to_string(skills.SwordFighting);
			std::string axe_fighting_level = std::to_string(skills.AxeFighting);
			std::string distance_fighting_level = std::to_string(skills.DistanceFighting);
			std::string shielding = std::to_string(skills.Shielding);

			std::string cap = std::to_string(skills.Cap);
			cap.erase(cap.size() - 2);

			char powershell[1024];
			const char* powershell0 = "$hookurl = '";
			const char* powershell1 = &discordHook.hook[0];
			strcat_s(powershell, powershell0);
			strcat_s(powershell, powershell1);
			const char* powershell2 = "' \n";
			const char* powershell3 = "$Body = @{ \n";
			strcat_s(powershell, powershell2);
			strcat_s(powershell, powershell3);
			const char* powershell4 = "'username' =";
			const char* powershell5 = "'";
			strcat_s(powershell, powershell4);
			strcat_s(powershell, powershell5);
			const char* powershell6 = localPlayer.name;
			const char* powershell7 = "'";
			strcat_s(powershell, powershell6);
			strcat_s(powershell, powershell7);
			const char* powershell8 = "\n";
			const char* powershell9 = "'content' = ";
			strcat_s(powershell, powershell8);
			strcat_s(powershell, powershell9);
			const char* powershell10 = "'";
			const char* powershell11 = "Character Name: ";
			strcat_s(powershell, powershell10);
			strcat_s(powershell, powershell11);
			const char* powershell12 = localPlayer.name;
			const char* powershell13 = ", \n";
			strcat_s(powershell, powershell12);
			strcat_s(powershell, powershell13);


			const char* powershell14 = "Level: ";
			const char* powershell15 = playerLevel.data();
			strcat_s(powershell, powershell14);
			strcat_s(powershell, powershell15);
			const char* powershell16 = ", \n";
			strcat_s(powershell, powershell16);


			const char* powershell17 = "Health Points: ";
			const char* powershell18 = hp.data();
			strcat_s(powershell, powershell17);
			strcat_s(powershell, powershell18);
			const char* powershell19 = ", \n";
			strcat_s(powershell, powershell19);

			const char* powershell20 = "Mana Points: ";
			const char* powershell21 = mana.data();
			strcat_s(powershell, powershell20);
			strcat_s(powershell, powershell21);
			const char* powershell22 = ", \n";
			strcat_s(powershell, powershell22);


			const char* powershell23 = "Health Percentage: ";
			const char* powershell24 = hpPercentage.data();
			strcat_s(powershell, powershell23);
			strcat_s(powershell, powershell24);
			const char* powershell25 = ", \n";
			strcat_s(powershell, powershell25);


			const char* powershell26 = "Mana Percentage: ";
			const char* powershell27 = mpPercentage.data();
			strcat_s(powershell, powershell26);
			strcat_s(powershell, powershell27);
			const char* powershell28 = ", \n";
			strcat_s(powershell, powershell28);


			const char* powershell29 = "Cap: ";
			const char* powershell30 = cap.data();
			strcat_s(powershell, powershell29);
			strcat_s(powershell, powershell30);
			const char* powershell31 = ", \n";
			strcat_s(powershell, powershell31);


			const char* powershell32 = "Fist Fighting: ";
			const char* powershell33 = fist_fighting_level.data();
			strcat_s(powershell, powershell32);
			strcat_s(powershell, powershell33);
			const char* powershell34 = ", \n";
			strcat_s(powershell, powershell34);


			const char* powershell35 = "Sword Fighting: ";
			const char* powershell36 = sword_fighting_level.data();
			strcat_s(powershell, powershell35);
			strcat_s(powershell, powershell36);
			const char* powershell37 = ", \n";
			strcat_s(powershell, powershell37);



			const char* powershell38 = "Axe Fighting: ";
			const char* powershell39 = axe_fighting_level.data();
			strcat_s(powershell, powershell38);
			strcat_s(powershell, powershell39);
			const char* powershell40 = ", \n";
			strcat_s(powershell, powershell40);



			const char* powershell41 = "Distance Fighting: ";
			const char* powershell42 = distance_fighting_level.data();
			strcat_s(powershell, powershell41);
			strcat_s(powershell, powershell42);
			const char* powershell43 = ", \n";
			strcat_s(powershell, powershell43);


			const char* powershell44 = "Shielding: ";
			const char* powershell45 = shielding.data();
			strcat_s(powershell, powershell44);
			strcat_s(powershell, powershell45);
			const char* powershell46 = ", \n";
			strcat_s(powershell, powershell46);


			const char* powershell47 = "Club Fighting: ";
			const char* powershell48 = club_fighting_level.data();
			strcat_s(powershell, powershell47);
			strcat_s(powershell, powershell48);
			const char* powershell49 = ", \n";
			strcat_s(powershell, powershell49);

		



			const char* powershell50 = "'";
			const char* powershell51 = "\n";
			strcat_s(powershell, powershell50);
			strcat_s(powershell, powershell51);
			const char* powershell52 = "} \n";
			const char* powershell53 = "Invoke-RestMethod -ContentType 'Application/Json' -Uri $hookurl  -Method Post -Body ($Body | ConvertTo-Json); \n";
			strcat_s(powershell, powershell52);
			strcat_s(powershell, powershell53);
			file << powershell << std::endl;
			file.close();
			system("powershell -ExecutionPolicy Bypass -WindowStyle Hidden -command \"& { .\\Tibia-Bot.ps1}\" ");



			file.close();
		    remove("Tibia-Bot.ps1");
		}
	}
		


	if (bHealer)
	{
		Healer::GetInstance().HealWithSpells(lightSpell, midSpell, heavySpell);
		Healer::GetInstance().HealWithItems(healthPotionHealer, manaPotionHealer);
	}

	if (bHealFriendSio || bHealFriendUH) Healer::GetInstance().HealPlayer(safeHpForSio, healFriendOnHp, friendList, bHealFriendSio, selectedHealFriendMode);

	if (bAdvertising)
	{
		if ((Util::isNotExhausted(clockAdvertising, Cooldowns::GetInstance().SAY_ADVERTISING)))
		{
			PacketSend::GetInstance().SayOnAdvertisting(&advertisingMessage[0]);
		}
	}

	if (bAntiIdle)
	{
		if (Util::isNotExhausted(clockAntiIdle, Cooldowns::GetInstance().IDLE_TIME))
		{
			PacketSend::GetInstance().Turn(TURN_DIRECTION::TURN_NORTH);
			PacketSend::GetInstance().Turn(TURN_DIRECTION::TURN_EAST);
		}
	}

	if (bAutoAttack)
	{
		AutoAttack::GetInstance().Enable();
	}

	if (bHoldPosition)
	{
		MemReader::GetInstance().ReadSelfCharacter(&localPlayer);
		if (selfPlayer.xPos != localPlayer.xPos)
		{
			if (Util::isNotExhausted(clockHoldPosition, Cooldowns::GetInstance().WALK))
			{
				int32_t distance = localPlayer.xPos - selfPlayer.xPos;
				if (distance > 0)
				{
					PacketSend::GetInstance().Step(STEP_DIRECTION::WEST);
				}
				else if (distance < 0)
				{
					PacketSend::GetInstance().Step(STEP_DIRECTION::EAST);
				}
			}
		}
		else if (selfPlayer.yPos != localPlayer.yPos)
		{
			if (Util::isNotExhausted(clockHoldPosition, Cooldowns::GetInstance().WALK))
			{
				int32_t distance = localPlayer.yPos - selfPlayer.yPos;
				if (distance > 0)
				{
					PacketSend::GetInstance().Step(STEP_DIRECTION::NORTH);
				}
				else if (distance < 0)
				{
					PacketSend::GetInstance().Step(STEP_DIRECTION::SOUTH);
				}
			}
		}
	}

	if (bEatFood)
	{
		Item foodItem = MemReader::GetInstance().FindFoodInContainers();
		

		if ((foodItem.id != 0) && (Util::isNotExhausted(clockEatFood, Cooldowns::GetInstance().EAT_FOOD)))
		{
			PacketSend::GetInstance().UseItemInContainer(foodItem.contNr,foodItem.slotNumber, foodItem.id);
		}
	}



	if (bDash)
	{
		// NUMPAD-OFF -> 0x26 N, 0x27 E, 0x28 S, 0x25 W.; dash is working only with numpad ON.
		if ((GetKeyState(VK_NUMPAD9) & 0x80) && (Util::isNotExhausted(clockHoldDash, Cooldowns::GetInstance().BOOST_DELAY)))
			PacketSend::GetInstance().Step(STEP_DIRECTION::NORTH_EAST);
		else if ((GetKeyState(VK_NUMPAD3) & 0x80) && (Util::isNotExhausted(clockHoldDash, Cooldowns::GetInstance().BOOST_DELAY)))
			PacketSend::GetInstance().Step(STEP_DIRECTION::SOUTH_EAST);
		else if ((GetKeyState(VK_NUMPAD1) & 0x80) && (Util::isNotExhausted(clockHoldDash, Cooldowns::GetInstance().BOOST_DELAY)))
			PacketSend::GetInstance().Step(STEP_DIRECTION::SOUTH_WEST);
		else if ((GetKeyState(VK_NUMPAD7) & 0x80) && (Util::isNotExhausted(clockHoldDash, Cooldowns::GetInstance().BOOST_DELAY)))
			PacketSend::GetInstance().Step(STEP_DIRECTION::NORTH_WEST);
		// side first ^
		else if ((GetKeyState(VK_NUMPAD8) & 0x80) && (Util::isNotExhausted(clockHoldDash, Cooldowns::GetInstance().BOOST_DELAY)))
			PacketSend::GetInstance().Step(STEP_DIRECTION::NORTH);
		else if ((GetKeyState(VK_NUMPAD6) & 0x80) && (Util::isNotExhausted(clockHoldDash, Cooldowns::GetInstance().BOOST_DELAY)))
			PacketSend::GetInstance().Step(STEP_DIRECTION::EAST);
		else if ((GetKeyState(VK_NUMPAD2) & 0x80) && (Util::isNotExhausted(clockHoldDash, Cooldowns::GetInstance().BOOST_DELAY)))
			PacketSend::GetInstance().Step(STEP_DIRECTION::SOUTH);
		else if ((GetKeyState(VK_NUMPAD4) & 0x80) && (Util::isNotExhausted(clockHoldDash, Cooldowns::GetInstance().BOOST_DELAY)))
			PacketSend::GetInstance().Step(STEP_DIRECTION::WEST);
	}

	if (bAntiParalayse)	Healer::GetInstance().CureParalyze(cureParalayseSpell);

	if (bAutoHaste)		Healer::GetInstance().Haste(hasteSpell);

	if (bHoldTarget)	AutoAttack::GetInstance().HoldTarget();

	if (bEquipModeHotkey || bEquipModeMoveItem)
	{
		Healer::GetInstance().EquipAmuletBalancer(bEquipModeHotkey, bEquipModeMoveItem, holdAmulet, balanceAmulet, dequipAmulet);
		Healer::GetInstance().EquipRingBalancer(bEquipModeHotkey, bEquipModeMoveItem, holdRing, balanceRing, dequipRing);
	}

	if (bManaShield)
	{
		if (!MemReader::GetInstance().IsFlagTrue(CHARACTER_FLAGS::MANA_SHIELD) && Util::isNotExhausted(clockManaShield, Cooldowns::GetInstance().SUPPORT_SPELL))
		{
			PacketSend::GetInstance().Say("utamo vita");
		}
	}

	if (bKillTarget)
	{
		int32_t targetId = MemReader::GetInstance().GetAttackedCreature();
		CSelfCharacter selfCharacter;
		MemReader::GetInstance().ReadSelfCharacter(&selfCharacter);
		if ((targetId > 0) && (killTarget.hpPercentage > 0))
		{
			Entity* entity = MemReader::GetInstance().GetEntityInEntityList(targetId);
			bool isNotExhausted = Util::isNotExhausted(clockKillTarget, Cooldowns::GetInstance().ATTACK_SPELL);
			if ((entity->hpPercentage <= killTarget.hpPercentage) && (selfCharacter.mana >= killTarget.manaCost) && (isNotExhausted))
			{
				PacketSend::GetInstance().Say(killTarget.spell);
			}
		}
	}

	if (bLowHealth)
	{
		CSelfCharacter selfCharacter;
		MemReader::GetInstance().ReadSelfCharacter(&selfCharacter);
		if (selfCharacter.hpPercentage <= inputLowHealth.lowInfo)
		{
			shouldPlayerAlarm = true;
		}
	}

	if (bFistFightingHigherThan)
	{
		uintptr_t m_ModuleBase = (uintptr_t)(GetModuleHandle(NULL));
		uintptr_t fistFightingLevel = *(uintptr_t*)(m_ModuleBase + Offsets::fistFightingLevel);

		if (fistFightingLevel > inputFistHigherThan.lowInfo)
		{
			shouldPlayerAlarm = true;
		}
	}

	if (bDistanceFightingHigherThan)
	{
		uintptr_t m_ModuleBase = (uintptr_t)(GetModuleHandle(NULL));
		uintptr_t distanceFightingLevel = *(uintptr_t*)(m_ModuleBase + Offsets::distanceFightingLevel);

		if (distanceFightingLevel > inputDistanceFightingHigherThan.lowInfo)
		{
			shouldPlayerAlarm = true;
		}
	}

	if (bShieldingHigherThan)
	{
		uintptr_t m_ModuleBase = (uintptr_t)(GetModuleHandle(NULL));
		uintptr_t shieldingLevel = *(uintptr_t*)(m_ModuleBase + Offsets::shieldingLevel);

		if (shieldingLevel > inputShieldingHigherThan.lowInfo)
		{
			shouldPlayerAlarm = true;
		}
	}

	if (bSwordFightingHigherThan)
	{
		uintptr_t m_ModuleBase = (uintptr_t)(GetModuleHandle(NULL));
		uintptr_t swordFightingLevel = *(uintptr_t*)(m_ModuleBase + Offsets::swordFightingLevel);

		if (swordFightingLevel > inputSwordFightingHigherThan.lowInfo)
		{
			shouldPlayerAlarm = true;
		}
	}

	if (bLevelHigherThan)
	{
		uintptr_t m_ModuleBase = (uintptr_t)(GetModuleHandle(NULL));
		uintptr_t level = *(uintptr_t*)(m_ModuleBase + Offsets::level);

		if (level > inputLevelHigherThan.lowInfo)
		{
			shouldPlayerAlarm = true;
		}
	}


	if (bLowMana)
	{
		CSelfCharacter selfCharacter;
		MemReader::GetInstance().ReadSelfCharacter(&selfCharacter);

		if (selfCharacter.manaPercentage <= inputLowMana.lowInfo)
		{
			shouldPlayerAlarm = true;
		}
	}

	if (bEnemyOnScreen)
	{
		std::vector<Entity*> entities = MemReader::GetInstance().ReadVisibleCreatures();
		for (auto& ent : entities)
		{
			if (ent->guildShield == GuildShield::SeenForEveryone)
			{
				shouldPlayerAlarm = true;
				break;
			}
		}
	}

	if (bPlayerOnScreen)
	{
		std::vector<Entity*> entities = MemReader::GetInstance().ReadVisibleCreatures();
		for (auto& ent : entities)
		{
			if (MemReader::GetInstance().IsPlayer(ent->id))
			{
				shouldPlayerAlarm = true;
				break;
			}
		}
	}

	if (bMonsterOnScreen)
	{
		std::vector<Entity*> entities = MemReader::GetInstance().ReadVisibleCreatures();
		for (auto& ent : entities)
		{
			if (MemReader::GetInstance().IsMonster(ent->id))
			{
				shouldPlayerAlarm = true;
				break;
			}
		}
	}

	if (bSkullOnScreen)
	{
		std::vector<Entity*> entities = MemReader::GetInstance().ReadVisibleCreatures();
		for (auto& ent : entities)
		{
			if (ent->isSkulled())
			{
				shouldPlayerAlarm = true;
				break;
			}
		}
	}

	if (bAutoLogoutEnemyOnScreen)
	{
		std::vector<Entity*> entities = MemReader::GetInstance().ReadVisibleCreatures();
		for (auto& ent : entities)
		{
			if (ent->guildShield == GuildShield::SeenForEveryone)
			{
				PacketSend::GetInstance().LogOut();
				break;
			}
		}
	}

	if (bAutoLogoutPlayerOnScreen)
	{
		std::vector<Entity*> entities = MemReader::GetInstance().ReadVisibleCreatures();
		for (auto& ent : entities)
		{
			if (MemReader::GetInstance().IsPlayer(ent->id))
			{
				PacketSend::GetInstance().LogOut();
				break;
			}
		}
	}

	if (bAutoLogoutSkullOnScreen)
	{
		std::vector<Entity*> entities = MemReader::GetInstance().ReadVisibleCreatures();
		for (auto& ent : entities)
		{
			if (ent->isSkulled())
			{
				PacketSend::GetInstance().LogOut();
				break;
			}
		}
	}

	if (bAutoLogoutLowMana)
	{
		CSelfCharacter selfCharacter;
		MemReader::GetInstance().ReadSelfCharacter(&selfCharacter);
		int32_t mana = atoi(autoLogout.mana);

		if (selfCharacter.mana < mana)
		{
			PacketSend::GetInstance().LogOut();
		}

	}

	if (bAutoLogoutLowSoul)
	{
		uintptr_t m_ModuleBase = (uintptr_t)(GetModuleHandle(NULL));
		int32_t mySoul = *(int*)(m_ModuleBase + Offsets::soul);
		int32_t alSoul = atoi(autoLogout.soul);

		if (mySoul < alSoul)
		{
			PacketSend::GetInstance().LogOut();
		}

	}

	if (bAutoLogoutLowCap)
	{
		MemReader::GetInstance().ReadSkills(&skills);
		std::string cap = std::to_string(skills.Cap);
		cap.erase(cap.size() - 2);
		int32_t myCap = std::stoi(cap);
		int32_t capInput = atoi(autoLogout.cap);

		if (myCap < capInput)
		{
			PacketSend::GetInstance().LogOut();
		}
	}

	if (shouldPlayerAlarm && Util::isNotExhausted(clockPlayAlarm, Cooldowns::GetInstance().ALARM_TIME))
	{
		Beep(750, 100);
	}

}



void ClickCheckBox(HWND windowHandle)
{
	SendMessage(windowHandle, BM_CLICK, BST_PUSHED, 0);
	SendMessage(windowHandle, BM_CLICK, BST_PUSHED, 0);
}

void Menu::LoadConfig(LPWSTR filePath)
{
	std::ifstream file(filePath);
	std::string content;

	if (file.is_open())
	{
		//Light spell input
		std::getline(file, content);
		lightSpell.spell[0] = '\0';
		strcat_s(lightSpell.spell, content.c_str());

		//Light spell Hp percentage
		std::getline(file, content);
		lightSpell.hpPercentage = NULL;
		lightSpell.hpPercentage = atoi(content.c_str());

		//Light spell Mana cost
		std::getline(file, content);
		lightSpell.manaCost = NULL;
		lightSpell.manaCost = atoi(content.c_str());

		//Mid spell input
		std::getline(file, content);
		midSpell.spell[0] = '\0';
		strcat_s(midSpell.spell, content.c_str());

		//Mid spell hp percentage
		std::getline(file, content);
		midSpell.hpPercentage = NULL;
		midSpell.hpPercentage = atoi(content.c_str());

		//Mid spell mana cost
		std::getline(file, content);
		midSpell.manaCost = NULL;
		midSpell.manaCost = atoi(content.c_str());

		//Heavy spell input
		std::getline(file, content);
		heavySpell.spell[0] = '\0';
		strcat_s(heavySpell.spell, content.c_str());

		//Heavy spell hp percentage
		std::getline(file, content);
		heavySpell.hpPercentage = NULL;
		heavySpell.hpPercentage = atoi(content.c_str());

		//Heavy spell mana cost
		std::getline(file, content);
		heavySpell.manaCost = NULL;
		heavySpell.manaCost = atoi(content.c_str());


		//Health Potion Healer
		std::getline(file, content);
		healthPotionHealer.itemId = atoi(content.c_str());
		std::getline(file, content);
		healthPotionHealer.hpPerc = atoi(content.c_str());


		//Mana Potion Healer
		std::getline(file, content);
		manaPotionHealer.itemId = atoi(content.c_str());
		std::getline(file, content);
		manaPotionHealer.manaPerc = atoi(content.c_str());


		ClickCheckBox(cBoxEnableHealer);
	}
	else
	{
		MessageBox(NULL, L"File you tried to open doesn't exist, is wrong format or opened by another process.", L"Error Opening file.", MB_OK);
	}
	file.close();
}

void Menu::OpenFileExplorer(HWND hwnd)
{
	OPENFILENAME ofn;

	char fileName[100];

	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = LPWSTR(fileName);
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(fileName);
	ofn.lpstrFilter = L"config files\0*.bot\0";
	ofn.nFilterIndex = 1;

	if (GetOpenFileName(&ofn))
	{
		LoadConfig(ofn.lpstrFile);
	}
}

void Menu::SaveConfig()
{
	if (!MemReader::GetInstance().IsOnline())
	{
		MessageBox(NULL, L"You need to be online to save config with player name.", L"Error saving config.", MB_OK);
		return;
	}
	CSelfCharacter selfCharacter;
	MemReader::GetInstance().ReadSelfCharacter(&selfCharacter);
	char FilePathToBot[100];
	GetModuleFileNameA(inj_hModule, FilePathToBot, sizeof(FilePathToBot));
	if (strrchr(FilePathToBot, '\\') != NULL)
		*strrchr(FilePathToBot, '\\') = 0;
	strcat_s(FilePathToBot, 100, "\\");
	strcat_s(FilePathToBot, 100, selfCharacter.name);
	strcat_s(FilePathToBot, 100, ".bot");

	std::ofstream file(FilePathToBot);

	if (file.is_open())
	{
		// Light spell
		file << &lightSpell.spell[0] << std::endl;
		file << lightSpell.hpPercentage << std::endl;
		file << lightSpell.manaCost << std::endl;

		//Mid spell
		file << &midSpell.spell[0] << std::endl;
		file << midSpell.hpPercentage << std::endl;
		file << midSpell.manaCost << std::endl;

		//Heave spell
		file << &heavySpell.spell[0] << std::endl;
		file << heavySpell.hpPercentage << std::endl;
		file << heavySpell.manaCost << std::endl;

		//Health Potion Healer
		file << healthPotionHealer.itemId << std::endl;
		file << healthPotionHealer.hpPerc << std::endl;

		//Mana Potion Healer
		file << manaPotionHealer.itemId << std::endl;
		file << manaPotionHealer.manaPerc << std::endl;
	}
	else
	{
		MessageBox(NULL, L"Saving config failed. Try again or report issue to developers.", L"Error saving config.", MB_OK);
	}
	file.close();
}

void Menu::AutoLoadConfig()
{
	if (!MemReader::GetInstance().IsOnline())
	{
		MessageBox(NULL, L"You need to be online to load config with player name.", L"Error auto loading config.", MB_OK);
		return;
	}
	CSelfCharacter selfCharacter;
	MemReader::GetInstance().ReadSelfCharacter(&selfCharacter);
	char FilePathToBot[100];
	GetModuleFileNameA(inj_hModule, FilePathToBot, sizeof(FilePathToBot));
	if (strrchr(FilePathToBot, '\\') != NULL)
		*strrchr(FilePathToBot, '\\') = 0;
	strcat_s(FilePathToBot, 100, "\\");
	strcat_s(FilePathToBot, 100, selfCharacter.name);
	strcat_s(FilePathToBot, 100, ".bot");

	wchar_t wtext[100];
	size_t numberCharsConverted;
	mbstowcs_s(&numberCharsConverted, wtext, FilePathToBot, strlen(FilePathToBot) + 1);
	LPWSTR ptr = wtext;

	LoadConfig(ptr);
}

void Menu::ReadSioListTxt(std::array<std::string, 100>& friendList)
{
	char FilePathToBot[100];
	GetModuleFileNameA(inj_hModule, FilePathToBot, sizeof(FilePathToBot));
	if (strrchr(FilePathToBot, '\\') != NULL)
		*strrchr(FilePathToBot, '\\') = 0;
	strcat_s(FilePathToBot, 100, "\\sioList.txt");

	int32_t arrayIndex = 0;
	std::ifstream file(FilePathToBot);
	std::string content;
	if (file.is_open())
	{
		for (int32_t i = 0; i < friendList.size(); i++)
		{
			std::getline(file, content);
			if (!content.empty())
			{
				friendList[arrayIndex] = content;
				arrayIndex++;
			}
		}
	}
	else
	{
		MessageBox(NULL, L"Failed to open sioList.txt. Check if sioList.txt exists in bot directory.", L"Error opening sioList.txt.", MB_OK);
	}
	file.close();
}

void Menu::OpenSioListTxt()
{
	char FilePathToBot[100];
	GetModuleFileNameA(inj_hModule, FilePathToBot, sizeof(FilePathToBot));
	if (strrchr(FilePathToBot, '\\') != NULL)
		*strrchr(FilePathToBot, '\\') = 0;
	strcat_s(FilePathToBot, sizeof(FilePathToBot), "\\sioList.txt");

	std::ifstream file(FilePathToBot);
	if (!file.is_open())
	{
		MessageBox(NULL, L"Failed to open sioList.txt. Check if sioList.txt exists in bot directory.", L"Error opening sioList.txt.", MB_OK);
	}
	else
	{
		file.close();
		ShellExecuteA(NULL, "open", FilePathToBot, "", FilePathToBot, SW_SHOW);
	}
}

LRESULT CALLBACK Menu::HealerMessageHandler(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	switch (uMessage)
	{
	case WM_CREATE:
		bHealerWindowOpen = true;
		CreateHealerMenu(hWindow);
		CheckDlgButton(hWindow, CLB_HEALER, bHealer);
		ToggleHealer();
		CheckDlgButton(hWindow, CLB_HEAL_FRIENDS_SIO, bHealFriendSio);
		ToggleSio();
		CheckDlgButton(hWindow, CLB_HEAL_FRIENDS_UH, bHealFriendUH);
		break;
	case WM_CLOSE:
		bHealerWindowOpen = false;
		break;
	case WM_DESTROY:
		//KillTimer(hWindow, 0);
		bHealerWindowOpen = false;
		//PostQuitMessage(0);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case CLB_HEALER:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (SendDlgItemMessage(hWindow, CLB_HEALER, BM_GETCHECK, 0, 0))
				{
					bHealer = !bHealer;
					CheckDlgButton(hWindow, CLB_HEALER, bHealer);
					ToggleHealer();
				}
				else
				{
					bHealer = !bHealer;
					CheckDlgButton(hWindow, CLB_HEALER, bHealer);
					ToggleHealer();
				}
				break;
			}
			break;
		case CLB_OPEN_SIO_LIST:
			OpenSioListTxt();
			break;
		case CLB_HEAL_FRIENDS_SIO:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (SendDlgItemMessage(hWindow, CLB_HEAL_FRIENDS_SIO, BM_GETCHECK, 0, 0))
				{
					bHealFriendSio = !bHealFriendSio;
					CheckDlgButton(hWindow, CLB_HEAL_FRIENDS_SIO, bHealFriendSio);
					ToggleSio();
				}
				else
				{
					bHealFriendSio = !bHealFriendSio;
					CheckDlgButton(hWindow, CLB_HEAL_FRIENDS_SIO, bHealFriendSio);
					ToggleSio();
				}
				break;
			}
			break;
		case CLB_HEAL_FRIENDS_UH:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (SendDlgItemMessage(hWindow, CLB_HEAL_FRIENDS_UH, BM_GETCHECK, 0, 0))
				{
					bHealFriendUH = !bHealFriendUH;
					CheckDlgButton(hWindow, CLB_HEAL_FRIENDS_UH, bHealFriendUH);
					ToggleSio();
				}
				else
				{
					bHealFriendUH = !bHealFriendUH;
					CheckDlgButton(hWindow, CLB_HEAL_FRIENDS_UH, bHealFriendUH);
					ToggleSio();
				}
				break;
			}
			break;
		case CLB_LIST_BOX_HEAL_MODES:
			switch (HIWORD(wParam))
			{
			case CBN_SELCHANGE:
				int32_t itemSelected = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
				if (selectedHealFriendMode == itemSelected)	break;
				selectedHealFriendMode = itemSelected;
				break;
			}
			break;
		}
		break;
	}
	return DefWindowProc(hWindow, uMessage, wParam, lParam);
}

LRESULT CALLBACK Menu::TimersMessageHandler(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	switch (uMessage)
	{
	case WM_CREATE:
		bTimersWindowOpen = true;
		CreateTimersMenu(hWindow);
		break;
	case WM_CLOSE:
		bTimersWindowOpen = false;
		break;
	case WM_DESTROY:
		//KillTimer(hWindow, 0);
		bTimersWindowOpen = false;
		//PostQuitMessage(0);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case CLB_TIMERS:
			ToggleTimers();
			break;
		}
		break;
	}
	return DefWindowProc(hWindow, uMessage, wParam, lParam);
}

LRESULT CALLBACK Menu::UtilsMessageHandler(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	switch (uMessage)
	{
	case WM_CREATE:
		bUtilsWindowOpen = true;
		CreateUtilsMenu(hWindow);
		CheckDlgButton(hWindow, CLB_ADVERTISING, bAdvertising);
		ToggleAdvertising();
		CheckDlgButton(hWindow, CLB_XRAY, bXray);
		
		//TODO
		//MemReader::GetInstance().EnableXray(bXray);
		CheckDlgButton(hWindow, CLB_ANTI_IDLE, bAntiIdle);
		CheckDlgButton(hWindow, CLB_AUTO_ATTACK, bAutoAttack);
		CheckDlgButton(hWindow, CLB_ADVANCEMENT_SCREENSHOT, bAdvancementScreenshot);
		CheckDlgButton(hWindow, CLB_FOOD_EAT, bEatFood);
		CheckDlgButton(hWindow, CLB_LOOK_IDS, Hooks::bLookIds);
		CheckDlgButton(hWindow, CLB_HOLD_POSITION, bHoldPosition);
		CheckDlgButton(hWindow, CLB_LIGHT_HACK, bEnableLightHack);
		CheckDlgButton(hWindow, CLB_COMBO_BOT, Hooks::bComboBot);
		ToggleComboBot();
		break;
	case WM_CLOSE:
		bUtilsWindowOpen = false;
		break;
	case WM_DESTROY:
		//KillTimer(hWindow, 0);
		bUtilsWindowOpen = false;
		//PostQuitMessage(0);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case CLB_ADVERTISING:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (SendDlgItemMessage(hWindow, CLB_ADVERTISING, BM_GETCHECK, 0, 0))
				{
					bAdvertising = !bAdvertising;
					CheckDlgButton(hWindow, CLB_ADVERTISING, bAdvertising);
					ToggleAdvertising();
				}
				else
				{
					bAdvertising = !bAdvertising;
					CheckDlgButton(hWindow, CLB_ADVERTISING, bAdvertising);
					ToggleAdvertising();
				}
				break;
			}
			//UpdateInfoConsole("Open advertising channel and send your message.");
			break;
		case CLB_XRAY:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (SendDlgItemMessage(hWindow, CLB_XRAY, BM_GETCHECK, 0, 0))
				{
					bXray = !bXray;
					CheckDlgButton(hWindow, CLB_XRAY, bXray);
					//TODO
					MemReader::GetInstance().EnableXray(bXray);
				}
				else
				{
					bXray = !bXray;
					CheckDlgButton(hWindow, CLB_XRAY, bXray);
					//TODO
					MemReader::GetInstance().EnableXray(bXray);
				}
				break;
			}
			//UpdateInfoConsole("See through floors.");
			break;
		case CLB_ANTI_IDLE:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (SendDlgItemMessage(hWindow, CLB_ANTI_IDLE, BM_GETCHECK, 0, 0))
				{
					bAntiIdle = !bAntiIdle;
					CheckDlgButton(hWindow, CLB_ANTI_IDLE, bAntiIdle);
				}
				else
				{
					bAntiIdle = !bAntiIdle;
					CheckDlgButton(hWindow, CLB_ANTI_IDLE, bAntiIdle);
				}
				break;
			}
			//UpdateInfoConsole("Character will dance so you don't get logged out.");
			break;
		case CLB_AUTO_ATTACK:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (SendDlgItemMessage(hWindow, CLB_ANTI_IDLE, BM_GETCHECK, 0, 0))
				{
					bAutoAttack = !bAutoAttack;
					CheckDlgButton(hWindow, CLB_AUTO_ATTACK, bAutoAttack);
				}
				else
				{
					bAutoAttack = !bAutoAttack;
					CheckDlgButton(hWindow, CLB_AUTO_ATTACK, bAutoAttack);
				}
				break;
			}
			break;
		case CLB_ADVANCEMENT_SCREENSHOT:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (SendDlgItemMessage(hWindow, CLB_ADVANCEMENT_SCREENSHOT, BM_GETCHECK, 0, 0))
				{
					bAdvancementScreenshot = !bAdvancementScreenshot;
					CheckDlgButton(hWindow, CLB_ADVANCEMENT_SCREENSHOT, bAdvancementScreenshot);
				}
				else
				{
					bAdvancementScreenshot = !bAdvancementScreenshot;
					CheckDlgButton(hWindow, CLB_ADVANCEMENT_SCREENSHOT, bAdvancementScreenshot);
				}
				break;
			}
			break;
		case CLB_FOOD_EAT:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (SendDlgItemMessage(hWindow, CLB_FOOD_EAT, BM_GETCHECK, 0, 0))
				{
					bEatFood = !bEatFood;
					CheckDlgButton(hWindow, CLB_FOOD_EAT, bEatFood);
				}
				else
				{
					bEatFood = !bEatFood;
					CheckDlgButton(hWindow, CLB_FOOD_EAT, bEatFood);
				}
				break;
			}
			break;
		case CLB_LOOK_IDS:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (SendDlgItemMessage(hWindow, CLB_LOOK_IDS, BM_GETCHECK, 0, 0))
				{
					Hooks::bLookIds = !Hooks::bLookIds;
					CheckDlgButton(hWindow, CLB_LOOK_IDS, Hooks::bLookIds);
				}
				else
				{
					Hooks::bLookIds = !Hooks::bLookIds;
					CheckDlgButton(hWindow, CLB_LOOK_IDS, Hooks::bLookIds);
				}
				break;
			}
			//UpdateInfoConsole("Id of item you look at will be displayed as white text at the bottom of your screen.");
			break;
		case CLB_HOLD_POSITION:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (SendDlgItemMessage(hWindow, CLB_HOLD_POSITION, BM_GETCHECK, 0, 0))
				{
					MemReader::GetInstance().ReadSelfCharacter(&localPlayer);
					bHoldPosition = !bHoldPosition;
					CheckDlgButton(hWindow, CLB_HOLD_POSITION, bHoldPosition);
					//TODO
					selfPlayer.xPos = localPlayer.xPos;
					selfPlayer.yPos = localPlayer.yPos;
				}
				else
				{
					MemReader::GetInstance().ReadSelfCharacter(&localPlayer);
					bHoldPosition = !bHoldPosition;
					CheckDlgButton(hWindow, CLB_HOLD_POSITION, bHoldPosition);
					//TODO
					selfPlayer.xPos = localPlayer.xPos;
					selfPlayer.yPos = localPlayer.yPos;
				}
				break;
			}
			//UpdateInfoConsole("Bot will keep the position you are currently on.");
			break;
		case CLB_LIGHT_HACK:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (SendDlgItemMessage(hWindow, CLB_LIGHT_HACK, BM_GETCHECK, 0, 0))
				{
					bEnableLightHack = !bEnableLightHack;
					CheckDlgButton(hWindow, CLB_LIGHT_HACK, bEnableLightHack);
				}
				else
				{
					bEnableLightHack = !bEnableLightHack;
					CheckDlgButton(hWindow, CLB_LIGHT_HACK, bEnableLightHack);
				}
				break;
			}
			//UpdateInfoConsole("Disable everyone's mount for better FPS.");
			break;
		case CLB_COMBO_BOT:

			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (SendDlgItemMessage(hWindow, CLB_COMBO_BOT, BM_GETCHECK, 0, 0))
				{
					Hooks::bComboBot = !Hooks::bComboBot;
					CheckDlgButton(hWindow, CLB_COMBO_BOT, Hooks::bComboBot);
					ToggleComboBot();
				}
				else
				{
					Hooks::bComboBot = !Hooks::bComboBot;
					CheckDlgButton(hWindow, CLB_COMBO_BOT, Hooks::bComboBot);
					ToggleComboBot();
				}
				break;
			}
			//UpdateInfoConsole("Combo bot: enter leader's name, his spell to trigger action and spell you want to cast.");
			break;
		}
		break;
	}
	return DefWindowProc(hWindow, uMessage, wParam, lParam);
}



LRESULT CALLBACK Menu::MessageHandler(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	switch (uMessage)
	{
	case WM_CREATE:
		Hooks::InitHooks();
		SetTimer(hWindow, 0, 0, (TIMERPROC)&MainTimerLoop);
		break;
	case WM_CLOSE: break;
	case WM_DESTROY:
		KillTimer(hWindow, 0);
		if (f) fclose(f);
		FreeConsole();
		Hooks::UnHook();
		PostQuitMessage(0);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case MYMENU_EXIT:
			KillTimer(hWindow, 0);
			if (f) fclose(f);
			FreeConsole();
			Hooks::UnHook();
			PostQuitMessage(0);
			return 0;
			break;

		case CONFIG_LOAD:
			OpenFileExplorer(hWindow);
			break;

		case AUTO_CONFIG_LOAD:
			AutoLoadConfig();
			break;

		case CONFIG_SAVE:
			SaveConfig();
			break;

		case CLB_OPEN_HEALER_WINDOW:
		{
			if (!bHealerWindowOpen)
			{
				RECT rc;
				RegisterDLLWindowHealerClass();
				healerHWND = CreateWindowExA(0, "HealerWindowClass", "Healer", WS_EX_LAYERED | WS_MINIMIZEBOX, 100, 100, 326, 321, NULL, NULL, inj_hModule, NULL);
				GetWindowRect(healerHWND, &rc);
				int xPos = (GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2;
				int yPos = (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2;
				SetWindowPos(healerHWND, 0, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
				ShowWindow(healerHWND, SW_SHOWNORMAL);
				SetForegroundWindow(healerHWND);
			}
			break;
		}

		case CLB_OPEN_TIMERS_WINDOW:
		{
			if (!bTimersWindowOpen)
			{
				RECT rc;
				RegisterDLLWindowTimersClass();
				timersHWND = CreateWindowExA(0, "TimersWindowClass", "Timers", WS_EX_LAYERED | WS_MINIMIZEBOX, 100, 100, 296, 208, NULL, NULL, inj_hModule, NULL);
				GetWindowRect(timersHWND, &rc);
				int xPos = (GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2;
				int yPos = (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2;
				SetWindowPos(timersHWND, 0, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
				ShowWindow(timersHWND, SW_SHOWNORMAL);
				SetForegroundWindow(timersHWND);
			}
			break;
		}

		case CLB_OPEN_UTILS_WINDOW:
		{
			if (!bUtilsWindowOpen)
			{
				RECT rc;
				RegisterDLLWindowUtilsClass();
				utilsHWND = CreateWindowExA(0, "UtilsWindowClass", "Utils", WS_EX_LAYERED | WS_MINIMIZEBOX, 100, 100, 256, 333, NULL, NULL, inj_hModule, NULL);
				GetWindowRect(utilsHWND, &rc);
				int xPos = (GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2;
				int yPos = (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2;
				SetWindowPos(utilsHWND, 0, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
				ShowWindow(utilsHWND, SW_SHOWNORMAL);
				SetForegroundWindow(utilsHWND);
			}
			break;
		}

		case CLB_OPEN_PVP_WINDOW:
		{
			if (!bPvpWindowOpen)
			{
				RECT rc;
				RegisterDLLWindowPvpClass();
				pvpHWND = CreateWindowExA(0, "PvpWindowClass", "Pvp", WS_EX_LAYERED | WS_MINIMIZEBOX, 100, 100, 425, 400, NULL, NULL, inj_hModule, NULL);
				GetWindowRect(pvpHWND, &rc);
				int xPos = (GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2;
				int yPos = (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2;
				SetWindowPos(pvpHWND, 0, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
				ShowWindow(pvpHWND, SW_SHOWNORMAL);
				SetForegroundWindow(pvpHWND);
			}
			break;
		}

		case CLB_OPEN_ALARMS_WINDOW:
		{
			if (!bAlarmsWindowOpen)
			{
				RECT rc;
				RegisterDLLWindowAlarmsClass();
				alarmsHWND = CreateWindowExA(0, "AlarmsWindowClass", "Alarms", WS_EX_LAYERED | WS_MINIMIZEBOX, 100, 100, 315, 295, NULL, NULL, inj_hModule, NULL);
				GetWindowRect(alarmsHWND, &rc);
				int xPos = (GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2;
				int yPos = (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2;
				SetWindowPos(alarmsHWND, 0, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
				ShowWindow(alarmsHWND, SW_SHOWNORMAL);
				SetForegroundWindow(alarmsHWND);
			}
			break;
		}

		case CLB_OPEN_DISCORD_WINDOW:
		{
			if (!bDiscordWindowOpen)
			{
				RECT rc;
				RegisterDLLWindowDiscordClass();
				discordHWND = CreateWindowExA(0, "DiscordWindowClass", "Discord", WS_EX_LAYERED | WS_MINIMIZEBOX, 100, 100, 315, 295, NULL, NULL, inj_hModule, NULL);
				GetWindowRect(discordHWND, &rc);
				int xPos = (GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2;
				int yPos = (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2;
				SetWindowPos(discordHWND, 0, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
				ShowWindow(discordHWND, SW_SHOWNORMAL);
				SetForegroundWindow(discordHWND);
			}
			break;
		}

		case CLB_OPEN_RUNE_MAKER_WINDOW:
		{
			if (!bRuneMakerWindowOpen)
			{
				RECT rc;
				RegisterDLLWindowRuneMakerClass();
				runeMakerHWND = CreateWindowExA(0, "RuneMakerWindowClass", "Rune Maker", WS_EX_LAYERED | WS_MINIMIZEBOX, 100, 100, 315, 295, NULL, NULL, inj_hModule, NULL);
				GetWindowRect(runeMakerHWND, &rc);
				int xPos = (GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2;
				int yPos = (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2;
				SetWindowPos(runeMakerHWND, 0, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
				ShowWindow(runeMakerHWND, SW_SHOWNORMAL);
				SetForegroundWindow(runeMakerHWND);
			}
			break;
		}

		case CLB_OPEN_AUTO_LOGOUT_WINDOW:
		{
			if (!bAutoLogoutWindowOpen)
			{
				RECT rc;
				RegisterDLLWindowAutoLogoutClass();
				autoLogoutHWND = CreateWindowExA(0, "AutoLogoutWindowClass", "Auto Logout", WS_EX_LAYERED | WS_MINIMIZEBOX, 100, 100, 315, 295, NULL, NULL, inj_hModule, NULL);
				GetWindowRect(autoLogoutHWND, &rc);
				int xPos = (GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2;
				int yPos = (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2;
				SetWindowPos(autoLogoutHWND, 0, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
				ShowWindow(autoLogoutHWND, SW_SHOWNORMAL);
				SetForegroundWindow(autoLogoutHWND);
			}
			break;
		}

		}
		break;
	}
	return DefWindowProc(hWindow, uMessage, wParam, lParam);
}



LRESULT CALLBACK Menu::PvpMessageHandler(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	switch (uMessage)
	{
	case WM_CREATE:
		bPvpWindowOpen = true;
		CreatePvpMenu(hWindow);
		CheckDlgButton(hWindow, CLB_HOLD_TARGET, bHoldTarget);
		CheckDlgButton(hWindow, CLB_DASH, bDash);
		CheckDlgButton(hWindow, CLB_MAGIC_SHIELD, bManaShield);
		CheckDlgButton(hWindow, CLB_KILL_ENEMY_ON_LOW_HP, bKillTarget);
		ToggleKillTarget();
		CheckDlgButton(hWindow, CLB_HASTE, bAutoHaste);
		ToggleHaste();
		CheckDlgButton(hWindow, CLB_ANTI_PARALAYSE, bAntiParalayse);
		ToggleAntiParalayse();
		CheckDlgButton(hWindow, CLB_EQUIP_MODE_AS_HOTKEY, bEquipModeHotkey);
		ToggleItemBalancer();
		CheckDlgButton(hWindow, CLB_EQUIP_MODE_AS_MOVE_ITEM, bEquipModeMoveItem);
		ToggleItemBalancer();
		break;
	case WM_CLOSE:
		bPvpWindowOpen = false;
		break;
	case WM_DESTROY:
		//KillTimer(hWindow, 0);
		bPvpWindowOpen = false;
		bLoadedAmulets = false;
		selectedAmuletOption = -1;
		bLoadedRings = false;
		selectedRingsOption = -1;
		//PostQuitMessage(0);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case CLB_HOLD_TARGET:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (SendDlgItemMessage(hWindow, CLB_HOLD_TARGET, BM_GETCHECK, 0, 0))
				{
					bHoldTarget = !bHoldTarget;
					CheckDlgButton(hWindow, CLB_HOLD_TARGET, bHoldTarget);
				}
				else
				{
					bHoldTarget = !bHoldTarget;
					CheckDlgButton(hWindow, CLB_HOLD_TARGET, bHoldTarget);
				}
				break;

			}
			break;
		case CLB_DASH:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (SendDlgItemMessage(hWindow, CLB_DASH, BM_GETCHECK, 0, 0))
				{
					bDash = !bDash;
					CheckDlgButton(hWindow, CLB_DASH, bDash);
					//UpdateInfoConsole("Use numpad keys to dash.");
				}
				else
				{
					bDash = !bDash;
					CheckDlgButton(hWindow, CLB_DASH, bDash);
				}
				break;
			}
			break;
		case CLB_MAGIC_SHIELD:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (SendDlgItemMessage(hWindow, CLB_MAGIC_SHIELD, BM_GETCHECK, 0, 0))
				{
					bManaShield = !bManaShield;
					CheckDlgButton(hWindow, CLB_MAGIC_SHIELD, bManaShield);
				}
				else
				{
					bManaShield = !bManaShield;
					CheckDlgButton(hWindow, CLB_MAGIC_SHIELD, bManaShield);
				}
				break;
			}
			break;
		case CLB_KILL_ENEMY_ON_LOW_HP:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (SendDlgItemMessage(hWindow, CLB_KILL_ENEMY_ON_LOW_HP, BM_GETCHECK, 0, 0))
				{
					bKillTarget = !bKillTarget;
					CheckDlgButton(hWindow, CLB_KILL_ENEMY_ON_LOW_HP, bKillTarget);
					ToggleKillTarget();
					//UpdateInfoConsole("Use spell when target is on x hp %");
				}
				else
				{
					bKillTarget = !bKillTarget;
					CheckDlgButton(hWindow, CLB_KILL_ENEMY_ON_LOW_HP, bKillTarget);
					ToggleKillTarget();
					//UpdateInfoConsole("Use spell when target is on x hp %");
				}
				break;
			}
			break;
		case CLB_HASTE:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (SendDlgItemMessage(hWindow, CLB_HASTE, BM_GETCHECK, 0, 0))
				{
					bAutoHaste = !bAutoHaste;
					CheckDlgButton(hWindow, CLB_HASTE, bAutoHaste);
					ToggleHaste();
					//UpdateInfoConsole("Use spell when target is on x hp %");
				}
				else
				{
					bAutoHaste = !bAutoHaste;
					CheckDlgButton(hWindow, CLB_HASTE, bAutoHaste);
					ToggleHaste();
					//UpdateInfoConsole("Use spell when target is on x hp %");
				}
				break;
			}
			break;
		case CLB_ANTI_PARALAYSE:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (SendDlgItemMessage(hWindow, CLB_HASTE, BM_GETCHECK, 0, 0))
				{
					bAntiParalayse = !bAntiParalayse;
					CheckDlgButton(hWindow, CLB_ANTI_PARALAYSE, bAntiParalayse);
					ToggleAntiParalayse();
					//UpdateInfoConsole("Use spell when target is on x hp %");
				}
				else
				{
					bAntiParalayse = !bAntiParalayse;
					CheckDlgButton(hWindow, CLB_ANTI_PARALAYSE, bAntiParalayse);
					ToggleAntiParalayse();
					//UpdateInfoConsole("Use spell when target is on x hp %");
				}
				break;
			}
			break;
		case CLB_EQUIP_MODE_AS_HOTKEY:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (SendDlgItemMessage(hWindow, CLB_EQUIP_MODE_AS_HOTKEY, BM_GETCHECK, 0, 0))
				{
					bEquipModeHotkey = !bEquipModeHotkey;
					CheckDlgButton(hWindow, CLB_EQUIP_MODE_AS_HOTKEY, bEquipModeHotkey);
					ToggleItemBalancer();
					//UpdateInfoConsole("Bot will use equip packet to equip/dequip items as you would press a hotkey.");

				}
				else
				{
					bEquipModeHotkey = !bEquipModeHotkey;
					CheckDlgButton(hWindow, CLB_EQUIP_MODE_AS_HOTKEY, bEquipModeHotkey);
					ToggleItemBalancer();
					//UpdateInfoConsole("Bot will use equip packet to equip/dequip items as you would press a hotkey.");

				}
				break;
			}
			break;
		case CLB_EQUIP_MODE_AS_MOVE_ITEM:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (SendDlgItemMessage(hWindow, CLB_EQUIP_MODE_AS_MOVE_ITEM, BM_GETCHECK, 0, 0))
				{
					bEquipModeMoveItem = !bEquipModeMoveItem;
					CheckDlgButton(hWindow, CLB_EQUIP_MODE_AS_MOVE_ITEM, bEquipModeMoveItem);
					ToggleItemBalancer();
					//UpdateInfoConsole("Bot will use move packet to equip/dequip items as you would drag from amulet/ring slot to main backpack.");

				}
				else
				{
					bEquipModeMoveItem = !bEquipModeMoveItem;
					CheckDlgButton(hWindow, CLB_EQUIP_MODE_AS_MOVE_ITEM, bEquipModeMoveItem);
					ToggleItemBalancer();
					//UpdateInfoConsole("Bot will use move packet to equip/dequip items as you would drag from amulet/ring slot to main backpack.");

				}
				break;
			}
			break;
		case CLB_LIST_BOX_AMULETS:
			switch (HIWORD(wParam))
			{
			case CBN_SELCHANGE:
			{
				int32_t itemSelected = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
				if (selectedAmuletOption == itemSelected)	break;
				selectedAmuletOption = itemSelected;

				if (!bLoadedAmulets) {
					lBoxAmulets = CreateGeneralComboBox("Amulet Options", 10, 245, 160, 70, CLB_LIST_BOX_AMULETS, hWindow);

					for (int32_t i = 0; i < sizeof(amuletOptions) / sizeof(amuletOptions[0]); i++)
					{
						SendMessageA(lBoxAmulets, CB_ADDSTRING, 0, (LPARAM)amuletOptions[i].name);
					}


					CreateGeneralLabel("Equip%:", 180, 245, 52, 20, hWindow);


					inputHoldAmulet.inputHpPerc = CreateGeneralInputBox("90", 240, 245, 30, 20, hWindow);

					CreateGeneralLabel("Item Id:", 280, 245, 45, 20, hWindow);

					inputHoldAmulet.inputItemId = CreateGeneralInputBox("3081", 330, 245, 50, 20, hWindow);

					bLoadedAmulets = true;
				}

				switch (selectedAmuletOption)
				{
				case 0:
					if (inputDequipAmulet.inputItemId)
					{
						DestroyWindow(inputDequipAmulet.inputItemId);
						inputDequipAmulet.inputItemId = NULL;
						if (itemsToDeleteAmulets[0])
						{
							DestroyWindow(itemsToDeleteAmulets[0]);
							DestroyWindow(itemsToDeleteAmulets[1]);
						}
					}
					else if (inputBalanceAmulet.inputItemId)
					{
						DestroyWindow(inputBalanceAmulet.inputItemId);
						inputBalanceAmulet.inputItemId = NULL;
						if (itemsToDeleteAmulets[0])
						{
							DestroyWindow(itemsToDeleteAmulets[0]);
							DestroyWindow(itemsToDeleteAmulets[1]);
						}
					}
					break;
				case 1:
					if (inputBalanceAmulet.inputItemId)
					{
						DestroyWindow(inputBalanceAmulet.inputItemId);
						inputBalanceAmulet.inputItemId = NULL;
						if (itemsToDeleteAmulets[0])
						{
							DestroyWindow(itemsToDeleteAmulets[0]);
							DestroyWindow(itemsToDeleteAmulets[1]);
						}
					}
					itemsToDeleteAmulets[0] = CreateGeneralLabel("Dequip:", 180, 270, 52, 20, hWindow);

					itemsToDeleteAmulets[1] = CreateGeneralLabel("Item Id:", 280, 270, 45, 20, hWindow);

					inputDequipAmulet.inputItemId = CreateGeneralInputBox("3081", 330, 270, 50, 20, hWindow);
					break;
				case 2:
					if (inputDequipAmulet.inputItemId)
					{
						DestroyWindow(inputDequipAmulet.inputItemId);
						inputDequipAmulet.inputItemId = NULL;
						if (itemsToDeleteAmulets[0])
						{
							DestroyWindow(itemsToDeleteAmulets[0]);
							DestroyWindow(itemsToDeleteAmulets[1]);
						}
					}
					itemsToDeleteAmulets[0] = CreateGeneralLabel("Balance:", 180, 270, 52, 20, hWindow);

					itemsToDeleteAmulets[1] = CreateGeneralLabel("Item Id:", 280, 270, 45, 20, hWindow);

					inputBalanceAmulet.inputItemId = CreateGeneralInputBox("7532", 330, 270, 50, 20, hWindow);
					break;
				}
				break;
			}
			}
			break;

			case CLB_LIST_BOX_RINGS:
				switch (HIWORD(wParam))
				{
				case CBN_SELCHANGE:
				{
					int32_t itemSelected = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
					if (selectedRingsOption == itemSelected)	break;
					selectedRingsOption = itemSelected;
					if (!bLoadedRings)
					{

						lBoxRings = CreateGeneralComboBox("Ring Options", 10, 300, 160, 70, CLB_LIST_BOX_RINGS, hWindow);

						for (int32_t i = 0; i < sizeof(ringOptions) / sizeof(ringOptions[0]); i++)
						{
							SendMessageA(lBoxRings, CB_ADDSTRING, 0, (LPARAM)ringOptions[i].name);
						}

						CreateGeneralLabel("Equip%:", 180, 300, 52, 20, hWindow);

						inputHoldRing.inputHpPerc = CreateGeneralInputBox("90", 240, 300, 30, 20, hWindow);

						CreateGeneralLabel("Item Id:", 280, 300, 45, 20, hWindow);

						inputHoldRing.inputItemId = CreateGeneralInputBox("3051", 330, 300, 50, 20, hWindow);

						bLoadedRings = true;
					}

					switch (selectedRingsOption)
					{
					case 0:
						if (inputDequipRing.inputItemId)
						{
							DestroyWindow(inputDequipRing.inputItemId);
							inputDequipRing.inputItemId = NULL;
							if (itemsToDeleteRings[0])
							{
								DestroyWindow(itemsToDeleteRings[0]);
								DestroyWindow(itemsToDeleteRings[1]);
							}
						}
						else if (inputBalanceRing.inputItemId)
						{
							DestroyWindow(inputBalanceRing.inputItemId);
							inputBalanceRing.inputItemId = NULL;
							if (itemsToDeleteRings[0])
							{
								DestroyWindow(itemsToDeleteRings[0]);
								DestroyWindow(itemsToDeleteRings[1]);
							}
						}
						break;
					case 1:
						if (inputBalanceRing.inputItemId)
						{
							DestroyWindow(inputBalanceRing.inputItemId);
							inputBalanceRing.inputItemId = NULL;
							if (itemsToDeleteRings[0])
							{
								DestroyWindow(itemsToDeleteRings[0]);
								DestroyWindow(itemsToDeleteRings[1]);
							}
						}
						itemsToDeleteRings[0] = CreateGeneralLabel("Dequip:", 180, 325, 42, 20, hWindow);
						itemsToDeleteRings[1] = CreateGeneralLabel("Item Id:", 280, 325, 45, 20, hWindow);
						inputDequipRing.inputItemId = CreateGeneralInputBox("3049", 330, 325, 50, 20, hWindow);
						break;
					case 2:
						if (inputDequipRing.inputItemId)
						{
							DestroyWindow(inputDequipRing.inputItemId);
							inputDequipRing.inputItemId = NULL;
							if (itemsToDeleteRings[0])
							{
								DestroyWindow(itemsToDeleteRings[0]);
								DestroyWindow(itemsToDeleteRings[1]);
							}
						}
						itemsToDeleteRings[0] = CreateGeneralLabel("Balance:", 180, 325, 50, 20, hWindow);
						itemsToDeleteRings[1] = CreateGeneralLabel("Item Id:", 280, 325, 45, 20, hWindow);
						inputBalanceRing.inputItemId = CreateGeneralInputBox("3048", 330, 325, 50, 20, hWindow);
						break;
					}
					break;
				}
				}
				break;
		}

		break;
	}
	return DefWindowProc(hWindow, uMessage, wParam, lParam);
}

LRESULT CALLBACK Menu::AlarmsMessageHandler(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	switch (uMessage)
	{
	case WM_CREATE:
		bAlarmsWindowOpen = true;
		CreateAlarmsMenu(hWindow);
		CheckDlgButton(hWindow, CLB_ENEMY_ON_SCREEN, bEnemyOnScreen);
		CheckDlgButton(hWindow, CLB_PLAYER_ON_SCREEN, bPlayerOnScreen);
		CheckDlgButton(hWindow, CLB_MONSTER_ON_SCREEN, bMonsterOnScreen);
		CheckDlgButton(hWindow, CLB_SKULL_ON_SCREEN, bSkullOnScreen);
		CheckDlgButton(hWindow, CLB_LOW_HEALTH, bLowHealth);
		EnableWindow(inputLowHealth.inputBox, !bLowHealth);
		CheckDlgButton(hWindow, CLB_FIST_HIGHER_THAN, bFistFightingHigherThan);
		EnableWindow(inputFistHigherThan.inputBox, !bFistFightingHigherThan);
		CheckDlgButton(hWindow, CLB_DISTANCE_FIHGTING_HIGHER_THAN, bDistanceFightingHigherThan);
		EnableWindow(inputDistanceFightingHigherThan.inputBox, !bDistanceFightingHigherThan);
		CheckDlgButton(hWindow, CLB_SHIELDING_HIGHER_THAN, bShieldingHigherThan);
		EnableWindow(inputSwordFightingHigherThan.inputBox, !bSwordFightingHigherThan);
		CheckDlgButton(hWindow, CLB_SWORD_FIGHTING_HIGHER_THAN, bSwordFightingHigherThan);
		EnableWindow(inputShieldingHigherThan.inputBox, !bShieldingHigherThan);
		
		CheckDlgButton(hWindow, CLB_LEVEL, bLevelHigherThan);
		EnableWindow(inputLevelHigherThan.inputBox, !bLevelHigherThan);

		CheckDlgButton(hWindow, CLB_LOW_MANA, bLowMana);
		EnableWindow(inputLowMana.inputBox, !bLowMana);
		break;
	case WM_CLOSE:
		bAlarmsWindowOpen = false;
		break;
	case WM_DESTROY:
		bAlarmsWindowOpen = false;
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case CLB_ENEMY_ON_SCREEN:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (SendDlgItemMessage(hWindow, CLB_ENEMY_ON_SCREEN, BM_GETCHECK, 0, 0))
				{
					bEnemyOnScreen = !bEnemyOnScreen;
					CheckDlgButton(hWindow, CLB_ENEMY_ON_SCREEN, bEnemyOnScreen);
					//UpdateInfoConsole("Alarm will be activated when enemy guild member enters screen.");
				}
				else
				{
					bEnemyOnScreen = !bEnemyOnScreen;
					CheckDlgButton(hWindow, CLB_ENEMY_ON_SCREEN, bEnemyOnScreen);
					//UpdateInfoConsole("Alarm will be activated when enemy guild member enters screen.");
				}
				break;

			}
			break;
		case CLB_PLAYER_ON_SCREEN:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (SendDlgItemMessage(hWindow, CLB_PLAYER_ON_SCREEN, BM_GETCHECK, 0, 0))
				{
					bPlayerOnScreen = !bPlayerOnScreen;
					CheckDlgButton(hWindow, CLB_PLAYER_ON_SCREEN, bPlayerOnScreen);
					//UpdateInfoConsole("Alarm will be activated when player enters screen.");
				}
				else
				{
					bPlayerOnScreen = !bPlayerOnScreen;
					CheckDlgButton(hWindow, CLB_PLAYER_ON_SCREEN, bPlayerOnScreen);
					//UpdateInfoConsole("Alarm will be activated when player enters screen.");
				}
				break;

			}
			break;
		case CLB_MONSTER_ON_SCREEN:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (SendDlgItemMessage(hWindow, CLB_MONSTER_ON_SCREEN, BM_GETCHECK, 0, 0))
				{
					bMonsterOnScreen = !bMonsterOnScreen;
					CheckDlgButton(hWindow, CLB_MONSTER_ON_SCREEN, bMonsterOnScreen);
					//UpdateInfoConsole("Alarm will be activated when monster enters screen.");
				}
				else
				{
					bMonsterOnScreen = !bMonsterOnScreen;
					CheckDlgButton(hWindow, CLB_MONSTER_ON_SCREEN, bMonsterOnScreen);
					//UpdateInfoConsole("Alarm will be activated when monster enters screen.");
				}
				break;

			}
			break;
		case CLB_SKULL_ON_SCREEN:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (SendDlgItemMessage(hWindow, CLB_MONSTER_ON_SCREEN, BM_GETCHECK, 0, 0))
				{
					bSkullOnScreen = !bSkullOnScreen;
					CheckDlgButton(hWindow, CLB_SKULL_ON_SCREEN, bSkullOnScreen);
					//UpdateInfoConsole("Alarm will be activated when skulled player enters screen.");
				}
				else
				{
					bSkullOnScreen = !bSkullOnScreen;
					CheckDlgButton(hWindow, CLB_SKULL_ON_SCREEN, bSkullOnScreen);
					//UpdateInfoConsole("Alarm will be activated when skulled player enters screen.");
				}
				break;

			}
			break;
		case CLB_LOW_HEALTH:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (SendDlgItemMessage(hWindow, CLB_LOW_HEALTH, BM_GETCHECK, 0, 0))
				{
					bLowHealth = !bLowHealth;
					CheckDlgButton(hWindow, CLB_LOW_HEALTH, bLowHealth);
					//UpdateInfoConsole("Alarm will be activated when you have low health.");
					if (bLowHealth)
					{
						GetWindowTextA(inputLowHealth.inputBox, &buf[0], sizeof(buf));
						inputLowHealth.lowInfo = atoi(buf);
						ToggleHpAlarm();

					}
					EnableWindow(inputLowHealth.inputBox, !bLowHealth);
				}
				else
				{
					bLowHealth = !bLowHealth;
					CheckDlgButton(hWindow, CLB_LOW_HEALTH, bLowHealth);
					//UpdateInfoConsole("Alarm will be activated when you have low health.");
					if (bLowHealth)
					{
						GetWindowTextA(inputLowHealth.inputBox, &buf[0], sizeof(buf));
						inputLowHealth.lowInfo = atoi(buf);
						ToggleHpAlarm();
					}
					EnableWindow(inputLowHealth.inputBox, !bLowHealth);
				}
				break;

			}
			break;
		case CLB_FIST_HIGHER_THAN:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (SendDlgItemMessage(hWindow, CLB_FIST_HIGHER_THAN, BM_GETCHECK, 0, 0))
				{
					bFistFightingHigherThan = !bFistFightingHigherThan;
					CheckDlgButton(hWindow, CLB_FIST_HIGHER_THAN, bFistFightingHigherThan);
					//UpdateInfoConsole("Alarm will be activated when you have low health.");
					if (bFistFightingHigherThan)
					{
						GetWindowTextA(inputFistHigherThan.inputBox, &buf[0], sizeof(buf));
						inputFistHigherThan.lowInfo = atoi(buf);
						ToggleFistFightingAlarm();

					}
					EnableWindow(inputFistHigherThan.inputBox, !bFistFightingHigherThan);
				}
				else
				{
					bFistFightingHigherThan = !bFistFightingHigherThan;
					CheckDlgButton(hWindow, CLB_FIST_HIGHER_THAN, bFistFightingHigherThan);
					//UpdateInfoConsole("Alarm will be activated when you have low health.");
					if (bFistFightingHigherThan)
					{
						GetWindowTextA(inputFistHigherThan.inputBox, &buf[0], sizeof(buf));
						inputFistHigherThan.lowInfo = atoi(buf);
						ToggleFistFightingAlarm();
					}
					EnableWindow(inputFistHigherThan.inputBox, !bFistFightingHigherThan);
				}
				break;

			}
			break;
		case CLB_DISTANCE_FIHGTING_HIGHER_THAN:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (SendDlgItemMessage(hWindow, CLB_DISTANCE_FIHGTING_HIGHER_THAN, BM_GETCHECK, 0, 0))
				{
					bDistanceFightingHigherThan = !bDistanceFightingHigherThan;
					CheckDlgButton(hWindow, CLB_DISTANCE_FIHGTING_HIGHER_THAN, bDistanceFightingHigherThan);
					//UpdateInfoConsole("Alarm will be activated when you have low health.");
					if (bDistanceFightingHigherThan)
					{
						GetWindowTextA(inputDistanceFightingHigherThan.inputBox, &buf[0], sizeof(buf));
						inputDistanceFightingHigherThan.lowInfo = atoi(buf);
						ToggleDistanceFightingAlarm();

					}
					EnableWindow(inputDistanceFightingHigherThan.inputBox, !bDistanceFightingHigherThan);
				}
				else
				{
					bDistanceFightingHigherThan = !bDistanceFightingHigherThan;
					CheckDlgButton(hWindow, CLB_DISTANCE_FIHGTING_HIGHER_THAN, bDistanceFightingHigherThan);
					//UpdateInfoConsole("Alarm will be activated when you have low health.");
					if (bDistanceFightingHigherThan)
					{
						GetWindowTextA(inputDistanceFightingHigherThan.inputBox, &buf[0], sizeof(buf));
						inputDistanceFightingHigherThan.lowInfo = atoi(buf);
						ToggleDistanceFightingAlarm();
					}
					EnableWindow(inputDistanceFightingHigherThan.inputBox, !bDistanceFightingHigherThan);
				}
				break;

			}
			break;
		case CLB_SHIELDING_HIGHER_THAN:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (SendDlgItemMessage(hWindow, CLB_SHIELDING_HIGHER_THAN, BM_GETCHECK, 0, 0))
				{
					bShieldingHigherThan = !bShieldingHigherThan;
					CheckDlgButton(hWindow, CLB_SHIELDING_HIGHER_THAN, bShieldingHigherThan);
					//UpdateInfoConsole("Alarm will be activated when you have low health.");
					if (bShieldingHigherThan)
					{
						GetWindowTextA(inputShieldingHigherThan.inputBox, &buf[0], sizeof(buf));
						inputShieldingHigherThan.lowInfo = atoi(buf);
						ToggleShieldingAlarm();

					}
					EnableWindow(inputShieldingHigherThan.inputBox, !bShieldingHigherThan);
				}
				else
				{
					bShieldingHigherThan = !bShieldingHigherThan;
					CheckDlgButton(hWindow, CLB_SHIELDING_HIGHER_THAN, bShieldingHigherThan);
					//UpdateInfoConsole("Alarm will be activated when you have low health.");
					if (bShieldingHigherThan)
					{
						GetWindowTextA(inputShieldingHigherThan.inputBox, &buf[0], sizeof(buf));
						inputShieldingHigherThan.lowInfo = atoi(buf);
						ToggleShieldingAlarm();
					}
					EnableWindow(inputShieldingHigherThan.inputBox, !bShieldingHigherThan);
				}
				break;

			}
			break;
		case CLB_SWORD_FIGHTING_HIGHER_THAN:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (SendDlgItemMessage(hWindow, CLB_SWORD_FIGHTING_HIGHER_THAN, BM_GETCHECK, 0, 0))
				{
					bSwordFightingHigherThan = !bSwordFightingHigherThan;
					CheckDlgButton(hWindow, CLB_SWORD_FIGHTING_HIGHER_THAN, bSwordFightingHigherThan);
					//UpdateInfoConsole("Alarm will be activated when you have low health.");
					if (bSwordFightingHigherThan)
					{
						GetWindowTextA(inputSwordFightingHigherThan.inputBox, &buf[0], sizeof(buf));
						inputSwordFightingHigherThan.lowInfo = atoi(buf);
						ToggleSwordFightingAlarm();

					}
					EnableWindow(inputSwordFightingHigherThan.inputBox, !bSwordFightingHigherThan);
				}
				else
				{
					bSwordFightingHigherThan = !bSwordFightingHigherThan;
					CheckDlgButton(hWindow, CLB_SWORD_FIGHTING_HIGHER_THAN, bSwordFightingHigherThan);
					//UpdateInfoConsole("Alarm will be activated when you have low health.");
					if (bSwordFightingHigherThan)
					{
						GetWindowTextA(inputSwordFightingHigherThan.inputBox, &buf[0], sizeof(buf));
						inputSwordFightingHigherThan.lowInfo = atoi(buf);
						ToggleSwordFightingAlarm();
					}
					EnableWindow(inputSwordFightingHigherThan.inputBox, !bSwordFightingHigherThan);
				}
				break;

			}
			break;
		case CLB_LEVEL:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (SendDlgItemMessage(hWindow, CLB_LEVEL, BM_GETCHECK, 0, 0))
				{
					bLevelHigherThan = !bLevelHigherThan;
					CheckDlgButton(hWindow, CLB_LEVEL, bLevelHigherThan);
					//UpdateInfoConsole("Alarm will be activated when you have low health.");
					if (bLevelHigherThan)
					{
						GetWindowTextA(inputLevelHigherThan.inputBox, &buf[0], sizeof(buf));
						inputLevelHigherThan.lowInfo = atoi(buf);
						ToggleLevelAlarm();

					}
					EnableWindow(inputLevelHigherThan.inputBox, !bLevelHigherThan);
				}
				else
				{
					bLevelHigherThan = !bLevelHigherThan;
					CheckDlgButton(hWindow, CLB_LEVEL, bLevelHigherThan);
					//UpdateInfoConsole("Alarm will be activated when you have low health.");
					if (bLevelHigherThan)
					{
						GetWindowTextA(inputLevelHigherThan.inputBox, &buf[0], sizeof(buf));
						inputLevelHigherThan.lowInfo = atoi(buf);
						ToggleLevelAlarm();
					}
					EnableWindow(inputLevelHigherThan.inputBox, !bLevelHigherThan);
				}
				break;

			}
			break;
		case CLB_LOW_MANA:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (SendDlgItemMessage(hWindow, CLB_LOW_MANA, BM_GETCHECK, 0, 0))
				{
					bLowMana = !bLowMana;
					CheckDlgButton(hWindow, CLB_LOW_MANA, bLowMana);
					//UpdateInfoConsole("Alarm will be activated when you have low mana.");
					if (bLowMana)
					{
						GetWindowTextA(inputLowMana.inputBox, &buf[0], sizeof(buf));
						inputLowMana.lowInfo = atoi(buf);
						ToggleMpAlarm();
					}
					EnableWindow(inputLowMana.inputBox, !bLowMana);
				}
				else
				{
					bLowMana = !bLowMana;
					CheckDlgButton(hWindow, CLB_LOW_MANA, bLowMana);
					//UpdateInfoConsole("Alarm will be activated when you have low mana.");
					if (bLowMana)
					{
						GetWindowTextA(inputLowMana.inputBox, &buf[0], sizeof(buf));
						inputLowMana.lowInfo = atoi(buf);
						ToggleMpAlarm();
					}
					EnableWindow(inputLowMana.inputBox, !bLowMana);
				}
				break;

			}
			break;
		}
		break;
	}
	return DefWindowProc(hWindow, uMessage, wParam, lParam);


}

LRESULT CALLBACK Menu::DiscordMessageHandler(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	switch (uMessage)
	{
	case WM_CREATE:
		bDiscordWindowOpen = true;
		CreateDiscordMenu(hWindow);
		CheckDlgButton(hWindow, CLB_DISCORD, bDiscord);
		ToggleDiscord();
		break;
	case WM_CLOSE:
		bDiscordWindowOpen = false;
		break;
	case WM_DESTROY:
		bDiscordWindowOpen = false;
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case CLB_DISCORD:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (SendDlgItemMessage(hWindow, CLB_DISCORD, BM_GETCHECK, 0, 0))
				{
					bDiscord = !bDiscord;
					CheckDlgButton(hWindow, CLB_DISCORD, bDiscord);
					ToggleDiscord();
				}
				else
				{
					bDiscord = !bDiscord;
					CheckDlgButton(hWindow, CLB_DISCORD, bDiscord);
					ToggleDiscord();
				}
				break;
			}
			break;
		}
	}
	return DefWindowProc(hWindow, uMessage, wParam, lParam);
}

LRESULT CALLBACK Menu::RuneMakerMessageHandler(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	switch (uMessage)
	{
	case WM_CREATE:
		bRuneMakerWindowOpen = true;
		CreateRuneMakerMenu(hWindow);
		CheckDlgButton(hWindow, CLB_RUNE_MAKER, bRuneMaker);
		ToggleRuneMaker();

		break;
	case WM_CLOSE:
		bRuneMakerWindowOpen = false;
		break;
	case WM_DESTROY:
		bRuneMakerWindowOpen = false;
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case CLB_RUNE_MAKER:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (SendDlgItemMessage(hWindow, CLB_RUNE_MAKER, BM_GETCHECK, 0, 0))
				{
					bRuneMaker = !bRuneMaker;
					CheckDlgButton(hWindow, CLB_RUNE_MAKER, bRuneMaker);
					ToggleRuneMaker();
				}
				else
				{
					bRuneMaker = !bRuneMaker;
					CheckDlgButton(hWindow, CLB_RUNE_MAKER, bRuneMaker);
					ToggleRuneMaker();
				}
				break;
			}
			break;
		}
	}
	return DefWindowProc(hWindow, uMessage, wParam, lParam);
}

LRESULT CALLBACK Menu::AutoLogoutMessageHandler(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	switch (uMessage)
	{
	case WM_CREATE:
		bAutoLogoutWindowOpen = true;

		CreateAutoLogoutMenu(hWindow);
		CheckDlgButton(hWindow, CLB_AUTO_LOGOUT_ENEMY_ON_SCREEN, bAutoLogoutEnemyOnScreen);
		CheckDlgButton(hWindow, CLB_AUTO_LOGOUT_PLAYER_ON_SCREEN, bAutoLogoutPlayerOnScreen);
		CheckDlgButton(hWindow, CLB_AUTO_LOGOUT_SKULL_ON_SCREEN, bAutoLogoutSkullOnScreen);

		CheckDlgButton(hWindow, CLB_AUTO_LOGOUT_LOW_MANA, bAutoLogoutLowMana);
		CheckDlgButton(hWindow, CLB_AUTO_LOGOUT_LOW_SOUL, bAutoLogoutLowSoul);
		CheckDlgButton(hWindow, CLB_AUTO_LOGOUT_LOW_CAP, bAutoLogoutLowCap);

		ToggleAutoLogout();


		break;
	case WM_CLOSE:
		bAutoLogoutWindowOpen = false;
		break;
	case WM_DESTROY:
		bAutoLogoutWindowOpen = false;
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case CLB_AUTO_LOGOUT_ENEMY_ON_SCREEN:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (SendDlgItemMessage(hWindow, CLB_AUTO_LOGOUT_ENEMY_ON_SCREEN, BM_GETCHECK, 0, 0))
				{
					bAutoLogoutEnemyOnScreen = !bAutoLogoutEnemyOnScreen;
					CheckDlgButton(hWindow, CLB_AUTO_LOGOUT_ENEMY_ON_SCREEN, bAutoLogoutEnemyOnScreen);
				}
				else
				{
					bAutoLogoutEnemyOnScreen = !bAutoLogoutEnemyOnScreen;
					CheckDlgButton(hWindow, CLB_AUTO_LOGOUT_ENEMY_ON_SCREEN, bAutoLogoutEnemyOnScreen);
				}
				break;
			}
			break;
		case CLB_AUTO_LOGOUT_PLAYER_ON_SCREEN:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (SendDlgItemMessage(hWindow, CLB_AUTO_LOGOUT_PLAYER_ON_SCREEN, BM_GETCHECK, 0, 0))
				{
					bAutoLogoutPlayerOnScreen = !bAutoLogoutPlayerOnScreen;
					CheckDlgButton(hWindow, CLB_AUTO_LOGOUT_PLAYER_ON_SCREEN, bAutoLogoutPlayerOnScreen);
				}
				else
				{
					bAutoLogoutPlayerOnScreen = !bAutoLogoutPlayerOnScreen;
					CheckDlgButton(hWindow, CLB_AUTO_LOGOUT_PLAYER_ON_SCREEN, bAutoLogoutPlayerOnScreen);
				}
				break;
			}
			break;
		case CLB_AUTO_LOGOUT_SKULL_ON_SCREEN:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (SendDlgItemMessage(hWindow, CLB_AUTO_LOGOUT_SKULL_ON_SCREEN, BM_GETCHECK, 0, 0))
				{
					bAutoLogoutSkullOnScreen = !bAutoLogoutSkullOnScreen;
					CheckDlgButton(hWindow, CLB_AUTO_LOGOUT_SKULL_ON_SCREEN, bAutoLogoutSkullOnScreen);
				}
				else
				{
					bAutoLogoutSkullOnScreen = !bAutoLogoutSkullOnScreen;
					CheckDlgButton(hWindow, CLB_AUTO_LOGOUT_SKULL_ON_SCREEN, bAutoLogoutSkullOnScreen);
				}
				break;
			}
			break;
		case CLB_AUTO_LOGOUT_LOW_MANA:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (SendDlgItemMessage(hWindow, CLB_AUTO_LOGOUT_LOW_MANA, BM_GETCHECK, 0, 0))
				{
					bAutoLogoutLowMana = !bAutoLogoutLowMana;
					CheckDlgButton(hWindow, CLB_AUTO_LOGOUT_LOW_MANA, bAutoLogoutLowMana);
					ToggleAutoLogout();
				}
				else
				{
					bAutoLogoutLowMana = !bAutoLogoutLowMana;
					CheckDlgButton(hWindow, CLB_AUTO_LOGOUT_LOW_MANA, bAutoLogoutLowMana);
					ToggleAutoLogout();
				}
				break;
			}
			break;
		case CLB_AUTO_LOGOUT_LOW_SOUL:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (SendDlgItemMessage(hWindow, CLB_AUTO_LOGOUT_LOW_SOUL, BM_GETCHECK, 0, 0))
				{
					bAutoLogoutLowSoul = !bAutoLogoutLowSoul;
					CheckDlgButton(hWindow, CLB_AUTO_LOGOUT_LOW_SOUL, bAutoLogoutLowSoul);
					ToggleAutoLogout();
				}
				else
				{
					bAutoLogoutLowSoul = !bAutoLogoutLowSoul;
					CheckDlgButton(hWindow, CLB_AUTO_LOGOUT_LOW_SOUL, bAutoLogoutLowSoul);
					ToggleAutoLogout();
				}
				break;
			}
			break;
		case CLB_AUTO_LOGOUT_LOW_CAP:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (SendDlgItemMessage(hWindow, CLB_AUTO_LOGOUT_LOW_CAP, BM_GETCHECK, 0, 0))
				{
					bAutoLogoutLowCap = !bAutoLogoutLowCap;
					CheckDlgButton(hWindow, CLB_AUTO_LOGOUT_LOW_CAP, bAutoLogoutLowCap);
					ToggleAutoLogout();
				}
				else
				{
					bAutoLogoutLowCap = !bAutoLogoutLowCap;
					CheckDlgButton(hWindow, CLB_AUTO_LOGOUT_LOW_CAP, bAutoLogoutLowCap);
					ToggleAutoLogout();
				}
				break;
			}
			break;
		}
	}
	return DefWindowProc(hWindow, uMessage, wParam, lParam);
}



BOOL Menu::RegisterDLLWindowClass(const wchar_t szClassName[])
{
	HBRUSH hb = ::CreateSolidBrush(RGB(30, 144, 255));
	WNDCLASSEX wc;
	wc.hInstance = inj_hModule;
	wc.lpszClassName = (LPCWSTR)szClassName;
	wc.lpfnWndProc = MessageHandler;
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.cbSize = sizeof(wc);
	wc.hIcon = LoadIcon(NULL, IDI_ERROR);
	wc.hIconSm = LoadIcon(wc.hInstance, MAKEINTRESOURCE(101));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszMenuName = NULL;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = hb;
	if (!RegisterClassEx(&wc))
		return 0;
	return 1;
}

void Menu::RegisterDLLWindowHealerClass()
{

	WNDCLASSEX wcex;


	wcex.hInstance = inj_hModule;
	wcex.lpszClassName = L"HealerWindowClass";
	wcex.lpfnWndProc = HealerMessageHandler;
	wcex.style = CS_VREDRAW | CS_HREDRAW;
	wcex.cbSize = sizeof(wcex);
	wcex.hIcon = LoadIcon(NULL, IDI_ERROR);
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(101));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.lpszMenuName = NULL;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);

	RegisterClassEx(&wcex);
}

void Menu::RegisterDLLWindowTimersClass()
{

	WNDCLASSEX wcex;


	wcex.hInstance = inj_hModule;
	wcex.lpszClassName = L"TimersWindowClass";
	wcex.lpfnWndProc = TimersMessageHandler;
	wcex.style = CS_VREDRAW | CS_HREDRAW;
	wcex.cbSize = sizeof(wcex);
	wcex.hIcon = LoadIcon(NULL, IDI_ERROR);
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(101));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.lpszMenuName = NULL;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);

	RegisterClassEx(&wcex);
}

void Menu::RegisterDLLWindowUtilsClass()
{

	WNDCLASSEX wcex;


	wcex.hInstance = inj_hModule;
	wcex.lpszClassName = L"UtilsWindowClass";
	wcex.lpfnWndProc = UtilsMessageHandler;
	wcex.style = CS_VREDRAW | CS_HREDRAW;
	wcex.cbSize = sizeof(wcex);
	wcex.hIcon = LoadIcon(NULL, IDI_ERROR);
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(101));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.lpszMenuName = NULL;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);

	RegisterClassEx(&wcex);
}

void Menu::RegisterDLLWindowPvpClass()
{

	WNDCLASSEX wcex;


	wcex.hInstance = inj_hModule;
	wcex.lpszClassName = L"PvpWindowClass";
	wcex.lpfnWndProc = PvpMessageHandler;
	wcex.style = CS_VREDRAW | CS_HREDRAW;
	wcex.cbSize = sizeof(wcex);
	wcex.hIcon = LoadIcon(NULL, IDI_ERROR);
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(101));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.lpszMenuName = NULL;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);

	RegisterClassEx(&wcex);
}

void Menu::RegisterDLLWindowAlarmsClass()
{

	WNDCLASSEX wcex;


	wcex.hInstance = inj_hModule;
	wcex.lpszClassName = L"AlarmsWindowClass";
	wcex.lpfnWndProc = AlarmsMessageHandler;
	wcex.style = CS_VREDRAW | CS_HREDRAW;
	wcex.cbSize = sizeof(wcex);
	wcex.hIcon = LoadIcon(NULL, IDI_ERROR);
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(101));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.lpszMenuName = NULL;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);

	RegisterClassEx(&wcex);
}

void Menu::RegisterDLLWindowDiscordClass()
{

	WNDCLASSEX wcex;


	wcex.hInstance = inj_hModule;
	wcex.lpszClassName = L"DiscordWindowClass";
	wcex.lpfnWndProc = DiscordMessageHandler;
	wcex.style = CS_VREDRAW | CS_HREDRAW;
	wcex.cbSize = sizeof(wcex);
	wcex.hIcon = LoadIcon(NULL, IDI_ERROR);
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(101));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.lpszMenuName = NULL;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);

	RegisterClassEx(&wcex);
}

void Menu::RegisterDLLWindowRuneMakerClass()
{
	WNDCLASSEX wcex;


	wcex.hInstance = inj_hModule;
	wcex.lpszClassName = L"RuneMakerWindowClass";
	wcex.lpfnWndProc = RuneMakerMessageHandler;
	wcex.style = CS_VREDRAW | CS_HREDRAW;
	wcex.cbSize = sizeof(wcex);
	wcex.hIcon = LoadIcon(NULL, IDI_ERROR);
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(101));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.lpszMenuName = NULL;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);

	RegisterClassEx(&wcex);
}

void Menu::RegisterDLLWindowAutoLogoutClass()
{
	WNDCLASSEX wcex;


	wcex.hInstance = inj_hModule;
	wcex.lpszClassName = L"AutoLogoutWindowClass";
	wcex.lpfnWndProc = AutoLogoutMessageHandler;
	wcex.style = CS_VREDRAW | CS_HREDRAW;
	wcex.cbSize = sizeof(wcex);
	wcex.hIcon = LoadIcon(NULL, IDI_ERROR);
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(101));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.lpszMenuName = NULL;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);

	RegisterClassEx(&wcex);
}

HWND Menu::CreateGeneralCheckBox(const char* lpWindowName, int32_t x, int32_t y, int32_t iWidth, int32_t iHeight, int32_t hMenu, HWND hWnd)
{
	HWND hwnd = CreateWindowExA(0, "button", lpWindowName, WS_CHILD | WS_VISIBLE | BS_CHECKBOX, x, y, iWidth, iHeight, hWnd, (HMENU)hMenu, NULL, 0);
	return hwnd;
}
HWND Menu::CreateGeneralButton(const char* lpWindowName, int32_t x, int32_t y, int32_t iWidth, int32_t iHeight, int32_t hMenu, HWND hWnd)
{
	HWND hwnd = CreateWindowExA(0, "button", lpWindowName, WS_CHILD | WS_VISIBLE, x, y, iWidth, iHeight, hWnd, (HMENU)hMenu, NULL, 0);
	SendMessage(hwnd, WM_SETFONT, (WPARAM)hFont, true);
	return hwnd;
}
HWND Menu::CreateGeneralLabel(const char* lpWindowName, int32_t x, int32_t y, int32_t iWidth, int32_t iHeight, HWND hWnd)
{
	HWND hwnd = CreateWindowExA(0, "static", lpWindowName, WS_CHILD | WS_VISIBLE, x, y, iWidth, iHeight, hWnd, 0, NULL, 0);
	SendMessage(hwnd, WM_SETFONT, (WPARAM)hFont, true);
	return hwnd;
}
HWND Menu::CreateGeneralGroupBox(const char* lpWindowName, int32_t x, int32_t y, int32_t iWidth, int32_t iHeight, HWND hWnd)
{
	HWND hwnd = CreateWindowExA(0, "button", lpWindowName, WS_CHILD | WS_VISIBLE | BS_GROUPBOX, x, y, iWidth, iHeight, hWnd, 0, NULL, 0);
	return hwnd;
}
HWND Menu::CreateGeneralListBox(const char* lpWindowName, int32_t x, int32_t y, int32_t iWidth, int32_t iHeight, int32_t hMenu, HWND hWnd)
{
	HWND hwnd = CreateWindowExA(WS_EX_STATICEDGE, "listbox", lpWindowName, WS_VISIBLE | WS_CHILD | LBS_STANDARD | LBS_DISABLENOSCROLL, x, y, iWidth, iHeight, hWnd, (HMENU)hMenu, NULL, 0);
	return hwnd;
}
HWND Menu::CreateGeneralInputBox(const char* lpWindowName, int32_t x, int32_t y, int32_t iWidth, int32_t iHeight, HWND hWnd)
{
	HWND hwnd = CreateWindowExA(WS_EX_STATICEDGE, "edit", lpWindowName, WS_CHILD | WS_BORDER | WS_VISIBLE | ES_AUTOHSCROLL | ES_CENTER, x, y, iWidth, iHeight, hWnd, NULL, NULL, 0);
	SendMessage(hwnd, WM_SETFONT, (WPARAM)hFont, true);
	return hwnd;
}
HWND Menu::CreateGeneralComboBox(const char* lpWindowName, int32_t x, int32_t y, int32_t iWidth, int32_t iHeight, int32_t hMenu, HWND hWnd)
{
	HWND hwnd = CreateWindowExA(WS_EX_STATICEDGE, "combobox", lpWindowName, WS_VISIBLE | WS_CHILD | LBS_STANDARD | LBS_DISABLENOSCROLL, x, y, iWidth, iHeight, hWnd, (HMENU)hMenu, NULL, 0);
	return hwnd;
}

void Menu::Init(HMODULE hModule)
{
	inj_hModule = hModule;
	HMENU hMenu = CreateDLLWindowMenu();

	// Main Window
	RegisterDLLWindowClass(L"DLLWindowClass");
	mainHWND = CreateWindowExA(0, "DLLWindowClass", "Tibia v8.6.0.0", WS_EX_LAYERED | WS_MINIMIZEBOX, 0, 0, mainWindowWidth, mainWindowHeight, NULL, hMenu, inj_hModule, NULL);

	RECT rc;
	//retrieves the dimensions of the bounding rectangle
	GetWindowRect(mainHWND, &rc);

	//get center position for main window
	int xPos = (GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2;
	int yPos = (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2;

	//set position of main window
	SetWindowPos(mainHWND, 0, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

	//show main window
	ShowWindow(mainHWND, SW_SHOWNORMAL);

	//display main window on top
	SetForegroundWindow(mainHWND);



	CreateMainMenu();
}
HWND Menu::CreateButton(const char* lpWindowName, int32_t x, int32_t y, int32_t iWidth, int32_t iHeight, int32_t hMenu)
{
	HWND hwnd = CreateWindowExA(0, "button", lpWindowName, WS_CHILD | WS_VISIBLE, x, y, iWidth, iHeight, mainHWND, (HMENU)hMenu, inj_hModule, 0);
	SendMessage(hwnd, WM_SETFONT, (WPARAM)hFont, true);
	return hwnd;
}

void Menu::CreateMainMenu()
{
	buttonHealer = CreateButton("Healer", 10, 10, 100, 20, CLB_OPEN_HEALER_WINDOW);
	buttonTimers = CreateButton("Timers", 120, 10, 100, 20, CLB_OPEN_TIMERS_WINDOW);
	buttonUtils = CreateButton("Utils", 230, 10, 100, 20, CLB_OPEN_UTILS_WINDOW);
	buttonPvp = CreateButton("Pvp", 340, 10, 100, 20, CLB_OPEN_PVP_WINDOW);
	buttonAlarms = CreateButton("Alarms", 10, 40, 100, 20, CLB_OPEN_ALARMS_WINDOW);
	buttonDiscord = CreateButton("Discord", 120, 40, 100, 20, CLB_OPEN_DISCORD_WINDOW);
	buttonRuneMaker = CreateButton("Rune Maker", 230, 40, 100, 20, CLB_OPEN_RUNE_MAKER_WINDOW);
	buttonAutoLogout = CreateButton("Auto Logout", 340, 40, 100, 20, CLB_OPEN_AUTO_LOGOUT_WINDOW);
}



void Menu::CreateHealerMenu(HWND hWindow)
{

	// REGION HEALING

	std::string LIGHT_HEAL_SPELL_PERC = std::to_string(lightSpell.hpPercentage);
	char const* LHPERC = LIGHT_HEAL_SPELL_PERC.c_str();
	std::string MID_HEAL_SPELL_PERC = std::to_string(midSpell.hpPercentage);
	char const* MHPERC = MID_HEAL_SPELL_PERC.c_str();
	std::string HEAVY_HEAL_SPELL_PERC = std::to_string(heavySpell.hpPercentage);
	char const* HHPERC = HEAVY_HEAL_SPELL_PERC.c_str();


	std::string LIGHT_HEAL_SPELL_MP = std::to_string(lightSpell.manaCost);
	char const* LHSMP = LIGHT_HEAL_SPELL_MP.c_str();
	std::string MID_HEAL_SPELL_MP = std::to_string(midSpell.manaCost);
	char const* MHSMP = MID_HEAL_SPELL_MP.c_str();
	std::string HEAVY_HEAL_SPELL_MP = std::to_string(heavySpell.manaCost);
	char const* HHSMP = HEAVY_HEAL_SPELL_MP.c_str();


	std::string HEALTH_POTION_ID = std::to_string(healthPotionHealer.itemId);
	char const* HPID = HEALTH_POTION_ID.c_str();
	std::string HEALTH_POTION_HPPERC = std::to_string(healthPotionHealer.hpPerc);
	char const* HPPERC = HEALTH_POTION_HPPERC.c_str();


	std::string MANA_POTION_ID = std::to_string(manaPotionHealer.itemId);
	char const* MPID = MANA_POTION_ID.c_str();
	std::string MANA_POTION_HPPERC = std::to_string(manaPotionHealer.manaPerc);
	char const* MPPERC = MANA_POTION_HPPERC.c_str();

	std::string FRIEND_HP_HPPERC = std::to_string(sioUhPercent.friendHpPercentage);
	char const* FHPPERC = FRIEND_HP_HPPERC.c_str();
	std::string MY_HP_HPPERC = std::to_string(sioUhPercent.myHpPercentage);
	char const* MYHPPERC = MY_HP_HPPERC.c_str();

	CreateGeneralGroupBox("Healing Spells / Potions", 5, 0, 310, 188, hWindow);

	cBoxEnableHealer = CreateGeneralCheckBox("", 172, 0, 15, 20, CLB_HEALER, hWindow);

	CreateGeneralLabel("Light:", 10, 25, 40, 20, hWindow);
	CreateGeneralLabel("Mid:", 10, 55, 25, 20, hWindow);
	CreateGeneralLabel("Heavy:", 10, 85, 40, 20, hWindow);

	inputLightSpell.inputSpell = CreateGeneralInputBox(&lightSpell.spell[0], 60, 25, 100, 20, hWindow);
	inputMidSpell.inputSpell = CreateGeneralInputBox(&midSpell.spell[0], 60, 55, 100, 20, hWindow);
	inputHeavySpell.inputSpell = CreateGeneralInputBox(&heavySpell.spell[0], 60, 85, 100, 20, hWindow);

	CreateGeneralLabel("%", 170, 25, 20, 20, hWindow);
	CreateGeneralLabel("%", 170, 55, 20, 20, hWindow);
	CreateGeneralLabel("%", 170, 85, 20, 20, hWindow);

	inputLightSpell.inputHpPerc = CreateGeneralInputBox(LHPERC, 190, 25, 30, 20, hWindow);
	inputMidSpell.inputHpPerc = CreateGeneralInputBox(MHPERC, 190, 55, 30, 20, hWindow);
	inputHeavySpell.inputHpPerc = CreateGeneralInputBox(HHPERC, 190, 85, 30, 20, hWindow);

	CreateGeneralLabel("MP:", 230, 25, 30, 20, hWindow);
	CreateGeneralLabel("MP:", 230, 55, 30, 20, hWindow);
	CreateGeneralLabel("MP:", 230, 85, 30, 20, hWindow);

	inputLightSpell.inputMana = CreateGeneralInputBox(LHSMP, 260, 25, 45, 20, hWindow);
	inputMidSpell.inputMana = CreateGeneralInputBox(MHSMP, 260, 55, 45, 20, hWindow);
	inputHeavySpell.inputMana = CreateGeneralInputBox(HHSMP, 260, 85, 45, 20, hWindow);

	CreateGeneralLabel("Health Potion:", 10, 120, 100, 20, hWindow);
	inputHealthPotion.inputItemId = CreateGeneralInputBox(HPID, 100, 120, 60, 20, hWindow);
	CreateGeneralLabel("HP", 170, 120, 50, 20, hWindow);
	inputHealthPotion.inputHpPerc = CreateGeneralInputBox(HPPERC, 200, 120, 30, 20, hWindow);

	CreateGeneralLabel("Mana Potion:", 10, 150, 100, 20, hWindow);
	inputManaPotion.inputItemId = CreateGeneralInputBox(MPID, 100, 150, 60, 20, hWindow);
	CreateGeneralLabel("MP", 170, 150, 50, 20, hWindow);
	inputManaPotion.inputMana = CreateGeneralInputBox(MPPERC, 200, 150, 30, 20, hWindow);


	CreateGeneralGroupBox("Heal Friends", 5, 188, 310, 100, hWindow);

	buttonSioList = CreateGeneralButton("Sio List", 10, 208, 100, 20, CLB_OPEN_SIO_LIST, hWindow);

	CreateGeneralLabel("FriendHP:", 10, 233, 80, 20, hWindow);
	inputSioHp.inputHpPerc = CreateGeneralInputBox(FHPPERC, 75, 233, 30, 20, hWindow);
	CreateGeneralLabel("%", 110, 233, 20, 20, hWindow);

	CreateGeneralLabel("Sio:", 140, 233, 80, 20, hWindow);
	cBoxHealFriendsSio = CreateGeneralCheckBox("", 170, 233, 10, 20, CLB_HEAL_FRIENDS_SIO, hWindow);

	CreateGeneralLabel("MyHP:", 10, 258, 80, 20, hWindow);
	inputSioMyHp.inputHpPerc = CreateGeneralInputBox(MYHPPERC, 75, 258, 30, 20, hWindow);
	CreateGeneralLabel("%", 110, 258, 20, 20, hWindow);

	CreateGeneralLabel("UH:", 140, 258, 80, 20, hWindow);
	cBoxHealFriendsUH = CreateGeneralCheckBox("", 170, 258, 10, 20, CLB_HEAL_FRIENDS_UH, hWindow);

	lBoxHealFriendsOptions = CreateGeneralComboBox("", 210, 208, 100, 70, CLB_LIST_BOX_HEAL_MODES, hWindow);

	for (int32_t i = 0; i < sizeof(healFriendOptions) / sizeof(healFriendOptions[0]); i++)
	{
		SendMessageA(lBoxHealFriendsOptions, CB_ADDSTRING, 0, (LPARAM)healFriendOptions[i].name);
	}

	SendMessage(lBoxHealFriendsOptions, CB_SETCURSEL, selectedHealFriendMode, (LPARAM)0);
}

void Menu::CreateTimersMenu(HWND hWindow)
{
	std::string HEAL_SPELL = std::to_string(Cooldowns::GetInstance().HEAL_SPELL);
	const char* HEAL_SPELL_COOLDOWN = HEAL_SPELL.c_str();

	std::string POTIONS = std::to_string(Cooldowns::GetInstance().HEAL_ITEM);
	const char* POTIONS_COOLDOWN = POTIONS.c_str();

	std::string RUNES = std::to_string(Cooldowns::GetInstance().ATTACK_ITEM);
	const char* RUNES_COOLDOWN = RUNES.c_str();

	std::string DASH = std::to_string(Cooldowns::GetInstance().BOOST_DELAY);
	const char* DASH_COOLDOWN = DASH.c_str();

	std::string PUSH = std::to_string(Cooldowns::GetInstance().PUSH_CREATURE);
	const char* PUSH_COOLDOWN = PUSH.c_str();

	std::string EQUIP = std::to_string(Cooldowns::GetInstance().EQUIP_ITEM);
	const char* EQUIP_COOLDOWN = EQUIP.c_str();

	std::string DROP = std::to_string(Cooldowns::GetInstance().DROP_ITEM);
	const char* DROP_COOLDOWN = DROP.c_str();

	std::string ADVERTISING = std::to_string(Cooldowns::GetInstance().SAY_ADVERTISING);
	const char* ADVERTISING_COOLDOWN = ADVERTISING.c_str();

	std::string HOLDTARGET = std::to_string(Cooldowns::GetInstance().ATTACK_CREATURE);
	const char* HOLDTARGET_COOLDOWN = HOLDTARGET.c_str();

	std::string COMBO = std::to_string(Cooldowns::GetInstance().COMBO_BOT);
	const char * COMBO_COOLDOWN = COMBO.c_str();

	std::string DISCORD = std::to_string(Cooldowns::GetInstance().DISCORD_PLAYER_INFO_DELAY);
	const char* DISCORD_COOLDOWN = DISCORD.c_str();


	CreateGeneralGroupBox("Timers", 5, 0, 280, 175, hWindow); // 437

	CreateGeneralLabel("Spells:", 10, 20, 80, 20, hWindow);
	inputTimerHealingSpells.inputTime = CreateGeneralInputBox(HEAL_SPELL_COOLDOWN, 70, 20, 60, 20, hWindow);

	CreateGeneralLabel("Potions:", 10, 45, 80, 20, hWindow);
	inputTimerHealingItems.inputTime = CreateGeneralInputBox(POTIONS_COOLDOWN, 70, 45, 60, 20, hWindow);

	CreateGeneralLabel("Runes:", 10, 70, 80, 20, hWindow);
	inputTimerRunes.inputTime = CreateGeneralInputBox(RUNES_COOLDOWN, 70, 70, 60, 20, hWindow);

	CreateGeneralLabel("Dash:", 10, 95, 80, 20, hWindow);
	inputTimerDash.inputTime = CreateGeneralInputBox(DASH_COOLDOWN, 70, 95, 60, 20, hWindow);

	CreateGeneralLabel("Push:", 10, 120, 80, 20, hWindow);
	inputTimerPush.inputTime = CreateGeneralInputBox(PUSH_COOLDOWN, 70, 120, 60, 20, hWindow);

	CreateGeneralLabel("Equip:", 10, 145, 80, 20, hWindow);
	inputTimerEquip.inputTime = CreateGeneralInputBox(EQUIP_COOLDOWN, 70, 145, 60, 20, hWindow);

	CreateGeneralLabel("Drop item:", 140, 20, 100, 20, hWindow);
	inputTimerDrop.inputTime = CreateGeneralInputBox(DROP_COOLDOWN, 220, 20, 60, 20, hWindow);

	CreateGeneralLabel("Advertising:", 140, 45, 100, 20, hWindow);
	inputTimerAdvertising.inputTime = CreateGeneralInputBox(ADVERTISING_COOLDOWN, 220, 45, 60, 20, hWindow);

	CreateGeneralLabel("Hold Target:", 140, 70, 100, 20, hWindow);
	inputTimerHoldTarget.inputTime = CreateGeneralInputBox(HOLDTARGET_COOLDOWN, 220, 70, 60, 20, hWindow);

	CreateGeneralLabel("Combo Bot:", 140, 95, 100, 20, hWindow);
	inputTimerDiscord.inputTime = CreateGeneralInputBox(COMBO_COOLDOWN, 220, 95, 60, 20, hWindow);

	CreateGeneralLabel("Discord", 140, 120, 100, 20, hWindow);
	inputTimerDiscord.inputTime = CreateGeneralInputBox(DISCORD_COOLDOWN, 220, 120, 60, 20, hWindow);

	buttonCooldowns = CreateGeneralButton("Set new cooldowns", 140, 145, 125, 20, CLB_TIMERS, hWindow);


}

void Menu::CreateUtilsMenu(HWND hWindow)
{
	CreateGeneralGroupBox("Usefull Tools", 5, 0, 240, 300, hWindow);

	cBoxEnableAdvert = CreateGeneralCheckBox("Advertisting", 10, 25, 100, 20, CLB_ADVERTISING, hWindow);

	inputAdvertisingString = CreateGeneralInputBox(&advertisingMessage[0], 120, 25, 120, 20, hWindow);

	cBoxXray = CreateGeneralCheckBox("Xray", 10, 45, 100, 20, CLB_XRAY, hWindow);

	cBoxEnableAntiIdle = CreateGeneralCheckBox("Anti Idle", 10, 65, 100, 20, CLB_ANTI_IDLE, hWindow);

	cBoxEnableOutfitHack = CreateGeneralCheckBox("Auto Attack", 10, 85, 100, 20, CLB_AUTO_ATTACK, hWindow);

	cBoxEnableAdvancementScreenshot = CreateGeneralCheckBox("Advancement Screenshot", 10, 105, 185, 20, CLB_ADVANCEMENT_SCREENSHOT, hWindow);

	cBoxEnableEatFood = CreateGeneralCheckBox("Eat Food", 10, 125, 100, 20, CLB_FOOD_EAT, hWindow);

	cBoxLookIds = CreateGeneralCheckBox("Look Ids", 10, 145, 120, 20, CLB_LOOK_IDS, hWindow);

	cBoxEnableHoldPosition = CreateGeneralCheckBox("Hold Position", 10, 165, 120, 20, CLB_HOLD_POSITION, hWindow);

	cBoxEnableLightHack = CreateGeneralCheckBox("Light Hack", 10, 185, 140, 20, CLB_LIGHT_HACK, hWindow);

	cBoxComboBot = CreateGeneralCheckBox("Combo Bot", 10, 205, 100, 20, CLB_COMBO_BOT, hWindow);

	HWND leaderName = CreateGeneralLabel("Leader name: ", 10, 225, 100, 20, hWindow);

	SendMessage(leaderName, WM_SETFONT, (WPARAM)hFontNew, true);

	inputComboBot.inputLeaderName = CreateGeneralInputBox(&Hooks::comboBotSpells.leaderName[0], 120, 225, 100, 20, hWindow);

	HWND leaderSpell = CreateGeneralLabel("Leader spell: ", 10, 247, 100, 20, hWindow);

	SendMessage(leaderSpell, WM_SETFONT, (WPARAM)hFontNew, true);

	inputComboBot.inputLeaderSpell = CreateGeneralInputBox(&Hooks::comboBotSpells.leaderSpell[0], 120, 247, 100, 20, hWindow);

	HWND yourSpell = CreateGeneralLabel("Your spell: ", 10, 270, 100, 20, hWindow);

	SendMessage(yourSpell, WM_SETFONT, (WPARAM)hFontNew, true);

	inputComboBot.inputMySpell = CreateGeneralInputBox(&Hooks::comboBotSpells.mySpell[0], 120, 270, 100, 20, hWindow);
}

void Menu::CreatePvpMenu(HWND hWindow)
{
	std::string KILL_TARGET_HP_PERCENTAGE = std::to_string(killTarget.hpPercentage);
	char const* KHPPERC = KILL_TARGET_HP_PERCENTAGE.c_str();

	std::string KILL_TARGET_MANA_COST = std::to_string(killTarget.manaCost);
	char const* KTMCOST = KILL_TARGET_MANA_COST.c_str();

	std::string HASTE_MANA_COST = std::to_string(hasteSpell.manaCost);
	char const* HMCOST = HASTE_MANA_COST.c_str();

	std::string PARALYZE_SPELL_MANA_COST = std::to_string(cureParalayseSpell.manaCost);
	char const* PSMCOST = PARALYZE_SPELL_MANA_COST.c_str();


	CreateGeneralGroupBox("PVP Tools / Helpers", 5, 0, 410, 220, hWindow);

	cBoxEnableHoldTarget = CreateGeneralCheckBox("Hold Target", 10, 25, 100, 20, CLB_HOLD_TARGET, hWindow);

	cBoxEnableHoldDash = CreateGeneralCheckBox("Numpad Dash", 10, 45, 120, 20, CLB_DASH, hWindow);

	cBoxEnableMagicShield = CreateGeneralCheckBox("Magic Shield", 10, 65, 100, 20, CLB_MAGIC_SHIELD, hWindow);

	cBoxKillTarget = CreateGeneralCheckBox("Kill Target", 10, 85, 100, 20, CLB_KILL_ENEMY_ON_LOW_HP, hWindow);

	CreateGeneralLabel("HP %:", 10, 105, 100, 20, hWindow);

	inputTargetSpell.inputHpPerc = CreateGeneralInputBox(KHPPERC, 50, 105, 50, 20, hWindow);

	CreateGeneralLabel("MP:", 105, 105, 80, 20, hWindow);

	inputTargetSpell.inputMana = CreateGeneralInputBox(KTMCOST, 130, 105, 50, 20, hWindow);

	inputTargetSpell.inputSpell = CreateGeneralInputBox(&killTarget.spell[0], 185, 105, 115, 20, hWindow);

	cBoxEnableHaste = CreateGeneralCheckBox("Auto Haste", 10, 125, 100, 20, CLB_HASTE, hWindow);

	inputHasteSpell.inputSpell = CreateGeneralInputBox(&hasteSpell.spell[0], 10, 145, 121, 20, hWindow);

	CreateGeneralLabel("MP:", 135, 145, 30, 20, hWindow);
	inputHasteSpell.inputMana = CreateGeneralInputBox(HMCOST, 160, 145, 40, 20, hWindow);


	cBoxEnableAntiParalayse = CreateGeneralCheckBox("Anti Paralyze", 10, 165, 110, 20, CLB_ANTI_PARALAYSE, hWindow);
	inputCureParalayseSpell.inputSpell = CreateGeneralInputBox(&cureParalayseSpell.spell[0], 10, 185, 120, 20, hWindow);
	CreateGeneralLabel("MP:", 135, 185, 30, 20, hWindow);
	inputCureParalayseSpell.inputMana = CreateGeneralInputBox(PSMCOST, 160, 185, 40, 20, hWindow);







	CreateGeneralGroupBox("Amulet + Ring Balancer", 5, 220, 410, 140, hWindow);

	//cBoxEquipByHotkey = CreateGeneralCheckBox("Mode: hotkey", 168, 220, 107, 20, CLB_EQUIP_MODE_AS_HOTKEY, hWindow);
	cBoxEquipByMoveItem = CreateGeneralCheckBox("Mode: move item", 188, 220, 135, 20, CLB_EQUIP_MODE_AS_MOVE_ITEM, hWindow);

	lBoxAmulets = CreateGeneralComboBox("Amulet Options", 10, 245, 160, 70, CLB_LIST_BOX_AMULETS, hWindow);

	for (int32_t i = 0; i < sizeof(amuletOptions) / sizeof(amuletOptions[0]); i++)
	{
		SendMessageA(lBoxAmulets, CB_ADDSTRING, 0, (LPARAM)amuletOptions[i].name);
	}
	//SendMessage(lBoxAmulets, CB_SETCURSEL, selectedAmuletOption, (LPARAM)0);



	lBoxRings = CreateGeneralComboBox("Ring Options", 10, 300, 160, 70, CLB_LIST_BOX_RINGS, hWindow);

	for (int32_t i = 0; i < sizeof(ringOptions) / sizeof(ringOptions[0]); i++)
	{
		SendMessageA(lBoxRings, CB_ADDSTRING, 0, (LPARAM)ringOptions[i].name);
	}
}

void Menu::CreateAlarmsMenu(HWND hWindow)
{



	CreateGeneralGroupBox("Alarms", 5, 0, 299, 260, hWindow);

	cBoxAlarmEnemyOnScreen = CreateGeneralCheckBox("Enemy on screen", 10, 25, 140, 20, CLB_ENEMY_ON_SCREEN, hWindow);
	cBoxAlarmPlayerOnScreen = CreateGeneralCheckBox("Player on screen", 10, 45, 140, 20, CLB_PLAYER_ON_SCREEN, hWindow);
	cBoxAlarmMonsterOnScreen = CreateGeneralCheckBox("Monster on screen", 10, 65, 140, 20, CLB_MONSTER_ON_SCREEN, hWindow);
	cBoxAlarmSkullOnScreen = CreateGeneralCheckBox("Skull on screen", 10, 85, 140, 20, CLB_SKULL_ON_SCREEN, hWindow);
	cBoxAlarmLowMana = CreateGeneralCheckBox("Low mana", 10, 105, 100, 20, CLB_LOW_MANA, hWindow);
	cBoxFistFighting = CreateGeneralCheckBox("Fist Fighting", 10, 145, 100, 20, CLB_FIST_HIGHER_THAN, hWindow);
	cBoxDistanceFighting = CreateGeneralCheckBox("Distance Fighting", 10, 165, 150, 20, CLB_DISTANCE_FIHGTING_HIGHER_THAN, hWindow);
	cBoxShielding = CreateGeneralCheckBox("Shielding", 10, 185, 100, 20, CLB_SHIELDING_HIGHER_THAN, hWindow);
	cBoxSwordFighting = CreateGeneralCheckBox("Sword Fighting", 10, 205, 125, 20, CLB_SWORD_FIGHTING_HIGHER_THAN, hWindow);
	cBoxLevel = CreateGeneralCheckBox("Level", 10, 225, 125, 20, CLB_LEVEL, hWindow);
	CreateGeneralLabel("MP:", 120, 107, 60, 20, hWindow);
	inputLowMana.inputBox = CreateGeneralInputBox(&mpAlarm.spell[0], 150, 105, 50, 20, hWindow);
	cBoxAlarmHealth = CreateGeneralCheckBox("Low health", 10, 125, 100, 20, CLB_LOW_HEALTH, hWindow);
	CreateGeneralLabel("HP:", 120, 125, 60, 20, hWindow);
	inputLowHealth.inputBox = CreateGeneralInputBox(&hpAlarm.spell[0], 150, 125, 50, 20, hWindow);

	CreateGeneralLabel(" > ", 120, 150, 150, 20, hWindow);
	inputFistHigherThan.inputBox = CreateGeneralInputBox(&fistFightingAlarm.spell[0], 150, 145, 50, 20, hWindow);
	
	CreateGeneralLabel(" > ", 155, 165, 100, 20, hWindow);
	inputDistanceFightingHigherThan.inputBox = CreateGeneralInputBox(&distanceFightingAlarm.spell[0], 190, 165, 50, 20, hWindow);

	CreateGeneralLabel(" > ", 130, 187, 150, 20, hWindow);
	inputShieldingHigherThan.inputBox = CreateGeneralInputBox(&shieldingAlarm.spell[0], 190, 185, 50, 20, hWindow);

	CreateGeneralLabel(" > ", 130, 209, 150, 20, hWindow);
	inputSwordFightingHigherThan.inputBox = CreateGeneralInputBox(&swordFightingAlarm.spell[0], 190, 205, 50, 20, hWindow);

	CreateGeneralLabel(" > ", 130, 231, 150, 20, hWindow);
	inputLevelHigherThan.inputBox = CreateGeneralInputBox(&levelAlarm.spell[0], 190, 225, 50, 20, hWindow);

}


void Menu::CreateDiscordMenu(HWND hWindow)
{

	CreateGeneralGroupBox("Discord Notifications", 5, 0, 299, 260, hWindow); // 437

	cBoxDiscord = CreateGeneralCheckBox("", 10, 20, 15, 20, CLB_DISCORD, hWindow);

	CreateGeneralLabel("Discord Hook URL:", 30, 20, 150, 20, hWindow);
	inputDiscordHook.inputHook = CreateGeneralInputBox(&discordHook.hook[0], 150, 20, 120, 20, hWindow);
}

void Menu::CreateRuneMakerMenu(HWND hWindow)
{
	CreateGeneralGroupBox("Rune Maker", 5, 0, 299, 260, hWindow); // 437
	cBoxRuneMaker = CreateGeneralCheckBox("", 95, 0, 15, 20, CLB_RUNE_MAKER, hWindow);
	



	CreateGeneralLabel("Spell", 15, 25, 140, 20, hWindow);
	inputRMSpell.inputBox = CreateGeneralInputBox(&rmSpell.spell[0], 60, 25, 150, 20, hWindow);

	CreateGeneralLabel("Spell Mana", 15, 45, 140, 20, hWindow);
	inputRMSpellMana.inputBox = CreateGeneralInputBox(&rmSpell.mana[0], 100, 45, 50, 20, hWindow);

	CreateGeneralLabel("Spell Soul", 15, 65, 140, 20, hWindow);
	inputRMSpellSoul.inputBox = CreateGeneralInputBox(&rmSpell.soul[0], 100, 65, 50, 20, hWindow);

	CreateGeneralLabel("Rune Delay", 15, 85, 140, 20, hWindow);
	inputRMRuneDelay.inputBox = CreateGeneralInputBox(&rmSpell.runeDelay[0], 100, 85, 50, 20, hWindow);

	CreateGeneralLabel("Rune Id", 15, 105, 140, 20, hWindow);
	inputRMRuneId.inputBox = CreateGeneralInputBox(&rmSpell.runeId[0], 100, 105, 50, 20, hWindow);


}

void Menu::CreateAutoLogoutMenu(HWND hWindow)
{
	CreateGeneralGroupBox("Auto Logout", 5, 0, 299, 260, hWindow); // 437

	cBoxAutoLogoutEnemyOnScreen = CreateGeneralCheckBox("Enemy on screen", 10, 25, 140, 20, CLB_AUTO_LOGOUT_ENEMY_ON_SCREEN, hWindow);

	cBoxAutoLogoutPlayerOnScreen = CreateGeneralCheckBox("Player on screen", 10, 45, 140, 20, CLB_AUTO_LOGOUT_PLAYER_ON_SCREEN, hWindow);

	cBoxAutoLogoutSkullOnScreen = CreateGeneralCheckBox("Skull on screen", 10, 65, 140, 20, CLB_AUTO_LOGOUT_SKULL_ON_SCREEN, hWindow);

	cBoxAutoLogoutLowMana = CreateGeneralCheckBox("Mana  <", 10, 95, 80, 20, CLB_AUTO_LOGOUT_LOW_MANA, hWindow);
	inputALmana.inputBox = CreateGeneralInputBox(&autoLogout.mana[0], 100, 95, 60, 20, hWindow);

	cBoxAutoLogoutLowSoul = CreateGeneralCheckBox("Soul  <", 10, 120, 80, 20, CLB_AUTO_LOGOUT_LOW_SOUL, hWindow);
	inputALsoul.inputBox = CreateGeneralInputBox(&autoLogout.soul[0], 100, 120, 60, 20, hWindow);

	cBoxAutoLogoutLowCap = CreateGeneralCheckBox("Cap     <", 10, 145, 80, 20, CLB_AUTO_LOGOUT_LOW_CAP, hWindow);
	inputALcap.inputBox = CreateGeneralInputBox(&autoLogout.cap[0], 100, 145, 60, 20, hWindow);
}


void Menu::ChangeCooldown(const InputTimerLabel& timerLabel, int32_t& cooldown)
{
	GetWindowTextA(timerLabel.inputTime, &buf[0], sizeof(buf));
	cooldown = atoi(buf);
}

void Menu::ToggleSio()
{
	if (bHealFriendSio || bHealFriendUH)
	{
		if (GetWindowTextA(inputSioHp.inputHpPerc, &buf[0], sizeof(buf)))
		{
			healFriendOnHp = atoi(buf);
			sioUhPercent.friendHpPercentage = atoi(buf);
			if (GetWindowTextA(inputSioMyHp.inputHpPerc, &buf[0], sizeof(buf)))
			{
				safeHpForSio = atoi(buf);
				sioUhPercent.myHpPercentage = atoi(buf);
			}
			else
			{
				safeHpForSio = NULL;
			}
		}
		else
		{
			safeHpForSio = NULL;
			healFriendOnHp = NULL;
			sioUhPercent.myHpPercentage = NULL;
			sioUhPercent.friendHpPercentage = NULL;
		}
		if (selectedHealFriendMode == 0)
		{
			ReadSioListTxt(friendList);
		}
	}
	if (bHealFriendSio)
	{
		EnableWindow(inputSioHp.inputHpPerc, !bHealFriendSio);
		EnableWindow(inputSioMyHp.inputHpPerc, !bHealFriendSio);
		EnableWindow(cBoxHealFriendsUH, !bHealFriendSio);
		EnableWindow(buttonSioList, !bHealFriendSio);
		EnableWindow(lBoxHealFriendsOptions, !bHealFriendSio);
	}
	else if (bHealFriendUH)
	{
		EnableWindow(inputSioHp.inputHpPerc, !bHealFriendUH);
		EnableWindow(inputSioMyHp.inputHpPerc, !bHealFriendUH);
		EnableWindow(cBoxHealFriendsSio, !bHealFriendUH);
		EnableWindow(buttonSioList, !bHealFriendUH);
		EnableWindow(lBoxHealFriendsOptions, !bHealFriendUH);
	}
	else
	{
		EnableWindow(inputSioHp.inputHpPerc, true);
		EnableWindow(inputSioMyHp.inputHpPerc, true);
		EnableWindow(cBoxHealFriendsUH, true);
		EnableWindow(cBoxHealFriendsSio, true);
		EnableWindow(buttonSioList, true);
		EnableWindow(lBoxHealFriendsOptions, true);
	}
}

void Menu::ToggleHealer()
{
	if (bHealer)
	{
		lightSpell.spellLenght = GetWindowTextA(inputLightSpell.inputSpell, &lightSpell.spell[0], sizeof(lightSpell.spell));
		if (lightSpell.spellLenght)
		{
			GetWindowTextA(inputLightSpell.inputMana, &buf[0], sizeof(buf));
			lightSpell.manaCost = atoi(buf);
			GetWindowTextA(inputLightSpell.inputHpPerc, &buf[0], sizeof(buf));
			lightSpell.hpPercentage = atoi(buf);
		}
		else
		{
			lightSpell.manaCost = 0;
			lightSpell.hpPercentage = 0;
		}

		midSpell.spellLenght = GetWindowTextA(inputMidSpell.inputSpell, &midSpell.spell[0], sizeof(midSpell.spell));
		if (midSpell.spellLenght)
		{
			GetWindowTextA(inputMidSpell.inputMana, &buf[0], sizeof(buf));
			midSpell.manaCost = atoi(buf);
			GetWindowTextA(inputMidSpell.inputHpPerc, &buf[0], sizeof(buf));
			midSpell.hpPercentage = atoi(buf);
		}
		else
		{
			midSpell.manaCost = 0;
			midSpell.hpPercentage = 0;
		}

		heavySpell.spellLenght = GetWindowTextA(inputHeavySpell.inputSpell, &heavySpell.spell[0], sizeof(heavySpell.spell));
		if (heavySpell.spellLenght)
		{
			GetWindowTextA(inputHeavySpell.inputMana, &buf[0], sizeof(buf));
			heavySpell.manaCost = atoi(buf);
			GetWindowTextA(inputHeavySpell.inputHpPerc, &buf[0], sizeof(buf));
			heavySpell.hpPercentage = atoi(buf);
		}
		else
		{
			heavySpell.manaCost = 0;
			heavySpell.hpPercentage = 0;
		}

		if (GetWindowTextA(inputHealthPotion.inputItemId, &buf[0], sizeof(buf)))
		{
			healthPotionHealer.itemId = atoi(buf);

			if (GetWindowTextA(inputHealthPotion.inputHpPerc, &buf[0], sizeof(buf)))
			{
				healthPotionHealer.hpPerc = atoi(buf);
			}
			else
			{
				healthPotionHealer.hpPerc = NULL;
			}
		}
		else
		{
			healthPotionHealer.itemId = NULL;
		}

		if (GetWindowTextA(inputManaPotion.inputItemId, &buf[0], sizeof(buf)))
		{
			manaPotionHealer.itemId = atoi(buf);
			if (GetWindowTextA(inputManaPotion.inputMana, &buf[0], sizeof(buf)))
			{
				manaPotionHealer.manaPerc = atoi(buf);
			}
			else
			{
				manaPotionHealer.manaPerc = NULL;
			}
		}
		else
		{
			manaPotionHealer.itemId = NULL;
		}
	}
	EnableWindow(inputLightSpell.inputSpell, !bHealer);
	EnableWindow(inputMidSpell.inputSpell, !bHealer);
	EnableWindow(inputHeavySpell.inputSpell, !bHealer);

	EnableWindow(inputLightSpell.inputMana, !bHealer);
	EnableWindow(inputMidSpell.inputMana, !bHealer);
	EnableWindow(inputHeavySpell.inputMana, !bHealer);

	EnableWindow(inputLightSpell.inputHpPerc, !bHealer);
	EnableWindow(inputMidSpell.inputHpPerc, !bHealer);
	EnableWindow(inputHeavySpell.inputHpPerc, !bHealer);

	EnableWindow(inputHealthPotion.inputItemId, !bHealer);
	EnableWindow(inputHealthPotion.inputHpPerc, !bHealer);

	EnableWindow(inputManaPotion.inputItemId, !bHealer);
	EnableWindow(inputManaPotion.inputMana, !bHealer);
}

void Menu::ToggleTimers()
{
	ChangeCooldown(inputTimerHealingSpells, Cooldowns::GetInstance().HEAL_SPELL);
	ChangeCooldown(inputTimerHealingItems, Cooldowns::GetInstance().HEAL_ITEM);
	ChangeCooldown(inputTimerAdvertising, Cooldowns::GetInstance().SAY_ADVERTISING);
	ChangeCooldown(inputTimerComboBot, Cooldowns::GetInstance().COMBO_BOT);
	ChangeCooldown(inputTimerDash, Cooldowns::GetInstance().BOOST_DELAY);
	ChangeCooldown(inputTimerDrop, Cooldowns::GetInstance().DROP_ITEM);
	ChangeCooldown(inputTimerEquip, Cooldowns::GetInstance().EQUIP_ITEM);
	ChangeCooldown(inputTimerHoldTarget, Cooldowns::GetInstance().ATTACK_CREATURE);
	ChangeCooldown(inputTimerRunes, Cooldowns::GetInstance().ATTACK_ITEM);
	ChangeCooldown(inputTimerPush, Cooldowns::GetInstance().PUSH_CREATURE);
	ChangeCooldown(inputTimerDiscord, Cooldowns::GetInstance().DISCORD_PLAYER_INFO_DELAY);
}

void Menu::ToggleDiscord()
{
	if (bDiscord)
	{
		std::string str(&discordHook.hook[0]);
		GetWindowTextA(inputDiscordHook.inputHook, &discordHook.hook[0], sizeof(discordHook.hook));
		str = atoi(buf);
	}
	
	EnableWindow(inputDiscordHook.inputHook, !bDiscord);
}



void Menu::ToggleAdvertising()
{
	// lock input after activating module
	GetWindowTextA(inputAdvertisingString, &advertisingMessage[0], sizeof(advertisingMessage));
	EnableWindow(inputAdvertisingString, !bAdvertising);
}

void Menu::ToggleComboBot()
{
	if (Hooks::bComboBot)
	{
		bool inputLenghtLeaderName = GetWindowTextA(inputComboBot.inputLeaderName, &Hooks::comboBotSpells.leaderName[0], sizeof(Hooks::comboBotSpells.leaderName)) > 0;
		bool inputLenghtLeaderSpell = GetWindowTextA(inputComboBot.inputLeaderSpell, &Hooks::comboBotSpells.leaderSpell[0], sizeof(Hooks::comboBotSpells.leaderSpell)) > 0;
		bool inputLenghtMySpell = GetWindowTextA(inputComboBot.inputMySpell, &Hooks::comboBotSpells.mySpell[0], sizeof(Hooks::comboBotSpells.mySpell)) > 0;
		if (!inputLenghtLeaderName || !inputLenghtLeaderSpell || !inputLenghtMySpell)
		{
			Hooks::comboBotSpells.leaderName[0] = NULL;
			Hooks::comboBotSpells.leaderSpell[0] = NULL;
			Hooks::comboBotSpells.mySpell[0] = NULL;
		}
	}
	EnableWindow(inputComboBot.inputLeaderName, !Hooks::bComboBot);
	EnableWindow(inputComboBot.inputLeaderSpell, !Hooks::bComboBot);
	EnableWindow(inputComboBot.inputMySpell, !Hooks::bComboBot);
}



void Menu::ToggleKillTarget()
{
	if (bKillTarget)
	{
		killTarget.spellLenght = GetWindowTextA(inputTargetSpell.inputSpell, &killTarget.spell[0], sizeof(killTarget.spell));
		if (killTarget.spellLenght)
		{
			GetWindowTextA(inputTargetSpell.inputMana, &buf[0], sizeof(buf));
			killTarget.manaCost = atoi(buf);
			GetWindowTextA(inputTargetSpell.inputHpPerc, &buf[0], sizeof(buf));
			killTarget.hpPercentage = atoi(buf);
		}
		else
		{
			killTarget.manaCost = 0;
			killTarget.hpPercentage = 0;
		}
	}
	EnableWindow(inputTargetSpell.inputSpell, !bKillTarget);
	EnableWindow(inputTargetSpell.inputHpPerc, !bKillTarget);
	EnableWindow(inputTargetSpell.inputMana, !bKillTarget);
}

void Menu::ToggleHaste()
{
	if (bAutoHaste)
	{
		hasteSpell.spellLenght = GetWindowTextA(inputHasteSpell.inputSpell, &hasteSpell.spell[0], sizeof(hasteSpell.spell));
		if (hasteSpell.spellLenght)
		{
			GetWindowTextA(inputHasteSpell.inputMana, &buf[0], sizeof(buf));
			hasteSpell.manaCost = atoi(buf);
		}
		else
		{
			hasteSpell.manaCost = 0;
		}
	}
	EnableWindow(inputHasteSpell.inputSpell, !bAutoHaste);
	EnableWindow(inputHasteSpell.inputMana, !bAutoHaste);
}

void Menu::ToggleAntiParalayse()
{
	if (bAntiParalayse)
	{
		cureParalayseSpell.spellLenght = GetWindowTextA(inputCureParalayseSpell.inputSpell, &cureParalayseSpell.spell[0], sizeof(cureParalayseSpell.spell));
		if (cureParalayseSpell.spellLenght)
		{
			GetWindowTextA(inputCureParalayseSpell.inputMana, &buf[0], sizeof(buf));
			cureParalayseSpell.manaCost = atoi(buf);
		}
		else
		{
			cureParalayseSpell.manaCost = 0;
		}
	}
	EnableWindow(inputCureParalayseSpell.inputSpell, !bAntiParalayse);
	EnableWindow(inputCureParalayseSpell.inputMana, !bAntiParalayse);
}


void Menu::ToggleHpAlarm()
{
	if (bLowHealth)
	{
		hpAlarm.spellLenght = GetWindowTextA(inputLowHealth.inputBox, &hpAlarm.spell[0], sizeof(hpAlarm.spell));
		if (hpAlarm.spellLenght)
		{
			GetWindowTextA(inputLowHealth.inputBox, &buf[0], sizeof(buf));
			hpAlarm.hpPercentage = atoi(buf);
		}
		else
		{
			hpAlarm.hpPercentage = 0;
		}
	}

	EnableWindow(inputLowHealth.inputBox, !bLowHealth);
}

void Menu::ToggleMpAlarm()
{
	if (bLowMana)
	{
		mpAlarm.spellLenght = GetWindowTextA(inputLowMana.inputBox, &mpAlarm.spell[0], sizeof(mpAlarm.spell));
		if (mpAlarm.spellLenght)
		{
			GetWindowTextA(inputLowMana.inputBox, &buf[0], sizeof(buf));
			mpAlarm.mpAlarm = atoi(buf);
		}
		else
		{
			mpAlarm.mpAlarm = 0;
		}
	}

	EnableWindow(inputLowMana.inputBox, !bLowMana);
}

void Menu::ToggleFistFightingAlarm()
{
	if (bFistFightingHigherThan)
	{
		fistFightingAlarm.spellLenght = GetWindowTextA(inputFistHigherThan.inputBox, &fistFightingAlarm.spell[0], sizeof(fistFightingAlarm.spell));
		if (fistFightingAlarm.spellLenght)
		{
			GetWindowTextA(inputFistHigherThan.inputBox, &buf[0], sizeof(buf));
			fistFightingAlarm.fistFightingAlarm = atoi(buf);
		}
		else
		{
			fistFightingAlarm.fistFightingAlarm = 0;
		}
	}

	EnableWindow(inputFistHigherThan.inputBox, !bFistFightingHigherThan);
}

void Menu::ToggleDistanceFightingAlarm()
{
	if (bDistanceFightingHigherThan)
	{
		distanceFightingAlarm.spellLenght = GetWindowTextA(inputDistanceFightingHigherThan.inputBox, &distanceFightingAlarm.spell[0], sizeof(distanceFightingAlarm.spell));
		if (distanceFightingAlarm.spellLenght)
		{
			GetWindowTextA(inputDistanceFightingHigherThan.inputBox, &buf[0], sizeof(buf));
			distanceFightingAlarm.distanceFightingAlarm = atoi(buf);
		}
		else
		{
			distanceFightingAlarm.distanceFightingAlarm = 0;
		}
	}

	EnableWindow(inputDistanceFightingHigherThan.inputBox, !bDistanceFightingHigherThan);
}

void Menu::ToggleShieldingAlarm()
{
	if (bShieldingHigherThan)
	{
		shieldingAlarm.spellLenght = GetWindowTextA(inputShieldingHigherThan.inputBox, &shieldingAlarm.spell[0], sizeof(shieldingAlarm.spell));
		if (distanceFightingAlarm.spellLenght)
		{
			GetWindowTextA(inputShieldingHigherThan.inputBox, &buf[0], sizeof(buf));
			shieldingAlarm.shieldingAlarm = atoi(buf);
		}
		else
		{
			shieldingAlarm.shieldingAlarm = 0;
		}
	}

	EnableWindow(inputShieldingHigherThan.inputBox, !bShieldingHigherThan);
}

void Menu::ToggleSwordFightingAlarm()
{
	if (bSwordFightingHigherThan)
	{
		swordFightingAlarm.spellLenght = GetWindowTextA(inputSwordFightingHigherThan.inputBox, &swordFightingAlarm.spell[0], sizeof(swordFightingAlarm.spell));
		if (distanceFightingAlarm.spellLenght)
		{
			GetWindowTextA(inputShieldingHigherThan.inputBox, &buf[0], sizeof(buf));
			swordFightingAlarm.swordFightingAlarm = atoi(buf);
		}
		else
		{
			swordFightingAlarm.swordFightingAlarm = 0;
		}
	}

	EnableWindow(inputSwordFightingHigherThan.inputBox, !bSwordFightingHigherThan);
}

void Menu::ToggleLevelAlarm()
{
	if (bLevelHigherThan)
	{
		levelAlarm.spellLenght = GetWindowTextA(inputLevelHigherThan.inputBox, &levelAlarm.spell[0], sizeof(levelAlarm.spell));
		if (distanceFightingAlarm.spellLenght)
		{
			GetWindowTextA(inputShieldingHigherThan.inputBox, &buf[0], sizeof(buf));
			levelAlarm.level = atoi(buf);
		}
		else
		{
			levelAlarm.level = 0;
		}
	}

	EnableWindow(inputLevelHigherThan.inputBox, !bLevelHigherThan);
}

void Menu::ToggleRuneMaker()
{
	if (bRuneMaker)
	{
		rmSpell.spellLenght = GetWindowTextA(inputRMSpell.inputBox, &rmSpell.spell[0], sizeof(rmSpell.spell));
		if (rmSpell.spellLenght)
		{
			GetWindowTextA(inputRMSpell.inputBox, &buf[0], sizeof(buf));
			strcpy_s(rmSpell.spell, buf);
			GetWindowTextA(inputRMSpellMana.inputBox, &buf[0], sizeof(buf));
			strcpy_s(rmSpell.mana, buf);
			GetWindowTextA(inputRMSpellSoul.inputBox, &buf[0], sizeof(buf));
			strcpy_s(rmSpell.soul, buf);
			GetWindowTextA(inputRMRuneDelay.inputBox, &buf[0], sizeof(buf));
			strcpy_s(rmSpell.runeDelay, buf);
			GetWindowTextA(inputRMRuneId.inputBox, &buf[0], sizeof(buf));
			strcpy_s(rmSpell.runeId, buf);
		}
		else
		{
			strcpy_s(rmSpell.spell, 0);
			strcpy_s(rmSpell.mana, 0);
			strcpy_s(rmSpell.soul, 0);
			strcpy_s(rmSpell.runeDelay, 0);
			strcpy_s(rmSpell.runeId, 0);
		}
	}
	EnableWindow(inputRMSpell.inputBox, !bRuneMaker);
	EnableWindow(inputRMSpellMana.inputBox, !bRuneMaker);
	EnableWindow(inputRMSpellSoul.inputBox, !bRuneMaker);
	EnableWindow(inputRMRuneDelay.inputBox, !bRuneMaker);
	EnableWindow(inputRMRuneId.inputBox, !bRuneMaker);
}

void Menu::ToggleAutoLogout()
{
	autoLogout.manaLength = GetWindowTextA(inputALmana.inputBox, &autoLogout.mana[0], sizeof(autoLogout.mana));
	autoLogout.soulLength = GetWindowTextA(inputALsoul.inputBox, &autoLogout.soul[0], sizeof(autoLogout.soul));
	autoLogout.capLength = GetWindowTextA(inputALcap.inputBox, &autoLogout.cap[0], sizeof(autoLogout.cap));

	if (autoLogout.manaLength)
	{
		GetWindowTextA(inputALmana.inputBox, &buf[0], sizeof(buf));
		strcpy_s(autoLogout.mana, buf);
	}
	else
	{
		strcpy_s(autoLogout.mana, 0);
	}

	if (autoLogout.soulLength)
	{
		GetWindowTextA(inputALsoul.inputBox, &buf[0], sizeof(buf));
		strcpy_s(autoLogout.soul, buf);
	}
	else
	{
		strcpy_s(autoLogout.soul, 0);
	}

	if (autoLogout.capLength)
	{
		GetWindowTextA(inputALcap.inputBox, &buf[0], sizeof(buf));
		strcpy_s(autoLogout.cap, buf);
	}
	else
	{
		strcpy_s(autoLogout.cap, 0);
	}


	EnableWindow(inputALmana.inputBox, !bAutoLogoutLowMana);
	EnableWindow(inputALsoul.inputBox, !bAutoLogoutLowSoul);
	EnableWindow(inputALcap.inputBox, !bAutoLogoutLowCap);
}



void Menu::ToggleItemBalancer()
{
	if (bEquipModeHotkey || bEquipModeMoveItem)
	{
		if (bLoadedAmulets)
		{
			if (GetWindowTextA(inputHoldAmulet.inputItemId, &buf[0], sizeof(buf)))
			{
				holdAmulet.Id = atoi(buf);
				if (GetWindowTextA(inputHoldAmulet.inputHpPerc, &buf[0], sizeof(buf)))
				{
					holdAmulet.hpPercentage = atoi(buf);

					///////
					balanceAmulet.hpPercentage = NULL;
					balanceAmulet.Id = NULL;
					dequipAmulet.hpPercentage = NULL;
					dequipAmulet.Id = NULL;
				}
				else
				{
					holdAmulet.hpPercentage = NULL;
				}
			}
			else
			{
				holdAmulet.hpPercentage = NULL;
				holdAmulet.Id = NULL;
			}
			switch (selectedAmuletOption)
			{
			case 1:
				if (GetWindowTextA(inputDequipAmulet.inputItemId, &buf[0], sizeof(buf)))
				{
					dequipAmulet.Id = atoi(buf);
					dequipAmulet.hpPercentage = holdAmulet.hpPercentage;

					///////
					balanceAmulet.hpPercentage = NULL;
					balanceAmulet.Id = NULL;
				}
				else
				{
					dequipAmulet.hpPercentage = NULL;
					dequipAmulet.Id = NULL;
				}
				break;
			case 2:
				if (GetWindowTextA(inputBalanceAmulet.inputItemId, &buf[0], sizeof(buf)))
				{
					balanceAmulet.Id = atoi(buf);
					balanceAmulet.hpPercentage = holdAmulet.hpPercentage;

					///////
					dequipAmulet.hpPercentage = NULL;
					dequipAmulet.Id = NULL;
				}
				else
				{
					balanceAmulet.hpPercentage = NULL;
					balanceAmulet.Id = NULL;
				}
				break;
			}
		}
		if (bLoadedRings)
		{
			if (GetWindowTextA(inputHoldRing.inputItemId, &buf[0], sizeof(buf)))
			{
				holdRing.Id = atoi(buf);
				if (GetWindowTextA(inputHoldRing.inputHpPerc, &buf[0], sizeof(buf)))
				{
					holdRing.hpPercentage = atoi(buf);
					holdRing.manaPerc = 0;

					balanceRing.hpPercentage = NULL;
					balanceRing.Id = NULL;
					balanceRing.hpPercentage = NULL;
					balanceRing.Id = NULL;
				}
				else
				{
					holdRing.hpPercentage = NULL;
				}
			}
			else
			{
				holdRing.hpPercentage = NULL;
				holdRing.Id = NULL;
			}
			switch (selectedRingsOption)
			{
			case 1:
				if (GetWindowTextA(inputDequipRing.inputItemId, &buf[0], sizeof(buf)))
				{
					dequipRing.Id = atoi(buf);
					dequipRing.hpPercentage = holdRing.hpPercentage;

					balanceRing.hpPercentage = NULL;
					balanceRing.Id = NULL;
				}
				else
				{
					dequipRing.hpPercentage = NULL;
					dequipRing.Id = NULL;
				}
				break;
			case 2:
				if (GetWindowTextA(inputBalanceRing.inputItemId, &buf[0], sizeof(buf)))
				{
					balanceRing.Id = atoi(buf);
					balanceRing.hpPercentage = holdRing.hpPercentage;
					balanceRing.manaPerc = 0;

					dequipRing.hpPercentage = NULL;
					dequipRing.Id = NULL;
				}
				else
				{
					balanceRing.hpPercentage = NULL;
					balanceRing.Id = NULL;
				}
				break;
			}
		}
		EnableWindow(cBoxEquipByMoveItem, !bEquipModeHotkey);
		EnableWindow(cBoxEquipByHotkey, !bEquipModeMoveItem);
		if (bLoadedAmulets)
		{
			EnableWindow(inputHoldAmulet.inputItemId, !bEquipModeHotkey);
			EnableWindow(inputHoldAmulet.inputHpPerc, !bEquipModeHotkey);
			switch (selectedAmuletOption)
			{
			case 1:
				EnableWindow(inputDequipAmulet.inputItemId, !bEquipModeHotkey);
				break;

			case 2:
				EnableWindow(inputBalanceAmulet.inputItemId, !bEquipModeHotkey);
				break;
			}
		}
		if (bLoadedRings)
		{
			EnableWindow(inputHoldRing.inputItemId, !bEquipModeHotkey);
			EnableWindow(inputHoldRing.inputHpPerc, !bEquipModeHotkey);
			switch (selectedRingsOption)
			{
			case 1:
				EnableWindow(inputDequipRing.inputItemId, !bEquipModeHotkey);
				break;

			case 2:
				EnableWindow(inputBalanceRing.inputItemId, !bEquipModeHotkey);
				break;
			}
		}
	}
	bool state = bEquipModeHotkey || bEquipModeMoveItem;
	EnableWindow(cBoxEquipByMoveItem, !bEquipModeHotkey);
	EnableWindow(cBoxEquipByHotkey, !bEquipModeMoveItem);
	EnableWindow(lBoxAmulets, !state);
	EnableWindow(lBoxRings, !state);
	EnableWindow(inputHoldAmulet.inputHpPerc, !state);
	EnableWindow(inputHoldAmulet.inputItemId, !state);
	EnableWindow(inputDequipAmulet.inputItemId, !state);
	EnableWindow(inputBalanceAmulet.inputItemId, !state);
	EnableWindow(inputHoldRing.inputItemId, !state);
	EnableWindow(inputHoldRing.inputHpPerc, !state);
	EnableWindow(inputDequipRing.inputItemId, !state);
	EnableWindow(inputBalanceRing.inputItemId, !state);
}