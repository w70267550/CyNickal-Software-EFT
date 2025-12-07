#pragma once
#include "Game/Classes/Players/CBaseEFTPlayer/CBaseEFTPlayer.h"

class DrawESPPlayers
{
public:
	static void DrawAll(const ImVec2& WindowPos, ImDrawList* DrawList);

private:
	static void Draw(const CBaseEFTPlayer& Player, const ImVec2& WindowPos, ImDrawList* DrawList);

private: 
	static inline Vector3 m_LatestLocalPlayerPos{};
};