#pragma once

class LootTable
{
public:
	static void Render();

public:
	static inline bool bMasterToggle{ true };
	static inline ImGuiTextFilter m_LootFilter{};
};