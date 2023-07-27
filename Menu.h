#pragma once
#include <Windows.h>
#include <string>





#define AUTO_CONFIG_LOAD (97)
#define CONFIG_SAVE (98)
#define CONFIG_LOAD (99)
#define MYMENU_EXIT (100)
#define CLB_HEALER (101)
#define CLB_HASTE (102)
#define CLB_ANTI_PARALAYSE (103)
#define CLB_LOOK_IDS (104)
#define CLB_HOLD_TARGET (105)
#define CLB_EQUIP_MODE_AS_HOTKEY (106)
#define CLB_EQUIP_MODE_AS_MOVE_ITEM (107)
#define CLB_DASH (110)
#define CLB_OPEN_CONSOLE (111)
#define CLB_ADVERTISING (112)
#define CLB_ANTI_IDLE (113)
#define CLB_XRAY (114)
#define CLB_SIO (115)
#define CLB_LIGHT (116)
#define CLB_MOUNT (117)
#define CLB_FOOD_EAT (118)
#define CLB_MAGIC_SHIELD (119)
#define CLB_HOLD_POSITION (120)
#define CLB_RAINBOW (121)
#define CLB_LIGHT_HACK (122)
#define CLB_DISABLE_FRAMES (123)
#define CLB_AUTO_ATTACK (124)
#define CLB_STAND_HASTE (125)
#define CLB_MW_TIMER_DYNAMIC (126)
#define CLB_HEAL_FRIENDS (128)
#define CLB_HEAL_FRIENDS_SIO (129)
#define CLB_HEAL_FRIENDS_UH (130)
#define CLB_HEAL_FRIENDS_PARTY (131)
#define CLB_HEAL_FRIENDS_WAR (132)
#define CLB_MW_TIMER_MS (134)
#define CLB_MW_TIMER_STATIC (135)
#define CLB_FREEZE_MAGIC_WALL (136)
#define CLB_MANA_BAR (143)
#define CLB_SHORTKEYS (137)
#define CLB_OPEN_SIO_LIST (138)
#define CLB_LIST_BOX_AMULETS (139)
#define CLB_LIST_BOX_RINGS (140)
#define CLB_COMBO_BOT (141)
#define CLB_TIMERS (142)
#define CLB_LIST_BOX_HEAL_MODES (143)
#define CLB_KILL_ENEMY_ON_LOW_HP (144)
#define CLB_ENEMY_ON_SCREEN (145)
#define CLB_PLAYER_ON_SCREEN (146)
#define CLB_MONSTER_ON_SCREEN (147)
#define CLB_SKULL_ON_SCREEN (148)
#define CLB_LOW_MANA (149)
#define CLB_LOW_HEALTH (150)
#define CLB_INVITE_TO_PARTY (151)
#define CLB_ENABLE_SHARED_EXP (152)
#define CLB_PASS_LEADERSHIP (153)
#define CLB_LIST_BOX_INVITE_PARTY_MODES (154)
#define CLB_FIST_HIGHER_THAN (155)
#define CLB_DISTANCE_FIHGTING_HIGHER_THAN (156)
#define CLB_SHIELDING_HIGHER_THAN (157)
#define CLB_SWORD_FIGHTING_HIGHER_THAN (158)
#define CLB_LEVEL (159)
#define CLB_AXE_FIGHTING_HIGHER_THAN (160)
#define CLB_CLUB_FIGHTING_HIGHER_THAN (157)
#define CLB_DISCORD (161)
#define CLB_RUNE_MAKER (162)

#define CLB_AUTO_LOGOUT_ENEMY_ON_SCREEN (163)
#define CLB_AUTO_LOGOUT_PLAYER_ON_SCREEN (164)
#define CLB_AUTO_LOGOUT_SKULL_ON_SCREEN (165)

#define CLB_AUTO_LOGOUT_LOW_MANA (166)
#define CLB_AUTO_LOGOUT_LOW_SOUL (167)
#define CLB_AUTO_LOGOUT_LOW_CAP (168)

