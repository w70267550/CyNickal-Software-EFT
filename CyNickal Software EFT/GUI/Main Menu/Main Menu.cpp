#include "pch.h"
#include "Main Menu.h"

#include "GUI/Color Picker/Color Picker.h"
#include "GUI/Player Table/Player Table.h"
#include "GUI/Loot Table/Loot Table.h"
#include "GUI/Fuser/Fuser.h"
#include "GUI/Radar/Radar.h"
#include "GUI/Aimbot/Aimbot.h"
#include "GUI/Config/Config.h"
#include "GUI/Keybinds/Keybinds.h"

void MainMenu::Render()
{

	ImGui::SetNextWindowSize(ImVec2(700, 550), ImGuiCond_Once);

	// Prevent Moving out of window
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking;

	ImGui::Begin("EFT DMA - CyNickal", nullptr, window_flags);

	if (ImGui::BeginTabBar("MenuTabs"))
	{
		if (ImGui::BeginTabItem("Fuser"))
		{
			Fuser::RenderSettings();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Radar"))
		{
			Radar::RenderSettings();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Aimbot"))
		{
			Aimbot::RenderSettings();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Color Picker"))
		{
			ColorPicker::Render();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Configs"))
		{
			Config::Render();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Players Table"))
		{
			PlayerTable::Render();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Loot Table"))
		{
			LootTable::Render();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Keybinds"))
		{
			Keybinds::Render();
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	ImGui::End();
}