#include "pch.h"
#include "Healer.h"

Healer::Healer()
{
	int32_t m_TimerHealerSpells = 0;
	int32_t m_TimerHealerItems = 0;
	int32_t m_TimerHealFriend = 0;
	int32_t m_TimerAutoHaste = 0;
	int32_t m_TimerAntiParal = 0;
	int32_t m_TimerEquipAmulet = 0;
	int32_t m_TimerEquipRing = 0;
	int32_t m_TimerEquipRune = 0;
	int32_t m_TimerRuneMaker = 0;
	int32_t itemContNr = 0;
	int32_t ringContNr = 0;
}

void Healer::HealWithSpells(const LightSpell& lightSpell, const MidSpell& midSpell, const HeavySpell& heavySpell)
{
	CSelfCharacter selfCharacter;
	MemReader::GetInstance().ReadSelfCharacter(&selfCharacter);
	if ((heavySpell.spellLenght > 0) && (selfCharacter.hpPercentage <= heavySpell.hpPercentage) && (selfCharacter.mana >= heavySpell.manaCost) && (Util::isNotExhausted(m_TimerHealerSpells, Cooldowns::GetInstance().HEAL_SPELL)))
	{
		PacketSend::GetInstance().Say(heavySpell.spell);
	}
	else if ((midSpell.spellLenght > 0) && (selfCharacter.hpPercentage <= midSpell.hpPercentage) && (selfCharacter.mana >= midSpell.manaCost) && (Util::isNotExhausted(m_TimerHealerSpells, Cooldowns::GetInstance().HEAL_SPELL)))
	{
		PacketSend::GetInstance().Say(midSpell.spell);
	}
	else if ((lightSpell.spellLenght > 0) && (selfCharacter.hpPercentage <= lightSpell.hpPercentage) && (selfCharacter.mana >= lightSpell.manaCost) && (Util::isNotExhausted(m_TimerHealerSpells, Cooldowns::GetInstance().HEAL_SPELL)))
	{
		PacketSend::GetInstance().Say(lightSpell.spell);
	}
}

void Healer::HealWithItems(const HealthPotionItemHealer& healthPotion, const ManaPotionItemHealer& manaPotion)
{
	CSelfCharacter selfCharacter;
	MemReader::GetInstance().ReadSelfCharacter(&selfCharacter);
	if ((selfCharacter.hpPercentage <= healthPotion.hpPerc) && (Util::isNotExhausted(m_TimerHealerItems, Cooldowns::GetInstance().HEAL_ITEM)))
	{
		if (healthPotion.itemId && healthPotion.hpPerc)
		{
			PacketSend::GetInstance().UseItemOn(healthPotion.itemId, selfCharacter.id);
		}
	}
	else if ((selfCharacter.manaPercentage <= manaPotion.manaPerc) && (Util::isNotExhausted(m_TimerHealerItems, Cooldowns::GetInstance().HEAL_ITEM)))
	{
		if (manaPotion.itemId && manaPotion.manaPerc)
		{
			PacketSend::GetInstance().UseItemOn(manaPotion.itemId, selfCharacter.id);
		}
	}
}


void Healer::HealPlayer(int32_t safeHp, int32_t healFriendOnHp, std::array<std::string, 100>& friendList, bool sioWithSpell, int32_t selectedMode)
{
	CSelfCharacter selfCharacter;
	MemReader::GetInstance().ReadSelfCharacter(&selfCharacter);
	std::vector<Entity*> entities = MemReader::GetInstance().ReadVisibleCreatures();
	std::vector<VipStruct*> vips;
	for (auto& entity : entities)
	{
		if ((entity->hpPercentage <= healFriendOnHp) && (selfCharacter.hpPercentage > safeHp))
		{
			bool shouldBeHealed = false;

			switch (selectedMode)
			{
			case 0:
				for (int32_t index = 0; index < friendList.size(); index++)
				{
					if (friendList[index].empty())	break;

					if (strcmp(entity->name, friendList[index].c_str()) == 0)
					{
						shouldBeHealed = true;
						break;
					}
				}
				break;
			case 1:
				shouldBeHealed = entity->isInGuild();
				break;
			case 2:
				shouldBeHealed = entity->isInParty();
				break;
			case 3:
				vips = MemReader::GetInstance().GetVips();
				for (VipStruct*& vip : vips)
				{
					if ((strcmp(entity->name, vip->getVipName()) == 0) && (vip->typeFlag == VipFlags::Heart))
					{
						shouldBeHealed = true;
						break;
					}
				}
				break;
			default:
				break;
			}

			if (!shouldBeHealed)	continue;

			if (Util::isNotExhausted(m_TimerHealerSpells, Cooldowns::GetInstance().HEAL_SPELL))
			{
				Position myPosition = { selfCharacter.xPos, selfCharacter.yPos, selfCharacter.zPos };
				Position toPosition = { entity->xPos, entity->yPos, entity->zPos };
				bool isShootable = MemReader::GetInstance().IsShootable(toPosition, myPosition);
				if (sioWithSpell && isShootable)
				{
					std::string msg("exura sio \"");
					msg += std::string(entity->name);
					PacketSend::GetInstance().Say(msg.c_str());
				}
				else if (isShootable)
				{
					PacketSend::GetInstance().UseItemOn(3160, entity->id);
				}
				else
				{
					return;
				}
			}
		}
	}
}

