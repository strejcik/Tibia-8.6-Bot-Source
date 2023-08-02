#pragma once

class Healer : public ISingleton<Healer>
{
private:
	int32_t m_OldEquipedRingId;

	const std::map<int, int> m_RingIds =
	{
		{3051, 3088},
		{3097,3099},
		{6299,6300},
		{3091,3094},
		{3049,3086},
		{3050,3087},
		{3093,3096},
		{3092,3095},
		{3053,3090},
		{3098,3100},
		{3052,3089},
		{16114,16264}
	};
public:
	Healer();
	int32_t m_TimerHealerSpells = 0;
	int32_t m_TimerHealerItems = 0;
	int32_t m_TimerHealFriend = 0;
	int32_t m_TimerAutoHaste = 0;
	int32_t m_TimerAntiParal = 0;
	int32_t m_TimerEquipRing = 0;
	int32_t m_TimerEquipAmulet = 0;
	int32_t m_TimerEquipRune = 0;
	int32_t m_TimerRuneMaker = 0;
	int32_t itemContNr = 0;
	int32_t ringContNr = 0;

	void HealWithSpells(const LightSpell& lightSpell, const MidSpell& midSpell, const HeavySpell& heavySpell);
	void HealWithItems(const HealthPotionItemHealer& healthPotion, const ManaPotionItemHealer& manaPotion);
	void HealPlayer(int32_t safeHp, int32_t healFriendOnHp, std::array<std::string, 100>& friendList, bool sioWithSpell, int32_t selectedMode);
	void Haste(const HasteSpell& hasteSpell);
	void CureParalyze(const CureParalayseSpell& paralSpell);

	void EquipAmuletBalancer(bool bEquipModeHotkey, bool bEquipModeMoveItem, const ExItem& toEquipIfLow, const ExItem& toSwitchWhenSafe, const ExItem& toDequipWhenSafe);
	void EquipRingBalancer(bool bEquipModeHotkey, bool bEquipModeMoveItem, const ExItem& toEquipIfLow, const ExItem& toSwitchWhenSafe, const ExItem& toDequipWhenSafe);
	void RuneMaker(const RuneMakerSpell& rmSpell);

	//void HealWithSpells(const LightSpell& lightSpell, const MidSpell& midSpell, const HeavySpell& heavySpell);
};
