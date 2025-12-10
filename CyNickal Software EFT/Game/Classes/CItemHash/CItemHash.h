#pragma once

class CItemHash
{
public:
	CItemHash(const std::string& WeaponName);
	const std::string& GetName() const;
	const uint32_t GetHash() const { return m_Hash; }

private:
	uint32_t m_Hash{ 0 };
};