void Healer::Haste(const HasteSpell& hasteSpell)
{
	CSelfCharacter selfCharacter;
	MemReader::GetInstance().ReadSelfCharacter(&selfCharacter);
	if ((selfCharacter.mana >= hasteSpell.manaCost) && (!(selfCharacter.flags & CHARACTER_FLAGS::HASTED)) && (Util::isNotExhausted(m_TimerAutoHaste, Cooldowns::GetInstance().SUPPORT_SPELL)))
		PacketSend::GetInstance().Say(hasteSpell.spell);
}

void Healer::CureParalyze(const CureParalayseSpell& paralSpell)
{
	CSelfCharacter selfCharacter;
	MemReader::GetInstance().ReadSelfCharacter(&selfCharacter);
	if ((selfCharacter.mana >= paralSpell.manaCost) && ((selfCharacter.flags & CHARACTER_FLAGS::PARALYSED)) && (Util::isNotExhausted(m_TimerAntiParal, Cooldowns::GetInstance().SUPPORT_SPELL)))
	{
		PacketSend::GetInstance().Say(paralSpell.spell);
	}
}

void Healer::EquipAmuletBalancer(bool bEquipModeHotkey, bool bEquipModeMoveItem, const ExItem& toEquipIfLow, const ExItem& toSwitchWhenSafe, const ExItem& toDequipWhenSafe)
{
	CSelfCharacter selfCharacter;
	MemReader::GetInstance().ReadSelfCharacter(&selfCharacter);
	int32_t currentItemOnSlot = MemReader::GetInstance().GetItemOnEquipmentSlots()->amuletId;
	Item item = MemReader::GetInstance().ReadContainersForItem2(toEquipIfLow.Id);
	Item item2 = MemReader::GetInstance().ReadContainersForItem2(toSwitchWhenSafe.Id);

	if ((toEquipIfLow.Id) && (currentItemOnSlot != toEquipIfLow.Id) && ((selfCharacter.hpPercentage <= toEquipIfLow.hpPercentage) && !MemReader::GetInstance().IsFlagTrue(CHARACTER_FLAGS::MANA_SHIELD)))
	{
		if ((bEquipModeMoveItem) && (Util::isNotExhausted(m_TimerEquipAmulet, Cooldowns::GetInstance().EQUIP_ITEM)))
		{
			if (item.id)
			{
				PacketSend::GetInstance().MoveItemFromContainerToPosition(item.id, item.slotNumber, 0xFFFF, EquipmentItems::Amulet, 0, item.contNr);
				itemContNr = item.contNr;

				
			}


		}
	}
	else if ((toSwitchWhenSafe.Id) && ((currentItemOnSlot != toSwitchWhenSafe.Id)) && ((selfCharacter.hpPercentage > toSwitchWhenSafe.hpPercentage) && !MemReader::GetInstance().IsFlagTrue(CHARACTER_FLAGS::MANA_SHIELD)))
	{

		if ((bEquipModeMoveItem) && (Util::isNotExhausted(m_TimerEquipAmulet, Cooldowns::GetInstance().BETWEEN_PACKETS)))
		{
			if (item2.id)
			{
				PacketSend::GetInstance().MoveItemFromContainerToPosition(item2.id, item2.slotNumber, 0xFFFF, EquipmentItems::Amulet, 0, item2.contNr);
			}
		}
	}
	else if ((toDequipWhenSafe.Id) && (currentItemOnSlot == toDequipWhenSafe.Id) && ((selfCharacter.hpPercentage > toDequipWhenSafe.hpPercentage) && (!MemReader::GetInstance().IsFlagTrue(CHARACTER_FLAGS::MANA_SHIELD))))
	{
		if ((bEquipModeMoveItem) && (Util::isNotExhausted(m_TimerEquipAmulet, Cooldowns::GetInstance().BETWEEN_PACKETS)))
		{
			if (itemContNr != 0)
			{
				PacketSend::GetInstance().MoveItemFromAmuletPositionToContainer(toDequipWhenSafe.Id, 0, 0xFFFF, itemContNr, 0);
			}
			else
			{
				PacketSend::GetInstance().MoveItemFromAmuletPositionToContainer(toDequipWhenSafe.Id, 0, 0xFFFF, item.contNr, 0);
			}
		}
	}
}

