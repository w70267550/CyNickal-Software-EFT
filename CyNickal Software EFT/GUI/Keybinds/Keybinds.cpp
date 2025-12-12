#include "pch.h"

#include "Keybinds.h"

#include "DMA/Input Manager.h"

#include "GUI/Aimbot/Aimbot.h"

#include "Game/Player List/Player List.h"

#include "GUI/Fuser/Draw/Loot.h"

extern std::atomic<bool> bRunning;

void Keybinds::Render()
{
	ImGui::InputScalar("Refresh Key", ImGuiDataType_U32, &m_RefreshPlayerList);
	ImGui::InputScalar("Force Exit Key", ImGuiDataType_U32, &m_ForceExitHotkey);
}

void Keybinds::OnFrame(DMA_Connection* Conn, uintptr_t LocalGameWorld)
{
	if (!c_keys::IsInitialized())
		return;

	if (c_keys::IsKeyDown(Conn, Aimbot::m_Keybind))
		Aimbot::OnDMAFrame(Conn);

	if (c_keys::IsKeyDown(Conn, m_ForceExitHotkey))
	{
		std::println("[Keybinds] Force Exit Key Pressed!");
		bRunning = false;

	}
	static bool prevLootKeyDown = false;
	bool currLootKeyDown = c_keys::IsKeyDown(Conn, DrawESPLoot::m_ToggleLootESP);
	if (currLootKeyDown && !prevLootKeyDown)
	{
		std::println("[Keybinds] Fuser Loot ESP Key Pressed!");
		DrawESPLoot::bMasterToggle = !DrawESPLoot::bMasterToggle;
	}
	prevLootKeyDown = currLootKeyDown;

	if (c_keys::IsKeyDown(Conn, m_RefreshPlayerList))
	{
		std::println("[Keybinds] Refresh Playerlist Key Pressed");
		PlayerList::FullUpdate(Conn);
	}

}