#define CLB_OPEN_HEALER_WINDOW (1000)
#define CLB_OPEN_TIMERS_WINDOW (1001)
#define CLB_OPEN_UTILS_WINDOW (1002)
#define CLB_OPEN_PVP_WINDOW (1003)
#define CLB_OPEN_SHORTKEYS_WINDOW (1004)
#define CLB_OPEN_ALARMS_WINDOW (1005)
#define CLB_OPEN_DISCORD_WINDOW (1006)
#define CLB_OPEN_RUNE_MAKER_WINDOW (1007)
#define CLB_OPEN_AUTO_LOGOUT_WINDOW (1008)

struct InputSpellLabel
{
	HWND inputSpell;
	HWND inputMana;
	HWND inputHpPerc;
};

struct InputItemLabel
{
	HWND inputItemId;
	HWND inputManaPerc;
	HWND inputHpPerc;
	HWND boolEquipDelay;
};

struct InputTimerLabel
{
	HWND inputTime;
};

struct InputDiscordLabel
{
	HWND inputHook;
};

struct InputRuneMakerLabel
{
	HWND inputBox;
};

struct InputAutoLogoutLabel
{
	HWND inputBox;
};


struct InputItemHealer
{
	HWND inputItemId;
	HWND inputMana;
	HWND inputHpPerc;
};

struct InputLowInfo
{
	HWND inputBox;
	int32_t lowInfo;
};

struct InputComboBot
{
	HWND inputLeaderName;
	HWND inputLeaderSpell;
	HWND inputMySpell;
};

namespace Menu
{

	static CSelfCharacter selfCharacter;
	static uintptr_t moduleBase = (uintptr_t)GetModuleHandle(NULL);
	static CSelfCharacter selfPlayer;
	static CSelfCharacter localPlayer;
	static Skills skills;

	static FILE* f;

	BOOL RegisterDLLWindowClass(const wchar_t szClassName[]);
	void RegisterDLLWindowHealerClass();
	void RegisterDLLWindowTimersClass();
	void RegisterDLLWindowUtilsClass();
	void RegisterDLLWindowPvpClass();
	void RegisterDLLWindowAlarmsClass();
	void RegisterDLLWindowDiscordClass();
	void RegisterDLLWindowRuneMakerClass();
	void RegisterDLLWindowAutoLogoutClass();
	HMENU CreateDLLWindowMenu();
	LRESULT CALLBACK MessageHandler(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam);
	void CALLBACK MainTimerLoop(HWND hwnd, UINT uMsg, int32_t timerId, DWORD dwTime);

	void Init(HMODULE hModule);
	void HandleMsgs();
	void ToggleHealer();
	void ToggleTimers();
	void ToggleDiscord();
	void ToggleSio();
	void ToggleAdvertising();

	void ToggleComboBot();
	void ToggleKillTarget();
	void ToggleHaste();
	void ToggleAntiParalayse();
	void ToggleItemBalancer();
	void ToggleHpAlarm();
	void ToggleMpAlarm();
	void ToggleFistFightingAlarm();
	void ToggleDistanceFightingAlarm();
	void ToggleShieldingAlarm();
	void ToggleSwordFightingAlarm();
	void ToggleLevelAlarm();
	void ToggleRuneMaker();
	void ToggleAutoLogout();

	HWND CreateButton(const char* lpWindowName, int32_t x, int32_t y, int32_t iWidth, int32_t iHeight, int32_t hMenu);


	HWND CreateGeneralCheckBox(const char* lpWindowName, int32_t x, int32_t y, int32_t iWidth, int32_t iHeight, int32_t hMenu, HWND hWnd);
	HWND CreateGeneralButton(const char* lpWindowName, int32_t x, int32_t y, int32_t iWidth, int32_t iHeight, int32_t hMenu, HWND hWnd);
	HWND CreateGeneralLabel(const char* lpWindowName, int32_t x, int32_t y, int32_t iWidth, int32_t iHeight, HWND hWnd);
	HWND CreateGeneralGroupBox(const char* lpWindowName, int32_t x, int32_t y, int32_t iWidth, int32_t iHeight, HWND hWnd);
	HWND CreateGeneralListBox(const char* lpWindowName, int32_t x, int32_t y, int32_t iWidth, int32_t iHeight, int32_t hMenu, HWND hWnd);
	HWND CreateGeneralInputBox(const char* lpWindowName, int32_t x, int32_t y, int32_t iWidth, int32_t iHeight, HWND hWnd);
	HWND CreateGeneralComboBox(const char* lpWindowName, int32_t x, int32_t y, int32_t iWidth, int32_t iHeight, int32_t hMenu, HWND hWnd);

