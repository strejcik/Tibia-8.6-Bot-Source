#include "pch.h"

//sort values [monsters] in ascending order and persist original indexes
template <typename T>
std::vector<size_t> sort_indexes(const std::vector<T>& v) {

	// initialize original index locations
	std::vector<size_t> idx(v.size());
	iota(idx.begin(), idx.end(), 0);

	// sort indexes based on comparing values in v
	// using std::stable_sort instead of std::sort
	// to avoid unnecessary index re-orderings
	// when v contains elements of equal values 
	stable_sort(idx.begin(), idx.end(),
		[&v](size_t i1, size_t i2) {return v[i1] < v[i2]; });

	return idx;
}

AutoAttack::AutoAttack()
{
	int32_t m_TimerAutoAttack = 0;
	int32_t m_TimerAttackSpell = 0;
	int32_t m_TimerHoldTarget = 0;
	int32_t m_lockedTargetId = 0;
}

bool MemReader::IsShootable(const Position& toPosition, const Position& myPosition)
{
	int32_t XSign = 0;
	if (toPosition.xPos > myPosition.xPos)
	{
		XSign = 1;
	}
	else if (toPosition.xPos < myPosition.xPos)
	{
		XSign = -1;
	}
	int32_t YSign = 0;
	if (toPosition.yPos > myPosition.yPos)
	{
		YSign = 1;
	}
	else if (toPosition.yPos < myPosition.yPos)
	{
		YSign = -1;
	}
	int32_t XDistance = abs(toPosition.xPos - myPosition.xPos);
	int32_t YDistance = abs(toPosition.yPos - myPosition.yPos);
	int32_t max = max(XDistance, YDistance);

	// This checks if location is on viewable screen, someone might to remove that for some reason
	//if (abs(XDistance) > 8 || abs(YDistance) > 5)
	//{
	//	return false;
	//}


	for (int32_t i = 1; i < max; i++)
	{
		float readX = (float(i * XDistance) / max);
		float readY = (float(i * YDistance) / max);
		MapObject* mapObject = ReadMapTile(static_cast<int32_t>(std::round(readX)) * XSign, static_cast<int32_t>(std::round(readY)) * YSign);

		std::vector<MapItem*> mapItems = mapObject->readTile();

		for (auto& mapItem : mapItems)
		{
			if (ItemFlagInfo(mapItem->itemId, 0x0E)) // good sio with 0x0E
			{
				return false;
			}
		}
	}
	return true;
}


//Gets closes to player (monster id) by coordinates
int32_t AutoAttack::GetEntityId()
{
	std::vector<Entity*> entities = MemReader::GetInstance().ReadVisibleMonsters();
	std::vector < double > distance;
	CSelfCharacter selfCharacter;
	MemReader::GetInstance().ReadSelfCharacter(&selfCharacter);

	for (std::size_t i = 0; i != entities.size(); ++i)
	{

		if (floor(sqrt(pow(entities[i]->xPos - selfCharacter.xPos, 2) + pow(entities[i]->yPos - selfCharacter.yPos, 2))) <= 1.73205)
		{
			//top left (1.73205)
			if ((entities[i]->xPos - selfCharacter.xPos == -1) && (entities[i]->yPos - selfCharacter.yPos == -1)) {
				distance.push_back(1);
			}
			//top (1.73205)
			else if ((entities[i]->xPos - selfCharacter.xPos == 0) && (entities[i]->yPos - selfCharacter.yPos == -1)) {
				distance.push_back(1);
			}
			//top right (1.73205)
			else if ((entities[i]->xPos - selfCharacter.xPos == 1) && (entities[i]->yPos - selfCharacter.yPos == -1)) {
				distance.push_back(1);
			}
			//right (1.41421)
			else if ((entities[i]->xPos - selfCharacter.xPos == 1) && (entities[i]->yPos - selfCharacter.yPos == 0)) {
				distance.push_back(1);
			}
			//bottom right (1.73205)
			else if ((entities[i]->xPos - selfCharacter.xPos == 1) && (entities[i]->yPos - selfCharacter.yPos == 1)) {
				distance.push_back(1);
			}
			//bottom(1.73205)
			else if ((entities[i]->xPos - selfCharacter.xPos == 0) && (entities[i]->yPos - selfCharacter.yPos == 1)) {
				distance.push_back(1);
			}
			//bottom left
			else if ((entities[i]->xPos - selfCharacter.xPos == -1) && (entities[i]->yPos - selfCharacter.yPos == 1)) {
				distance.push_back(1);
			}
			//left
			else if ((entities[i]->xPos - selfCharacter.xPos == -1) && (entities[i]->yPos - selfCharacter.yPos == 0)) {
				distance.push_back(1);
			}
		}
		else
		{
			distance.push_back(floor(sqrt(pow(entities[i]->xPos - selfCharacter.xPos, 2) + pow(entities[i]->yPos - selfCharacter.yPos, 2))));
		}
		//distance.push_back(floor(sqrt(pow(entities[i]->xPos - selfCharacter.xPos, 2) + pow(entities[i]->yPos - selfCharacter.yPos, 2))));
	}

	for (auto i : sort_indexes(distance))
	{
		if (entities[i])
		{
			return entities[i]->id;
		}
	}
	return 0;
}

