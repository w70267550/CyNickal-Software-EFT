#pragma once

#include "sqlite3.h"

class Database
{
public:
	static void InsertToWeaponTable(std::wstring Name);
	static sqlite3* GetTarkovDB();

private:
	static inline sqlite3* m_TarkovDB{ nullptr };
	static void CreateWeaponTable(sqlite3* db);
};