	void CreateMainMenu();
	void CreateHealerMenu(HWND hWindow);
	void CreateTimersMenu(HWND hWindow);
	void CreateUtilsMenu(HWND hWindow);
	void CreatePvpMenu(HWND hWindow);
	void CreateAlarmsMenu(HWND hWindow);
	void CreateDiscordMenu(HWND hWindow);
	void CreateRuneMakerMenu(HWND hWindow);
	void CreateAutoLogoutMenu(HWND hWindow);
	void ChangeCooldown(const InputTimerLabel& timerLabel, int32_t& cooldown);
	void OpenFileExplorer(HWND hwnd);
	void LoadConfig(LPWSTR filePath);
	void AutoLoadConfig();
	void SaveConfig();
	void OpenSioListTxt();
	void ReadSioListTxt(std::array<std::string, 100>& friendList);
	LRESULT CALLBACK HealerMessageHandler(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam);

	LRESULT CALLBACK TimersMessageHandler(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK UtilsMessageHandler(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK PvpMessageHandler(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK AlarmsMessageHandler(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK DiscordMessageHandler(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK RuneMakerMessageHandler(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK AutoLogoutMessageHandler(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam);
	static HWND lBoxAmulets;
	static HWND lBoxRings;

	// Global variables
	const int32_t mainWindowWidth = 456;
	const int32_t mainWindowHeight = 115;

	static HMODULE inj_hModule;
	static HWND mainHWND;
	static HWND healerHWND;
	static HWND timersHWND;
	static HWND utilsHWND;
	static HWND pvpHWND;
	static HWND alarmsHWND;
	static HWND discordHWND;
	static HWND runeMakerHWND;
	static HWND autoLogoutHWND;
	static MSG messages;
	const HFONT hFont = CreateFontA(16,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		("Arial"));                 // lpszFacename

	const HFONT hFontNew = CreateFontA(16,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		("Arial"));                 // lpszFacename





	static HWND cBoxKillTarget;
	static InputSpellLabel inputTargetSpell;


	static HWND buttonSioList;
	static HWND buttonHealer;
	static HWND buttonTimers;
	static HWND buttonUtils;
	static HWND buttonPvp;
	static HWND buttonAlarms;
	static HWND buttonDiscord;
	static HWND buttonRuneMaker;
	static HWND buttonAutoLogout;

	static HWND cBoxHealFriendsSio;
	static HWND cBoxHealFriendsUH;
	static HWND lBoxHealFriendsOptions;

	static InputItemLabel inputHoldAmulet;
	static InputItemLabel inputDequipAmulet;
	static InputItemLabel inputBalanceAmulet;
	static ExItem holdAmulet;
	static ExItem dequipAmulet;
	static ExItem balanceAmulet;

	static InputItemLabel inputHoldRing;
	static InputItemLabel inputDequipRing;
	static InputItemLabel inputBalanceRing;
	static ExItem holdRing;
	static ExItem dequipRing;
	static ExItem balanceRing;


	static InputLowInfo inputLowMana;
	static InputLowInfo inputLowHealth;
	static InputLowInfo inputFistHigherThan;
	static InputLowInfo inputDistanceFightingHigherThan;
	static InputLowInfo inputShieldingHigherThan;
	static InputLowInfo inputSwordFightingHigherThan;
	static InputLowInfo inputLevelHigherThan;
	static InputLowInfo inputAxeFightingHigherThan;
	static InputLowInfo inputClubFightingHigherThan;

	static InputSpellLabel inputLightSpell;
	static InputSpellLabel inputMidSpell;
	static InputSpellLabel inputHeavySpell;

	static InputItemHealer inputHealthPotion;
	static InputItemHealer inputManaPotion;

	static HealthPotionItemHealer healthPotionHealer;
	static ManaPotionItemHealer manaPotionHealer;


	static InputTimerLabel inputTimerHealingSpells;
	static InputTimerLabel inputTimerHealingItems;
	static InputTimerLabel inputTimerRunes;
	static InputTimerLabel inputTimerDash;
	static InputTimerLabel inputTimerPush;
	static InputTimerLabel inputTimerEquip;

	//Discord input
	static InputDiscordLabel inputDiscordHook;


	//RuneMaker input
	static InputRuneMakerLabel inputRMSpell;
	static InputRuneMakerLabel inputRMSpellMana;
	static InputRuneMakerLabel inputRMSpellSoul;
	static InputRuneMakerLabel inputRMRuneDelay;
	static InputRuneMakerLabel inputRMRuneId;

	static InputRuneMakerLabel inputRMCustomSpell;
	static InputRuneMakerLabel inputRMCustomSpellMana;


	//AutoLoogut input
	static InputAutoLogoutLabel inputALmana;
	static InputAutoLogoutLabel inputALsoul;
	static InputAutoLogoutLabel inputALcap;




	static InputSpellLabel inputSioHp;
	static InputSpellLabel inputSioMyHp;

	static InputTimerLabel inputTimerDrop;
	static InputTimerLabel inputTimerAdvertising;
	static InputTimerLabel inputTimerHoldTarget;
	static InputTimerLabel inputTimerComboBot;
	static InputTimerLabel inputTimerDiscord;

	static InputSpellLabel inputHasteSpell;
	static InputSpellLabel inputCureParalayseSpell;


	static char buf[30];
	static char advertisingMessage[255] = "Your message.";

	static int32_t safeHpForSio;
	static int32_t healFriendOnHp;



	static LightSpell lightSpell;
	static MidSpell midSpell;
	static HeavySpell heavySpell;
	static SioUhPercent sioUhPercent;
	static KillTarget killTarget;
	static HasteSpell hasteSpell;
	static CureParalayseSpell cureParalayseSpell;




	//ALARMS

	static AlarmHp hpAlarm;
	static AlarmMp mpAlarm;
	static AlarmFistFighting fistFightingAlarm;
	static AlarmDistanceFighting distanceFightingAlarm;
	static AlarmShielding shieldingAlarm;
	static AlarmSwordFighting swordFightingAlarm;
	static AlarmLevel levelAlarm;


	//Discord
	static DiscordHook discordHook;

	//RuneMaker

	static RuneMakerSpell rmSpell;

	// AutoLogout
	
	static AutoLogout autoLogout;








	static bool bHealerWindowOpen = false;
	static bool bTimersWindowOpen = false;
	static bool bUtilsWindowOpen = false;
	static bool bPvpWindowOpen = false;
	static bool bAlarmsWindowOpen = false;
	static bool bDiscordWindowOpen = false;
	static bool bRuneMakerWindowOpen = false;
	static bool bAutoLogoutWindowOpen = false;

	static bool bAutoAttackState = false;
	static bool bHealer = false;
	static bool bRainbowOutfit = false;
	static bool bAutoLoot = false;
	static bool bHealSio = false;
	static bool bAutoHaste = false;
	static bool bXray = false;
	static bool bAntiParalayse = false;
	static bool bHoldTarget = false;
	static bool bDash = false;
	static bool bConsole = false;
	static bool bAdvertising = false;
	static bool bAntiIdle = false;
	static bool bActivated = false;
	static bool bHealFriendSio = false;
	static bool bHealFriendUH = false;
	static bool bEquipModeHotkey = false;
	static bool bEquipModeMoveItem = false;
	static bool bMount = false;
	static bool bEnableLightHack = false;
	static bool bEatFood = false;
	static bool bHoldPosition = false;
	static bool bManaShield = false;
	static bool bAutoAttack = false;
	static bool bLoadedAmulets = false;
	static bool bLoadedRings = false;
	static bool bFreezeMagicWalls = false;
	static bool bKillTarget = false;
	static bool bDiscord = false;
	static bool bRuneMaker = false;



	static bool bEnemyOnScreen = false;
	static bool bPlayerOnScreen = false;
	static bool bMonsterOnScreen = false;
	static bool bSkullOnScreen = false;
	static bool bLowHealth = false;
	static bool bLowMana = false;


	static bool bFistFightingHigherThan = false;
	static bool bDistanceFightingHigherThan = false;
	static bool bShieldingHigherThan = false;
	static bool bSwordFightingHigherThan = false;
	static bool bLevelHigherThan = false;
	static bool bAxeFightingHigherThan = false;
	static bool bClubFightingHigherThan = false;


	static bool bAutoLogoutEnemyOnScreen = false;
	static bool bAutoLogoutPlayerOnScreen = false;
	static bool bAutoLogoutSkullOnScreen = false;

	static bool bAutoLogoutLowMana = false;
	static bool bAutoLogoutLowSoul = false;
	static bool bAutoLogoutLowCap = false;

	//Buttons
	static HWND buttonCooldowns;



	static HWND itemsToDeleteAmulets[3] = { NULL, NULL, NULL };
	static HWND itemsToDeleteRings[3] = { NULL, NULL, NULL };

	static BalanceItemOptions amuletOptions[] = { {"Equip & Hold Amulet", 0}, {"Equip/Dequip Amulet", 1}, {"Balance Amulet", 2} };

	static int32_t selectedAmuletOption = -1, selectedRingsOption = -1;

	static BalanceItemOptions ringOptions[] = { {"Equip & Hold Ring", 0}, {"Equip/Dequip Ring", 1}, {"Balance Ring", 2} };


	static int32_t selectedHealFriendMode = 0;

	static BalanceItemOptions healFriendOptions[] = { {"List", 0}, {"Guild", 1}, {"Party", 2}, {"Hearts", 3} };

	static int32_t selectedPartyInviteMode = 0;

	static BalanceItemOptions partyInviteOptions[] = { {"Guild", 0}, {"Hearts", 1}, {"Message", 2} };

	static std::array<std::string, 100> friendList = {};

	static HWND cBoxEnableHealer;
	static HWND cBoxEnableHaste;
	static HWND cBoxEnableAntiParalayse;
	static HWND cBoxEnableHoldTarget;
	static HWND cBoxEnableHoldDash;
	static HWND cBoxEnableConsole;
	static HWND cBoxEnableAdvert;
	static HWND cBoxEnableAntiIdle;
	static HWND cBoxEnableLightHack;
	static HWND cBoxEnableAutoMount;
	static HWND cBoxEnableEatFood;
	static HWND cBoxEnableMagicShield;
	static HWND cBoxLookIds;
	static HWND cBoxEnableHoldPosition;
	static HWND cBoxEnableRainbow;
	static HWND cBoxEnableDisableMounts;
	static HWND cBoxEnableDisableFrames;
	static HWND cBoxEnableOutfitHack;
	static HWND cBoxEnableStandHaste;
	static HWND cBoxXray;
	static HWND cBoxShortKeys;
	static HWND cBoxEquipByHotkey;
	static HWND cBoxEquipByMoveItem;
	static HWND cBoxComboBot;
	static HWND cBoxDiscord;
	static HWND cBoxRuneMaker;




	static HWND cBoxAlarmEnemyOnScreen;
	static HWND cBoxAlarmPlayerOnScreen;
	static HWND cBoxAlarmMonsterOnScreen;
	static HWND cBoxAlarmSkullOnScreen;
	static HWND cBoxAlarmLowMana;
	static HWND cBoxAlarmHealth;
	static HWND cBoxFistFighting;
	static HWND cBoxDistanceFighting;
	static HWND cBoxShielding;
	static HWND cBoxSwordFighting;
	static HWND cBoxLevel;
	static HWND cBoxAxeFighting;
	static HWND cBoxClubFighting;

	//AutoLogout

	static HWND cBoxAutoLogoutEnemyOnScreen;
	static HWND cBoxAutoLogoutPlayerOnScreen;
	static HWND cBoxAutoLogoutSkullOnScreen;

	static HWND cBoxAutoLogoutLowMana;
	static HWND cBoxAutoLogoutLowSoul;
	static HWND cBoxAutoLogoutLowCap;




	// Say On Advertisting //
	static HWND inputAdvertisingString;

	// Combo Bot
	static InputComboBot inputComboBot;

	static int32_t clockHoldDash;
	static int32_t clockAdvertising;
	static int32_t clockAntiIdle;
	static int32_t clockMountVar;
	static int32_t clockEatFood;
	static int32_t clockHoldPosition;
	static int32_t clockManaShield;
	static int32_t clockKillTarget;
	static int32_t clockPlayAlarm;

	//Discord
	static int32_t clockDiscordPlayerInfo;

	//RuneMaker
	static int32_t clockRuneMaker;
};
