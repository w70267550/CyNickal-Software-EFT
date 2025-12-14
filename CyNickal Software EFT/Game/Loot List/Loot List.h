#pragma once
#include "DMA/DMA.h"
#include "Game/Classes/CObservedLootItem/CObservedLootItem.h"
#include "Game/Classes/CLootableContainer/CLootableContainer.h"

class LootList
{
public:
	using Loot = std::variant<CObservedLootItem, CLootableContainer>;

	static inline std::mutex m_LootMutex{};
	static inline std::vector<Loot> m_LootList{};

public:
	static void CompleteUpdate(DMA_Connection* Conn);
	static void FullUpdate(DMA_Connection* Conn);
	static void QuickUpdate(DMA_Connection* Conn);

private:
	static inline uintptr_t m_pLootListAddress{ 0 };
	static inline uintptr_t m_BaseLootListAddress{ 0 };
	static inline uint32_t m_LootNum{ 0 };

private:
	static void GetAndSortEntityAddresses(DMA_Connection* Conn);
	static inline std::vector<uintptr_t> m_UnsortedAddresses{};
	static inline std::vector<uintptr_t> m_LootableContainerAddresses{};
	static inline std::vector<uintptr_t> m_ObservedItemAddresses{};

	static void PopulateTypeAddressCache(DMA_Connection* Conn);
	static inline std::unordered_map<std::string, uintptr_t> ObjectTypeAddressCache{};
};