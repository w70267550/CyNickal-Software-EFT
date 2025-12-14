#pragma once
#include "Game/Classes/CLootableContainer/CLootableContainer.h"
#include "Game/Classes/CObservedLootItem/CObservedLootItem.h"
class DrawESPLoot
{
public:
	static void DrawAll(const ImVec2& WindowPos, ImDrawList* DrawList);
	static void DrawSettings();

private:
	static void DrawLoot(CObservedLootItem& Item, ImDrawList* DrawList, ImVec2 WindowPos, Vector3 LocalPlayerPos);
	static void DrawLoot(CLootableContainer& Container, ImDrawList* DrawList, ImVec2 WindowPos, Vector3 LocalPlayerPos);

public:
	static inline bool bMasterToggle{ true };
	static inline bool bItemToggle{ true };
	static inline bool bContainerToggle{ true };
	static inline float fMaxItemDistance{ 50.0f };
	static inline float fMaxContainerDistance{ 20.0f };
	static inline int32_t m_MinItemPrice{ -1 };
};