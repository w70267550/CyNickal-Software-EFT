#include "pch.h"
#include "Fuser.h"
#include "Draw/Players.h"
#include "Draw/Loot.h"
#include "GUI/Aimbot/Aimbot.h"

void Fuser::Render()
{
	if (!bMasterToggle) return;

	ImGui::SetNextWindowSize(Fuser::m_ScreenSize);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 255.0f));
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::Begin("Fuser", nullptr, ImGuiWindowFlags_NoDecoration);
	auto WindowPos = ImGui::GetWindowPos();
	auto DrawList = ImGui::GetWindowDrawList();

	Fuser::RenderWatermark();

	DrawESPPlayers::DrawAll(WindowPos, DrawList);
	DrawESPLoot::DrawAll(WindowPos, DrawList);
	Aimbot::RenderFOVCircle(WindowPos, DrawList);

	ImGui::End();
	ImGui::PopStyleColor();
}

void Fuser::RenderSettings()
{
	ImGui::Checkbox("Master Toggle", &bMasterToggle);
	ImGui::Checkbox("Player Names", &DrawESPPlayers::bNameText);
	ImGui::Checkbox("Player Skeletons", &DrawESPPlayers::bSkeleton);
	ImGui::Checkbox("Player Boxes", &DrawESPPlayers::bBox);
	ImGui::Checkbox("Player Head Dots", &DrawESPPlayers::bHeadDot);
	ImGui::Checkbox("Loot", &DrawESPLoot::bMasterToggle);
	ImGui::InputScalar("Fuser Loot ESP Key", ImGuiDataType_U32, &DrawESPLoot::m_ToggleLootESP);
	ImGui::SliderFloat("Loot Max Distance", &DrawESPLoot::fMaxDistance, 1.0f, 100.0f, "%.1f m");
	DrawESPLoot::m_LootFilter.Draw("Loot Filter");
	ImGui::InputFloat("Screen Width", &Fuser::m_ScreenSize.x);
	ImGui::InputFloat("Screen Height", &Fuser::m_ScreenSize.y);
	ImGui::Checkbox("Render Watermark", &bRenderWatermark);
}

ImVec2 Fuser::GetCenterScreen()
{
	return { m_ScreenSize.x * .5f, m_ScreenSize.y * .5f };
}

void Fuser::RenderWatermark()
{
	if (!bRenderWatermark) return;

	ImVec2 boxPos = ImVec2(10, 10);
	ImVec2 boxSize = ImVec2(175, 34);
	ImGui::SetCursorPos(boxPos);

	ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(30, 30, 30, 200));

	ImGui::BeginChild("WatermarkBox", boxSize, true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);
	ImGui::Text("EFT DMA |");
	ImGui::SameLine();
	// Limit FPS update rate to once per second for readability
	static float lastFps = 0.0f;
	static double lastUpdate = 0.0;
	double currentTime = ImGui::GetTime();
	if (currentTime - lastUpdate > 0.1) {
		auto io = ImGui::GetIO();
		lastFps = io.Framerate;
		lastUpdate = currentTime;
	}
	ImGui::Text("FPS: %.0f", lastFps);
	ImGui::EndChild();
	ImGui::PopStyleColor();
}