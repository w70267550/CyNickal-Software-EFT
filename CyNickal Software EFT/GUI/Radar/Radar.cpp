#include "pch.h"
#include "GUI/Radar/Radar.h"
#include "GUI/Radar/Draw/Radar Players.h"
#include "GUI/Radar/Draw/Radar Loot.h"
#include "GUI/Color Picker/Color Picker.h"
#include <algorithm>

void Radar::Render()
{
	if (!bMasterToggle) return;

	ImGui::SetNextWindowPos(ImVec2(175, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(350, 350), ImGuiCond_FirstUseEver);

	ImGui::Begin("Radar", &bMasterToggle);

	auto& io = ImGui::GetIO();
	if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) && io.MouseWheel != 0.0f)
	{
		const float sensitivity = 0.1f;
		fScale += io.MouseWheel * sensitivity;
		fScale = std::clamp(fScale, 0.1f, 5.0f);
	}

	auto WindowPos = ImGui::GetWindowPos();
	auto WindowSize = ImGui::GetWindowSize();
	auto DrawList = ImGui::GetWindowDrawList();

	ImVec2 RectTopLeft = WindowPos;
	ImVec2 RectBottomRight = ImVec2(WindowPos.x + WindowSize.x, WindowPos.y + WindowSize.y);

	DrawList->AddRectFilled(RectTopLeft, RectBottomRight, ColorPicker::m_RadarBackgroundColor);

	DrawRadarLoot::DrawAll(WindowPos, WindowSize, DrawList);
	DrawRadarPlayers::DrawAll(WindowPos, WindowSize, DrawList);

	ImGui::End();
}

void Radar::RenderSettings()
{
	ImGui::Checkbox("Master Toggle", &bMasterToggle);
	ImGui::SliderFloat("Scale", &Radar::fScale, 0.1f, 5.0f, "%.1f");
	ImGui::SliderFloat("Local View Ray Length", &Radar::fLocalViewRayLength, 10.0f, 500.0f, "%.1f");
	ImGui::SliderFloat("Other View Ray Length", &Radar::fOtherViewRayLength, 10.0f, 500.0f, "%.1f");
	ImGui::Checkbox("Local Player View Ray", &Radar::bLocalViewRay);
	ImGui::Checkbox("Players View Rays", &Radar::bOtherPlayerViewRays);
	ImGui::Checkbox("Loot", &DrawRadarLoot::bMasterToggle);
}
