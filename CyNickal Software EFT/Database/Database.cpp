#include "pch.h"

#include "Database.h"

void Database::InsertToWeaponTable(std::wstring Name)
{
	auto db = GetTarkovDB();

	CreateWeaponTable(db);

	const char* InsertStatement = "INSERT INTO tarkov_weapons (weapon_name_blob) VALUES (?);";
	sqlite3_stmt* stmt{ nullptr };
	sqlite3_prepare_v2(db, InsertStatement, -1, &stmt, nullptr);
	sqlite3_bind_blob(stmt, 1, Name.data(), Name.size() * sizeof(wchar_t), SQLITE_STATIC);
	sqlite3_step(stmt);
	sqlite3_finalize(stmt);
}

sqlite3* Database::GetTarkovDB()
{
	if(m_TarkovDB)
		return m_TarkovDB;

	sqlite3_open("EFTItems.db", &m_TarkovDB);

	return m_TarkovDB;
}

void Database::CreateWeaponTable(sqlite3* db)
{
	const char* Statement = R"(CREATE TABLE IF NOT EXISTS tarkov_weapons (
	weapon_uid INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
	weapon_name_blob BLOB NOT NULL);)";
	sqlite3_exec(db, Statement, nullptr, nullptr, nullptr);
}