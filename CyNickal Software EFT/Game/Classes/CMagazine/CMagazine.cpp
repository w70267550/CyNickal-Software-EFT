#include "pch.h"
#include "CMagazine.h"
#include "Game/Offsets/Offsets.h"

CMagazine::CMagazine(uintptr_t MagazineSlotAddress) : CBaseEntity(MagazineSlotAddress)
{
	std::println("[CMagazine] Constructed at {0:X}", m_EntityAddress);
}

void CMagazine::PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CSlot::pContainedItem, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_ContainedItemAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CMagazine::PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t))
		SetInvalid();

	if(!m_ContainedItemAddress)
		SetInvalid();

	if (IsInvalid()) return;
	
	VMMDLL_Scatter_PrepareEx(vmsh, m_ContainedItemAddress + Offsets::CItem::pCartridges, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_MagazineCartridgesAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CMagazine::PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t))
		SetInvalid();

	if (!m_MagazineCartridgesAddress)
		SetInvalid();

	if (IsInvalid()) return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_MagazineCartridgesAddress + Offsets::CStackSlot::pItems, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_StackItemsAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
	VMMDLL_Scatter_PrepareEx(vmsh, m_MagazineCartridgesAddress + Offsets::CStackSlot::Max, sizeof(uint32_t), reinterpret_cast<BYTE*>(&m_MaxCartridges), nullptr);
}

void CMagazine::PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t))
		SetInvalid();

	if (!m_StackItemsAddress)
		SetInvalid();

	if (IsInvalid()) return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_StackItemsAddress + 0x10, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_ListAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CMagazine::PrepareRead_5(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t))
		SetInvalid();

	if (!m_ListAddress)
		SetInvalid();

	if (IsInvalid()) return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_ListAddress + 0x20, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_AmmoItemAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CMagazine::PrepareRead_6(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t))
		SetInvalid();

	if (!m_AmmoItemAddress)
		SetInvalid();

	if (IsInvalid()) return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_AmmoItemAddress + Offsets::CItem::StackCount, sizeof(uint32_t), reinterpret_cast<BYTE*>(&m_CurrentCartridges), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CMagazine::Finalize()
{
	if (IsInvalid()) return;
}

void CMagazine::QuickRead(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_AmmoItemAddress + Offsets::CItem::StackCount, sizeof(uint32_t), reinterpret_cast<BYTE*>(&m_CurrentCartridges), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CMagazine::QuickFinalize()
{
	if (m_BytesRead != sizeof(uint32_t))
		SetInvalid();
}