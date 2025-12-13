#pragma once
#include "Game/Classes/Players/CBaseEFTPlayer/CBaseEFTPlayer.h"

class AmmoCountOverlay
{
public:
	static void Render();

private:
	static std::pair<uint32_t, uint32_t> GetLocalPlayerAmmoCount();
	static std::pair<uint32_t, uint32_t> GetAmmoCount(CBaseEFTPlayer* Player);
};