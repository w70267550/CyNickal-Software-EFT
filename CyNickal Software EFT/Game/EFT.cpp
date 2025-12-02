#include "pch.h"
#include <fstream>
#include "EFT.h"
#include "Classes/CGameObjectManager.h"
#include "Classes/CLinkedListEntry.h"
#include "Classes/CObject.h"
#include "Classes/CPlayer.h"

#include "Player List/Player List.h"

bool EFT::Initialize(DMA_Connection* Conn)
{
	std::println("Initializing EFT module...");

	Proc.GetProcessInfo(Conn);

	uintptr_t pGOMAddress = Proc.GetUnityAddress() + 0x1A208D8;
	GameObjectManagerAddress = Proc.ReadMem<uintptr_t>(Conn, pGOMAddress);
	std::println("GameObjectManager Address: 0x{:X}", GameObjectManagerAddress);

	LastActiveNode = Proc.ReadMem<uintptr_t>(Conn, GameObjectManagerAddress + offsetof(CGameObjectManager, pLastActiveNode));
	std::println("LastActiveNode Address: 0x{:X}", LastActiveNode);

	ActiveNodes = Proc.ReadMem<uintptr_t>(Conn, GameObjectManagerAddress + offsetof(CGameObjectManager, pActiveNodes));
	std::println("ActiveNodes Address: 0x{:X}", ActiveNodes);

	GetObjectAddresses(Conn);

	PopulateObjectInfoListFromAddresses(Conn);

	DumpAllObjectsToFile("ObjectDump.txt");

	uintptr_t LocalGameWorld = GetLocalGameWorldAddr(Conn);

	auto MainPlayer = Proc.ReadMem<uintptr_t>(Conn, LocalGameWorld + offsetof(CLocalGameWorld, pMainPlayer));
	std::println("[EFT] MainPlayer Address: 0x{:X}", MainPlayer);

	auto PlayerMovementContext = Proc.ReadMem<uintptr_t>(Conn, MainPlayer + offsetof(CPlayer, pMovementContext));
	std::println("[EFT] Main PlayerMovementContext Address: 0x{:X}", PlayerMovementContext);

	auto PlayerMovementContextYaw = Proc.ReadMem<float>(Conn, PlayerMovementContext + offsetof(CMovementContext, Yaw));
	std::println("[EFT] Main Player Yaw: {}", PlayerMovementContextYaw);

	PlayerList::m_PlayerAddr.clear();
	PlayerList::m_PlayerAddr.push_back(MainPlayer);
	PlayerList::FullUpdate(Conn);
	PlayerList::PrintPlayers();

	for (int i = 0; i < 100; i++)
	{
		PlayerList::QuickUpdate(Conn);
		PlayerList::PrintPlayers();
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	return false;
}

const Process& EFT::GetProcess()
{
	return Proc;
}

void EFT::GetObjectAddresses(DMA_Connection* Conn, uint32_t MaxNodes)
{
	m_ObjectAddresses.clear();

	auto StartTime = std::chrono::high_resolution_clock::now();
	uint32_t NodeCount = 0;
	DWORD BytesRead = 0;
	uintptr_t CurrentActiveNode = ActiveNodes;
	uintptr_t FirstNode = ActiveNodes;

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), Proc.GetPID(), 0);
	while (CurrentActiveNode != LastActiveNode && CurrentActiveNode && NodeCount <= MaxNodes)
	{
		CLinkedListEntry NodeEntry{};
		VMMDLL_Scatter_PrepareEx(vmsh, CurrentActiveNode, sizeof(CLinkedListEntry), reinterpret_cast<BYTE*>(&NodeEntry), &BytesRead);
		VMMDLL_Scatter_Execute(vmsh);
		VMMDLL_Scatter_Clear(vmsh, Proc.GetPID(), 0);

		if (BytesRead != sizeof(CLinkedListEntry))
		{
			std::println("[EFT] UpdateObjectList; Incomplete read: {0:d}/{1:d} @ {2:X}", BytesRead, sizeof(CLinkedListEntry), CurrentActiveNode);
			break;
		}

		//std::println("[EFT] UpdateObjectList; Found Object at 0x{:X}", NodeEntry.pObject);
		m_ObjectAddresses.push_back(NodeEntry.pObject);

		if (NodeEntry.pNextEntry == FirstNode)
			break;

		CurrentActiveNode = NodeEntry.pNextEntry;
		NodeCount++;
	}

	auto EndTime = std::chrono::high_resolution_clock::now();
	auto Duration = std::chrono::duration_cast<std::chrono::milliseconds>(EndTime - StartTime).count();
	std::println("[EFT] UpdateObjectList; {} nodes in {}ms", NodeCount, Duration);
}