void AutoAttack::Enable()
{
	CSelfCharacter selfCharacter;
	MemReader::GetInstance().ReadSelfCharacter(&selfCharacter);
	int32_t creatureId = GetEntityId();
	std::vector<Entity*> ent = MemReader::GetInstance().GetEntityById(creatureId);

	
	bool isShootable = false;

	if (ent.size() > 0)
	{
		Position myPosition = { selfCharacter.xPos, selfCharacter.yPos, selfCharacter.zPos };
		Position toPosition = { ent[0]->xPos, ent[0]->yPos, ent[0]->zPos };
		isShootable = MemReader::GetInstance().IsShootable(toPosition, myPosition);
	}
	if ((creatureId) && (Util::isNotExhausted(m_TimerAutoAttack, Cooldowns::GetInstance().BETWEEN_PACKETS)) && MemReader::GetInstance().GetAttackedCreature() == 0)
	{
		if (isShootable) {
			PacketSend::GetInstance().Attack(creatureId);
			MemReader::GetInstance().SetAttackedCreature(creatureId);
		}
	}

	//if ((!creatureId) && (Util::isNotExhausted(m_TimerAutoAttack, Cooldowns::GetInstance().BETWEEN_PACKETS)))
	//{
	//	PacketSend::GetInstance().Attack(0);
	//	MemReader::GetInstance().SetAttackedCreature(0);
	//}

}



void AutoAttack::HoldTarget()
{
	if (GetAsyncKeyState(VK_ESCAPE) & 1)
	{
		m_lockedTargetId = 0;
		PacketSend::GetInstance().Attack(m_lockedTargetId);
		MemReader::GetInstance().SetAttackedCreature(m_lockedTargetId);
	}

	if (MemReader::GetInstance().GetAttackedCreature())
	{
		m_lockedTargetId = MemReader::GetInstance().GetAttackedCreature();
	}
	else
	{
		if (Util::isNotExhausted(m_TimerHoldTarget, Cooldowns::GetInstance().ATTACK_CREATURE))
		{
			std::vector<Entity*> entities = MemReader::GetInstance().ReadVisibleCreatures();
			for (Entity*& entity : entities)
			{
				if (entity->id == m_lockedTargetId)
				{
					PacketSend::GetInstance().Attack(m_lockedTargetId);
					MemReader::GetInstance().SetAttackedCreature(m_lockedTargetId);
				}
			}
		}
	}
}

/*	numberOfMobs = number of monsters around us to cast the spell
	relativeDistance = relative distance between our character and monster
	spell = which aoe spell to cast
	manaCost = mana needed for that spell
	safeMode = bool that indicates if we want to cast spells when other players are on screen which will get us int32_to PVP and they can attack us
*/
void AutoAttack::UseAOEspell(int32_t numberOfMobs, int32_t relativeDistance, const char* spell, int32_t manaCost, bool safeMode)
{
	std::vector<Entity*> entities = MemReader::GetInstance().ReadVisibleCreatures();
	CSelfCharacter selfCharacter;
	MemReader::GetInstance().ReadSelfCharacter(&selfCharacter);
	int32_t mobs = 0;
	for (Entity*& entity : entities)
	{
		if ((safeMode) && (!MemReader::GetInstance().IsMonster(entity->id))) return;
		else if ((MemReader::GetInstance().IsMonster(entity->id)) && (abs(entity->xPos - selfCharacter.xPos) <= relativeDistance) && (abs(entity->yPos - selfCharacter.yPos) <= relativeDistance) && (entity->hpPercentage > 0))
		{
			mobs += 1;
		}
	}
	if ((mobs >= numberOfMobs) && (selfCharacter.mana >= manaCost) && (Util::isNotExhausted(m_TimerAttackSpell, Cooldowns::GetInstance().ATTACK_SPELL))) PacketSend::GetInstance().Say(spell);
}