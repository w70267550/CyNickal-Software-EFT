#pragma once

class Fuser
{
public:
	static void Render();
	static void RenderSettings();
	static ImVec2 GetCenterScreen();
	static void RenderWatermark();


public:
	static inline bool bMasterToggle{ true };
	static inline bool bRenderWatermark{ true };
	static inline ImVec2 m_ScreenSize{ 1920.0f,1080.0f };

private:
};