std::vector<uintptr_t> EFT::GetGameWorldAddresses(DMA_Connection* Conn)
{
	std::vector<uintptr_t> GameWorldAddresses{};

	for (auto& ObjInfo : m_ObjectInfo)
	{
		if (ObjInfo.m_ObjectName == "GameWorld")
		{
			std::println("[EFT] GameWorld Address: 0x{:X}", ObjInfo.m_ObjectAddress);
			GameWorldAddresses.push_back(ObjInfo.m_ObjectAddress);
		}
	}

	return GameWorldAddresses;
}

uintptr_t EFT::GetLocalGameWorldAddr(DMA_Connection* Conn)
{
	auto GameWorldAddrs = GetGameWorldAddresses(Conn);

	for (auto& GameWorldAddr : GameWorldAddrs)
	{
		auto Deref1 = Proc.ReadMem<uintptr_t>(Conn, GameWorldAddr + 0x58);
		auto Deref2 = Proc.ReadMem<uintptr_t>(Conn, Deref1 + 0x18);
		auto LocalWorldAddr = Proc.ReadMem<uintptr_t>(Conn, Deref2 + 0x30);
		auto MainPlayerAddr = Proc.ReadMem<uintptr_t>(Conn, LocalWorldAddr + offsetof(CLocalGameWorld, pMainPlayer));
		if (MainPlayerAddr != 0)
		{
			std::println("[EFT] LocalGameWorld Address: 0x{:X}", LocalWorldAddr);
			return LocalWorldAddr;
		}
	}

	throw std::runtime_error("Failed to find valid LocalGameWorld address.");
}

void EFT::DumpAllObjectsToFile(const std::string& FileName)
{
	std::ofstream OutFile(FileName, std::ios::out | std::ios::trunc);
	if (!OutFile.is_open())
	{
		std::println("[EFT] DumpAllObjectsToFile; Failed to open file: {}", FileName);
		return;
	}

	for (int i = 0; i < m_ObjectInfo.size(); i++)
	{
		auto& ObjInfo = m_ObjectInfo[i];
		OutFile << std::format("Entity #{0:d} @ {1:X} named `{2:s}`", i, ObjInfo.m_ObjectAddress, ObjInfo.m_ObjectName.c_str()) << std::endl;
	}

	OutFile.close();

}

std::vector<std::pair<uintptr_t, std::array<std::byte, 32>>> ObjectDataBuffers{};
void EFT::PopulateObjectInfoListFromAddresses(DMA_Connection* Conn)
{
	ObjectDataBuffers.resize(m_ObjectAddresses.size());

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), Proc.GetPID(), VMMDLL_FLAG_NOCACHE);

	for (int i = 0; i < m_ObjectAddresses.size(); i++)
	{
		auto& ObjAddr = m_ObjectAddresses[i];
		uintptr_t NameAddress = ObjAddr + offsetof(CObject, pName);
		VMMDLL_Scatter_PrepareEx(vmsh, NameAddress, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&ObjectDataBuffers[i].first), nullptr);
	}

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_Clear(vmsh, Proc.GetPID(), VMMDLL_FLAG_NOCACHE);

	for (int i = 0; i < m_ObjectAddresses.size(); i++)
	{
		auto& [NameAddress, DataBuffer] = ObjectDataBuffers[i];
		VMMDLL_Scatter_PrepareEx(vmsh, NameAddress, DataBuffer.size(), reinterpret_cast<BYTE*>(DataBuffer.data()), nullptr);
	}

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);

	m_ObjectInfo.clear();
	for (int i = 0; i < m_ObjectAddresses.size(); i++)
	{
		auto& [NameAddress, DataBuffer] = ObjectDataBuffers[i];
		std::string Name(reinterpret_cast<char*>(DataBuffer.data()), strnlen_s(reinterpret_cast<char*>(DataBuffer.data()), DataBuffer.size()));
		CObjectInfo ObjInfo{};
		ObjInfo.m_ObjectAddress = m_ObjectAddresses[i];
		ObjInfo.m_ObjectName = Name;
		m_ObjectInfo.push_back(ObjInfo);
	}
}