void Healer::EquipRingBalancer(bool bEquipModeHotkey, bool bEquipModeMoveItem, const ExItem& toEquipIfLow, const ExItem& toSwitchWhenSafe, const ExItem& toDequipWhenSafe)
{
	CSelfCharacter selfCharacter;
	MemReader::GetInstance().ReadSelfCharacter(&selfCharacter);
	int32_t currentItemOnSlot = MemReader::GetInstance().GetItemOnEquipmentSlots()->ringId;
	Item item = MemReader::GetInstance().ReadContainersForItem2(toEquipIfLow.Id);
	Item item2 = MemReader::GetInstance().ReadContainersForItem2(toSwitchWhenSafe.Id);

	if ((toEquipIfLow.Id) && (currentItemOnSlot != toEquipIfLow.Id) && ((selfCharacter.hpPercentage <= toEquipIfLow.hpPercentage)))
	{
		if ((bEquipModeMoveItem) && (Util::isNotExhausted(m_TimerEquipRing, Cooldowns::GetInstance().EQUIP_ITEM)))
		{
			if (item.id)
			{
				PacketSend::GetInstance().MoveItemFromContainerToPosition(item.id, item.slotNumber, 0xFFFF, EquipmentItems::Ring, 0, item.contNr);
				ringContNr = item.contNr;

			}


		}
	}
	else if ((toSwitchWhenSafe.Id) && ((currentItemOnSlot != toSwitchWhenSafe.Id)) && ((selfCharacter.hpPercentage > toSwitchWhenSafe.hpPercentage)))
	{

		if ((bEquipModeMoveItem) && (Util::isNotExhausted(m_TimerEquipRing, Cooldowns::GetInstance().BETWEEN_PACKETS)))
		{
			if (item2.id)
			{
				PacketSend::GetInstance().MoveItemFromContainerToPosition(item2.id, item2.slotNumber, 0xFFFF, EquipmentItems::Ring, 0, item2.contNr);
			}
		}
	}
	else if ((toDequipWhenSafe.Id) && (currentItemOnSlot == toDequipWhenSafe.Id) && ((selfCharacter.hpPercentage > toDequipWhenSafe.hpPercentage)))
	{
		if ((bEquipModeMoveItem) && (Util::isNotExhausted(m_TimerEquipRing, Cooldowns::GetInstance().BETWEEN_PACKETS)))
		{
			//MessageBoxA(NULL, "inside", NULL, NULL);
			
			if (ringContNr != 0)
			{
				PacketSend::GetInstance().MoveItemFromRingPositionToContainer(toDequipWhenSafe.Id, 0, 0xFFFF, ringContNr, 0);
			}
			else
			{
				PacketSend::GetInstance().MoveItemFromRingPositionToContainer(toDequipWhenSafe.Id, 0, 0xFFFF, item.contNr, 0);
			}
		}
	}
}



void Healer::RuneMaker(const RuneMakerSpell& rmSpell)
{
	CSelfCharacter selfCharacter;
	MemReader::GetInstance().ReadSelfCharacter(&selfCharacter);
	int32_t currentItemOnSlot = MemReader::GetInstance().GetItemOnEquipmentSlots()->leftHandId;
	Item item = MemReader::GetInstance().ReadContainersForItem2(3147);
	int32_t mySoul = *(int*)(m_ModuleBase + Offsets::soul);

	int32_t spellSoul = atoi(rmSpell.soul);
	int32_t spellMana = atoi(rmSpell.mana);
	int32_t runeId = atoi(rmSpell.runeId);


	

	if (spellSoul <= mySoul && spellMana < selfCharacter.mana && rmSpell.spellLenght != 0)
	{
		if (currentItemOnSlot != 3147 && item.id !=0)
		{

			if ((Util::isNotExhausted(m_TimerEquipRune, Cooldowns::GetInstance().BETWEEN_PACKETS)))
			{
				PacketSend::GetInstance().MoveItemFromContainerToLeftHand(item.id, item.slotNumber, 0xFFFF, EquipmentItems::Left_hand, 0, item.contNr);
			}
		}
		else if (Util::isNotExhausted(m_TimerEquipRune, Cooldowns::GetInstance().BETWEEN_PACKETS))
		{
			PacketSend::GetInstance().Say(rmSpell.spell);
			currentItemOnSlot = MemReader::GetInstance().GetItemOnEquipmentSlots()->leftHandId;
			item = MemReader::GetInstance().ReadContainersForItem2(runeId);
		}
		
		else if ((Util::isNotExhausted(m_TimerEquipRune, Cooldowns::GetInstance().BETWEEN_PACKETS)) && currentItemOnSlot == runeId && item.id != 0)
		{
			PacketSend::GetInstance().MoveItemFromPositionToContainer(runeId, 0, 0xFFFF, item.contNr, 0);
		}
	}
}
//3172