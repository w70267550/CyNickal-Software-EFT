#include "pch.h"
#include "CLootableContainer.h"

CLootableContainer::CLootableContainer(uintptr_t EntityAddress) : CBaseLootItem(EntityAddress)
{
	std::println("[CLootableContainer] Constructed with {0:X}", m_EntityAddress);
}
