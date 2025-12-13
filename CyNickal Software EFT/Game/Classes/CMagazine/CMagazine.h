#pragma once
#include "Game/Classes/CBaseEntity/CBaseEntity.h"

class CMagazine : public CBaseEntity
{
public:
	CMagazine(uintptr_t MagazineSlotAddress);
	void PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_5(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_6(VMMDLL_SCATTER_HANDLE vmsh);
	void Finalize();
	void QuickRead(VMMDLL_SCATTER_HANDLE vmsh);
	void QuickFinalize();

public:
	uint32_t m_MaxCartridges{ 0 };
	uint32_t m_CurrentCartridges{ 0 };

private:
	uintptr_t m_ContainedItemAddress{ 0 };
	uintptr_t m_MagazineCartridgesAddress{ 0 };
	uintptr_t m_StackItemsAddress{ 0 };
	uintptr_t m_ListAddress{ 0 };
	uintptr_t m_AmmoItemAddress{ 0 };
};