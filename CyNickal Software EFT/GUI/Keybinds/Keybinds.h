#include "DMA/DMA.h"


class Keybinds
{
public:
	static void Render();
	static void OnFrame(DMA_Connection* Conn, uintptr_t LocalGameWorld);

public:
	static inline uint32_t m_DebugHotkey = VK_F12;
	static inline uint32_t m_ForceExitHotkey = VK_END;
	static inline uint32_t m_RefreshPlayerList = VK_INSERT;
};