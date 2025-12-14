#pragma once
#include "Game/Classes/CBaseLootItem/CBaseLootItem.h"

class CLootableContainer : public CBaseLootItem
{
public:
	CLootableContainer(uintptr_t EntityAddress);
};