#include "pch.h"
#include "Config.h"

#include <shlobj.h>
#include <fstream>

#include "GUI/Aimbot/Aimbot.h"
#include "GUI/Fuser/Fuser.h"
#include "GUI/Color Picker/Color Picker.h"
#include "GUI/Radar/Radar.h"
#include "GUI/Fuser/Draw/Loot.h"
#include "GUI/Fuser/Draw/Players.h"

std::string Config::getConfigDir() {
	char path[MAX_PATH];
	// Ensure CSIDL_PERSONAL is defined by including <shlobj.h>
	if (SUCCEEDED(SHGetFolderPathA(nullptr, CSIDL_PERSONAL, nullptr, 0, path))) {
		std::filesystem::path docPath(path);
		docPath /= "EFT-DMA";
		docPath /= "Configs";
		if (!std::filesystem::exists(docPath)) {
			std::filesystem::create_directories(docPath);
		}
		return docPath.string();
	}

	std::filesystem::path fallback("Configs");
	if (!std::filesystem::exists(fallback))
		std::filesystem::create_directory(fallback);
	return fallback.string();
}

std::string Config::getConfigPath(const std::string& configName) {
	std::filesystem::path p = getConfigDir();
	p /= configName + ".json";
	return p.string();
}

void Config::Render()
{
	static char configNameBuf[128] = "default";
	static int selectedConfig = -1;
	std::string configDir;
	std::vector<std::string> configFiles;

	bool firstrun = false;
	if (!firstrun) {
		configDir = getConfigDir();
		for (const auto& entry : std::filesystem::directory_iterator(configDir)) {
			if (entry.is_regular_file() && entry.path().extension() == ".json") {
				configFiles.push_back(entry.path().stem().string());
			}
		}
		firstrun = true;
	}

	ImGui::BeginChild("##ConfigsSettings", ImVec2(ImGui::GetContentRegionAvail().x / 2, ImGui::GetContentRegionAvail().y), true);

	ImGui::Text("Config Settings");
	ImGui::Separator();

	ImGui::SetNextItemWidth(150.0f);
	ImGui::InputText("Config Name", configNameBuf, IM_ARRAYSIZE(configNameBuf));

	if (ImGui::Button("Save Config"))
	{
		SaveConfig(configNameBuf);
	}
	ImGui::SameLine();
	if (ImGui::Button("Load Config"))
	{
		LoadConfig(configNameBuf);
	}
	ImGui::SameLine();
	if (ImGui::Button("Delete Config")) {
		if (selectedConfig >= 0 && selectedConfig < static_cast<int>(configFiles.size())) {
			std::string fileToDelete = Config::getConfigPath(configFiles[selectedConfig]);
			if (std::filesystem::exists(fileToDelete)) {
				std::error_code ec;
				std::filesystem::remove(fileToDelete, ec);
				if (!ec) {
					selectedConfig = -1;
					strncpy_s(configNameBuf, sizeof(configNameBuf), "default", _TRUNCATE);
					configNameBuf[sizeof(configNameBuf) - 1] = '\0';
				}
			}
		}
	}
	if (ImGui::Button("Open Config Folder")) {
		ShellExecuteA(nullptr, "open", configDir.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
	}
	ImGui::EndChild();

	ImGui::SameLine();

	ImGui::BeginChild("##ConfigsSettings2", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true);

	ImGui::Text("Config List");
	ImGui::SameLine();
	if (ImGui::Button("Refresh")) {
		configFiles.clear();
		for (const auto& entry : std::filesystem::directory_iterator(configDir)) {
			if (entry.is_regular_file() && entry.path().extension() == ".json") {
				configFiles.push_back(entry.path().stem().string());
			}
		}
	}
	ImGui::Separator();

	for (size_t i = 0; i < configFiles.size(); ++i) {
		if (ImGui::Selectable(configFiles[i].c_str(), selectedConfig == static_cast<int>(i))) {
			selectedConfig = static_cast<int>(i);
			strncpy_s(configNameBuf, sizeof(configNameBuf), configFiles[i].c_str(), _TRUNCATE);
			configNameBuf[sizeof(configNameBuf) - 1] = '\0';
		}
	}

	ImGui::EndChild();
}

json Config::SerializeCheatConfig() {
	json j;

	j["Aimbot"] = {
		{"bMasterToggle", Aimbot::bMasterToggle},
		{"bDrawFOV", Aimbot::bDrawFOV},
		{"fDampen", Aimbot::fDampen},
		{"fPixelFOV", Aimbot::fPixelFOV},
		{"fDeadzoneFov", Aimbot::fDeadzoneFov},
		{"m_Keybind", Aimbot::m_Keybind}
	};

	j["Fuser"] = {
		{"bMasterToggle", Fuser::bMasterToggle},
		{"bRenderFPS", Fuser::bRenderFPS},
		{"ScreenSize", {Fuser::m_ScreenSize.x, Fuser::m_ScreenSize.y}},

		{"Player", {
			{"bNameText", DrawESPPlayers::bNameText},
			{"bSkeleton", DrawESPPlayers::bSkeleton},
			{"bHeadDot", DrawESPPlayers::bHeadDot},
			{"bBox", DrawESPPlayers::bBox},
		}},

		{"Loot", {
			{"bMasterToggle", DrawESPLoot::bMasterToggle},
			{"fMaxDistance", DrawESPLoot::fMaxDistance},
			{"m_ToggleLootESP", DrawESPLoot::m_ToggleLootESP},
		}},

	};

	j["Radar"] = {
		{"bMasterToggle", Radar::bMasterToggle},
		{"bLocalViewRay", Radar::bLocalViewRay},
		{"bOtherPlayerViewRays", Radar::bOtherPlayerViewRays},
		{"fScale", Radar::fScale},
		{"fLocalViewRayLength", Radar::fLocalViewRayLength},
		{"fOtherViewRayLength", Radar::fOtherViewRayLength},
		{"m_RadarBackgroundColor", static_cast<uint32_t>(Radar::m_RadarBackgroundColor)},
	};

	j["Colors"] = {
		{"m_PMCColor", static_cast<uint32_t>(ColorPicker::m_PMCColor)},
		{"m_ScavColor", static_cast<uint32_t>(ColorPicker::m_ScavColor)},
		{"m_PlayerScavColor", static_cast<uint32_t>(ColorPicker::m_PlayerScavColor)},
		{"m_LocalPlayerColor", static_cast<uint32_t>(ColorPicker::m_LocalPlayerColor)},
		{"m_BossColor", static_cast<uint32_t>(ColorPicker::m_BossColor)},
		{"m_LootColor", static_cast<uint32_t>(ColorPicker::m_LootColor)},
		{"m_ValuableLootColor", static_cast<uint32_t>(ColorPicker::m_ValuableLootColor)},

	};

	return j;
}

void Config::DeserializeCheatConfig(const json& j) {

	if (j.contains("Aimbot")) {
		const auto& aimbotTable = j["Aimbot"];

		if (aimbotTable.contains("bMasterToggle")) {
			Aimbot::bMasterToggle = aimbotTable["bMasterToggle"].get<bool>();
		}
		if (aimbotTable.contains("fDampen")) {
			Aimbot::bDrawFOV = aimbotTable["bDrawFOV"].get<bool>();
		}
		if (aimbotTable.contains("fDampen")) {
			Aimbot::fDampen = aimbotTable["fDampen"].get<float>();
		}
		if (aimbotTable.contains("fPixelFOV")) {
			Aimbot::fPixelFOV = aimbotTable["fPixelFOV"].get<float>();
		}
		if (aimbotTable.contains("fDeadzoneFov")) {
			Aimbot::fDeadzoneFov = aimbotTable["fDeadzoneFov"].get<float>();
		}
		if (aimbotTable.contains("m_Keybind")) {
			Aimbot::m_Keybind = aimbotTable["m_Keybind"].get<uint32_t>();
		}
	}

	if (j.contains("Fuser")) {
		const auto& fuserTable = j["Fuser"];

		if (fuserTable.contains("bMasterToggle")) {
			Fuser::bMasterToggle = fuserTable["bMasterToggle"].get<bool>();
		}
		if (fuserTable.contains("bRenderWatermark")) {
			Fuser::bRenderFPS = fuserTable["bRenderFPS"].get<bool>();
		}
		if (fuserTable.contains("ScreenSize")) {
			const auto& screenSizeJson = fuserTable["ScreenSize"];
			if (screenSizeJson.is_array() && screenSizeJson.size() == 2) {
				Fuser::m_ScreenSize = ImVec2(screenSizeJson[0].get<float>(), screenSizeJson[1].get<float>());
			}
		}

		if (fuserTable.contains("Player")) {
			const auto& PlayerTable = fuserTable["Player"];

			if (PlayerTable.contains("bNameText")) {
				DrawESPPlayers::bNameText = PlayerTable["bNameText"].get<bool>();
			}
			if (PlayerTable.contains("bSkeleton")) {
				DrawESPPlayers::bSkeleton = PlayerTable["bSkeleton"].get<bool>();
			}
			if (PlayerTable.contains("bHeadDot")) {
				DrawESPPlayers::bHeadDot = PlayerTable["bHeadDot"].get<bool>();
			}
			if (PlayerTable.contains("bBox")) {
				DrawESPPlayers::bBox = PlayerTable["bBox"].get<bool>();
			}
		}

		if (fuserTable.contains("Loot")) {
			const auto& LootTable = fuserTable["Loot"];

			if (LootTable.contains("bMasterToggle")) {
				DrawESPLoot::bMasterToggle = LootTable["bMasterToggle"].get<bool>();
			}
			if (LootTable.contains("fMaxDistance")) {
				DrawESPLoot::fMaxDistance = LootTable["fMaxDistance"].get<float>();
			}
			if (LootTable.contains("m_ToggleLootESP")) {
				DrawESPLoot::m_ToggleLootESP = LootTable["m_ToggleLootESP"].get<uint32_t>();
			}
		}
	}

	if (j.contains("Radar")) {
		const auto& radarTable = j["Radar"];

		if (radarTable.contains("bMasterToggle")) {
			Radar::bMasterToggle = radarTable["bMasterToggle"].get<bool>();
		}
		if (radarTable.contains("bLocalViewRay")) {
			Radar::bLocalViewRay = radarTable["bLocalViewRay"].get<bool>();
		}
		if (radarTable.contains("bOtherPlayerViewRays")) {
			Radar::bOtherPlayerViewRays = radarTable["bOtherPlayerViewRays"].get<bool>();
		}
		if (radarTable.contains("fScale")) {
			Radar::fScale = radarTable["fScale"].get<float>();
		}
		if (radarTable.contains("fLocalViewRayLength")) {
			Radar::fLocalViewRayLength = radarTable["fLocalViewRayLength"].get<float>();
		}
		if (radarTable.contains("fOtherViewRayLength")) {
			Radar::fOtherViewRayLength = radarTable["fOtherViewRayLength"].get<float>();
		}
		if (radarTable.contains("m_RadarBackgroundColor")) {
			Radar::m_RadarBackgroundColor = radarTable["m_RadarBackgroundColor"].get<ImU32>();
		}
	}

	if (j.contains("Colors")) {
		const auto& colorsTable = j["Colors"];

		if (colorsTable.contains("m_PMCColor")) {
			ColorPicker::m_PMCColor = colorsTable["m_PMCColor"].get<ImU32>();
		}
		if (colorsTable.contains("m_ScavColor")) {
			ColorPicker::m_ScavColor = colorsTable["m_ScavColor"].get<ImU32>();
		}
		if (colorsTable.contains("m_PlayerScavColor")) {
			ColorPicker::m_PlayerScavColor = colorsTable["m_PlayerScavColor"].get<ImU32>();
		}
		if (colorsTable.contains("m_LocalPlayerColor")) {
			ColorPicker::m_LocalPlayerColor = colorsTable["m_LocalPlayerColor"].get<ImU32>();
		}
		if (colorsTable.contains("m_BossColor")) {
			ColorPicker::m_BossColor = colorsTable["m_BossColor"].get<ImU32>();
		}
		if (colorsTable.contains("m_LootColor")) {
			ColorPicker::m_LootColor = colorsTable["m_LootColor"].get<ImU32>();
		}
		if (colorsTable.contains("m_ValuableLootColor")) {
			ColorPicker::m_ValuableLootColor = colorsTable["m_ValuableLootColor"].get<ImU32>();
		}
	}

}

void Config::SaveConfig(const std::string& configName) {
	std::println("[+] Saving config: {}", configName);
	json j = SerializeCheatConfig();
	std::ofstream file(getConfigPath(configName));
	if (!file.is_open())
		return;
	file << std::setw(4) << j;
	file.close();
}

bool Config::LoadConfig(const std::string& configName) {
	std::println("[+] Loading config: {}", configName);
	std::ifstream file(getConfigPath(configName));
	if (!file.is_open())
	{
		std::println("[#] Failed to open config file: {}", getConfigPath(configName));
		return false;
	}
	json j;
	try {
		file >> j;
	}
	catch (...) {
		file.close();
		return false;
	}
	file.close();
	DeserializeCheatConfig(j);
	return true;
}