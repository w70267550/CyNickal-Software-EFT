#pragma once

class LootTable
{
public:
	static void Render();

public:
	static inline bool bValuableOnly{ false };
	static inline ImGuiTextFilter m_LootFilter{};
};