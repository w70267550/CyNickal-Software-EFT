#pragma once
#include "Game/Classes/CBaseEntity/CBaseEntity.h"
#include "Game/Classes/CItemHash/CItemHash.h"

class CItem : public CBaseEntity
{
public:
	CItem(uintptr_t EntityAddress);
	void PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh);
	void CompleteUpdate();
	void Finalize();
	const char* GetUnfilteredName() const;
	const char* GetSanitizedName() const;

private:
	uintptr_t m_ItemTemplateAddress{ 0 };
	uintptr_t m_ItemTemplateNameAddress{ 0 };
	std::array<wchar_t, 32> m_wItemNameBuffer{ 0 };
	std::array<char, 32> m_ItemNameBuffer{ 0 };

public:
	CItemHash m_ItemHash{ "" };
};