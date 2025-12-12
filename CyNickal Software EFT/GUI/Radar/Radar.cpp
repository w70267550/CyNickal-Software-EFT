#include "pch.h"
#include "GUI/Radar/Radar.h"
#include "GUI/Radar/Draw/Radar Players.h"
#include "GUI/Radar/Draw/Radar Loot.h"
#include "GUI/Color Picker/Color Picker.h"

void Radar::Render()
{
	if (!bMasterToggle) return;

	ImGui::SetNextWindowPos(ImVec2(175, 10), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(350, 350), ImGuiCond_Once);

	ImGui::Begin("Radar", &bMasterToggle);

	auto WindowPos = ImGui::GetWindowPos();
	auto WindowSize = ImGui::GetWindowSize();
	auto DrawList = ImGui::GetWindowDrawList();

	ImVec2 RectTopLeft = WindowPos;
	ImVec2 RectBottomRight = ImVec2(WindowPos.x + WindowSize.x, WindowPos.y + WindowSize.y);

	DrawList->AddRectFilled(RectTopLeft, RectBottomRight, m_RadarBackgroundColor);

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

	ImGui::SetNextItemWidth(150.0f);
	ImGui::ColorEdit4("Radar Background Color", &m_RadarBackgroundColor.Value.x);
}
