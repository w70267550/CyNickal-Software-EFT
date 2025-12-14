#include "pch.h"

#include "Database.h"

sqlite3* Database::GetTarkovDB()
{
	if(m_TarkovDB)
		return m_TarkovDB;

	sqlite3_open("EFT_Data.db", &m_TarkovDB);

	return m_TarkovDB;
}