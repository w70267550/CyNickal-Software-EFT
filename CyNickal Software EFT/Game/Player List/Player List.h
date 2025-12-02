#pragma once
#include "Game/Classes/CEFTPlayer.h"
#include "DMA/DMA.h"

class PlayerList
{
public:
	static inline std::mutex m_PlayerMutex{};
	static inline std::vector<CEFTPlayer> m_Players{};
	static inline std::vector<uintptr_t> m_PlayerAddr{};

public:
	static void FullUpdate(DMA_Connection* Conn);
	static void QuickUpdate(DMA_Connection* Conn);
	static void PrintPlayers();
};