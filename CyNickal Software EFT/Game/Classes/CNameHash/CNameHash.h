#pragma once

enum class ENameMap : uint8_t
{
	Loot,
	Weapons,
	Ammo
};

class CNameHash
{
public:
	CNameHash(const std::string& WeaponName);
	const uint32_t GetHash() const { return m_Hash; }
	const bool IsValuable() const;
	const std::string* GetSanitizedName(ENameMap map) const;

private:
	uint32_t m_Hash{ 0 };
};