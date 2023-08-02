#include "pch.h"
#include "PacketSend.h"
#include "structs.h"

PacketSend::PacketSend()
{
	moduleBase = (uintptr_t)GetModuleHandle(NULL);
	PrintWhiteText = FnPacketLPCSTR(moduleBase + 0x144A90);
	PacketItem = FnPacketInt(moduleBase + 0xF88A0);
	PacketMsg = FnPacketLPCSTR(moduleBase + 0xF8A40);

	PacketStart = FnPacketInt(moduleBase + 0xF8290);

	PacketContainer = FnPacketInt(moduleBase + 0xF8700);
	PacketChannelId = FnPacketInt(moduleBase + 0xF8560);

	PacketChannelId2 = FnPacketInt(moduleBase + 0x4F8560);

	PacketEnd = FnPacketByte(moduleBase + 0xF8E40);
	PacketEnd2 = FnPacketByte(moduleBase + 0x4F8E40);
	PacketPlayerId = FnPacketInt(moduleBase + 0xF88A0);
}

void PacketSend::Attack(int32_t creatureId)
{
	PacketStart(0xa1);
	PacketItem(creatureId);
	PacketItem(10);
	PacketEnd(1);
}

void PacketSend::Say(const char* msgToSay)
{
	PacketStart(0x96);
	PacketChannelId(CHANNEL_ID::SAY);
	PacketMsg(msgToSay);
	PacketEnd(1);
}

void PacketSend::SayOnAdvertisting(const char* msgToSay)
{
	PacketStart(0x96);
	PacketChannelId(CHANNEL_ID::ADVERTISING);
	PacketContainer(CHANNEL_ID::PRIVATE_MSG);
	PacketMsg(msgToSay);
	PacketEnd(1);
}


void PacketSend::Turn(uint8_t direction)
{
	PacketStart(direction);
	PacketEnd(1);
}


void PacketSend::UseItemWithFromContainerToCreature(uint16_t itemId,uint32_t creatureId, BYTE slotNumber, int32_t toXPos, int32_t toYPos, int32_t toZPos)
{
	PacketStart(0x83);
	PacketContainer(65535);
	PacketContainer(64);
	PacketChannelId(slotNumber);
	PacketContainer(itemId);
	PacketChannelId(slotNumber);
	PacketContainer(toXPos);
	PacketContainer(toYPos);
	PacketChannelId(toZPos);
	PacketContainer(creatureId);
	PacketChannelId(1);
	PacketEnd(1);
}

void PacketSend::UseItemOn(int32_t itemId, uint32_t creatureId)
{
	PacketStart(0x84);
	PacketContainer(65535);
	PacketContainer(0);
	PacketChannelId(0);
	PacketContainer(itemId);
	PacketChannelId(0);
	PacketPlayerId(creatureId);
	PacketEnd(1);
}

void PacketSend::Step(uint8_t direction)
{
	PacketStart(direction);
	PacketEnd(1);
}

void PacketSend::UseItemInContainer(int32_t inContainerNumber, int32_t onSlotNumber, int32_t itemId)
{
	PacketStart(0x82);
	PacketContainer(0xFFFF);
	PacketContainer(0x40 + inContainerNumber);
	PacketChannelId(onSlotNumber);
	PacketContainer(itemId);
	PacketChannelId(onSlotNumber);
	PacketChannelId(0);
	PacketEnd(1);
}

void PacketSend::MoveItemFromContainerToPosition(int32_t itemId, int32_t fromSlotNumber, int32_t toXPos, int32_t toYPos, int32_t toZPos, int32_t contNr)
{
	PacketStart(0x78);
	PacketContainer(0xFFFF);
	PacketContainer(0x40 + contNr);
	PacketChannelId(fromSlotNumber);
	PacketContainer(itemId);
	PacketChannelId(fromSlotNumber);
	PacketContainer(toXPos);
	PacketContainer(toYPos);
	PacketChannelId(toZPos);
	PacketChannelId(1);
	PacketEnd(1);
}

void PacketSend::MoveItemFromContainerToLeftHand(int32_t itemId, int32_t fromSlotNumber, int32_t toXPos, int32_t toYPos, int32_t toZPos, int32_t contNr)
{
	PacketStart(0x78);
	PacketContainer(0xFFFF);
	PacketContainer(0x40 + contNr);
	PacketChannelId(fromSlotNumber);
	PacketContainer(itemId);
	PacketChannelId(fromSlotNumber);
	PacketContainer(toXPos);
	PacketContainer(toYPos);
	PacketChannelId(toZPos);
	PacketChannelId(1);
	PacketEnd(1);
}

void PacketSend::MoveItemFromPositionToContainer(int32_t itemId, int32_t fromSlotNumber, int32_t toXPos, int32_t toYPos, int32_t toZPos)
{
	PacketStart(0x78);
	PacketContainer(0xFFFF);
	PacketContainer(6);
	PacketChannelId(fromSlotNumber);
	PacketContainer(itemId);
	PacketChannelId(fromSlotNumber);
	PacketContainer(toXPos);
	PacketContainer(0x40 + toYPos);
	PacketChannelId(toZPos);
	PacketChannelId(1);
	PacketEnd(1);
}

void PacketSend::MoveItemFromRingPositionToContainer(int32_t itemId, int32_t fromSlotNumber, int32_t toXPos, int32_t toYPos, int32_t toZPos)
{
	PacketStart(0x78);
	PacketContainer(0xFFFF);
	PacketContainer(9);
	PacketChannelId(fromSlotNumber);
	PacketContainer(itemId);
	PacketChannelId(fromSlotNumber);
	PacketContainer(toXPos);
	PacketContainer(0x40 + toYPos);
	PacketChannelId(toZPos);
	PacketChannelId(1);
	PacketEnd(1);
}

void PacketSend::MoveItemFromAmuletPositionToContainer(int32_t itemId, int32_t fromSlotNumber, int32_t toXPos, int32_t toYPos, int32_t toZPos)
{
	PacketStart(0x78);
	PacketContainer(0xFFFF);
	PacketContainer(2);
	PacketChannelId(fromSlotNumber);
	PacketContainer(itemId);
	PacketChannelId(fromSlotNumber);
	PacketContainer(toXPos);
	PacketContainer(0x40 + toYPos);
	PacketChannelId(toZPos);
	PacketChannelId(1);
	PacketEnd(1);
}

void PacketSend::LogOut()
{
	PacketStart(0x14);
	PacketEnd(1);
}