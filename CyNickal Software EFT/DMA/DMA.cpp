#include "pch.h"

#include "DMA.h"

#include <filesystem>
#include <fstream>

DMA_Connection* DMA_Connection::GetInstance()
{
	if (m_Instance == nullptr)
		m_Instance = new DMA_Connection();

	return m_Instance;
}

VMM_HANDLE DMA_Connection::GetHandle()
{
	return m_VMMHandle;
}

bool DMA_Connection::EndConnection()
{
	this->~DMA_Connection();

	return true;
}

DMA_Connection::DMA_Connection()
{
	std::println("[-] Connecting to DMA...");

	LPCSTR args[8] = { "", "-device", "fpga://algo=0", "-norefresh" };
	DWORD argc = 4;

	// Dump memory map
	std::string path = "mmap.txt";
	if (true)
	{
		bool dumped = false;
		if (!std::filesystem::exists(path))
		{
			std::println("[+] Dumping memory map to {}...", path);
			dumped = this->DumpMemoryMap(false);
		}
		else
		{
			std::println("[+] Memory map file {} already exists. No need to dump", path);
			dumped = true;
		}

		if (!dumped)
		{
			std::println("[#] Failed to dump memory map.");
		}
		else
		{
			args[argc++] = "-memmap";
			args[argc++] = path.c_str();
		}
	}

	m_VMMHandle = VMMDLL_Initialize(argc, args);

	if (!m_VMMHandle)
		throw std::runtime_error("Failed to initialize VMM DLL");

	// Get FPGA check Validation
	if (!this->GetFPGAInfo())
	{
		std::println("[#] FPGA validation failed. Disconnecting...");
	}

	std::println("[+] Connected to DMA!");
}

DMA_Connection::~DMA_Connection()
{
	VMMDLL_Close(m_VMMHandle);

	m_VMMHandle = nullptr;

	std::println("Disconnected from DMA!");
}

bool DMA_Connection::GetFPGAInfo()
{
	bool result;
	ULONG64 qwID, qwVersionMajor, qwVersionMinor;
	if (VMMDLL_ConfigGet(this->m_VMMHandle, LC_OPT_FPGA_FPGA_ID, &qwID) && VMMDLL_ConfigGet(this->m_VMMHandle, LC_OPT_FPGA_VERSION_MAJOR, &qwVersionMajor) && VMMDLL_ConfigGet(this->m_VMMHandle, LC_OPT_FPGA_VERSION_MINOR, &qwVersionMinor))
	{
		// Get FPGA and Device IDs
		ULONG64 FPGA_ID, DEVICE_ID;
		VMMDLL_ConfigGet(this->m_VMMHandle, LC_OPT_FPGA_FPGA_ID, &FPGA_ID);
		VMMDLL_ConfigGet(this->m_VMMHandle, LC_OPT_FPGA_DEVICE_ID, &DEVICE_ID);

		std::println("[+] FPGA ID: {}, Device ID: {} Version: {}.{}", FPGA_ID, DEVICE_ID, qwVersionMajor, qwVersionMinor);

		if ((qwVersionMajor >= 4) && ((qwVersionMajor >= 5) || (qwVersionMinor >= 7)))
		{
			HANDLE handle;
			LC_CONFIG config = { .dwVersion = LC_CONFIG_VERSION, .szDevice = "existing" };
			handle = LcCreate(&config);
			if (handle)
			{
				unsigned char abort[4] = { 0x10, 0x00, 0x10, 0x00 };
				LcCommand(handle, LC_CMD_FPGA_CFGREGPCIE_MARKWR | 0x002, 4, (PBYTE)&abort, NULL, NULL);
				LcClose(handle);
			}
		}
	}
	else
	{
		std::println("[#] Failed to get FPGA Info.");
		return false;
	}
	return true;
}

bool DMA_Connection::DumpMemoryMap(bool debug)
{
	LPCSTR args[8] = { "", "-device", "fpga://algo=0", "", "" };
	int argc = 3;
	if (debug)
	{
		args[argc++] = "-v";
		args[argc++] = "-printf";
	}
	VMM_HANDLE handle = VMMDLL_Initialize(argc, args);
	if (!handle)
	{
		return false;
	}
	PVMMDLL_MAP_PHYSMEM pPhysMemMap = NULL;
	if (VMMDLL_Map_GetPhysMem(handle, &pPhysMemMap))
	{
		if (pPhysMemMap->dwVersion != VMMDLL_MAP_PHYSMEM_VERSION)
		{
			VMMDLL_MemFree(pPhysMemMap);
			VMMDLL_Close(handle);
			return false;
		}

		if (pPhysMemMap->cMap == 0)
		{
			std::println("[#] No physical memory map entries found.");
			VMMDLL_MemFree(pPhysMemMap);
			VMMDLL_Close(handle);
			return false;
		}
		//Dump map to file
		std::stringstream sb;
		for (DWORD i = 0; i < pPhysMemMap->cMap; i++)
		{
			sb << std::setfill('0') << std::setw(4) << i << "  " << std::hex << pPhysMemMap->pMap[i].pa << "  -  " << (pPhysMemMap->pMap[i].pa + pPhysMemMap->pMap[i].cb - 1) << "  ->  " << pPhysMemMap->pMap[i].pa << std::endl;
		}
		std::ofstream nFile("mmap.txt");
		nFile << sb.str();
		nFile.close();

		VMMDLL_MemFree(pPhysMemMap);

	}
	VMMDLL_Close(handle); // Disconnect from DMA reconnect with memory map

	Sleep(3000); // Wait for the DMA to be ready again

	return true;
}

void DMA_Connection::LightRefreshWrapper(DMA_Connection* Conn)
{
	VMMDLL_ConfigSet(Conn->GetHandle(), VMMDLL_OPT_REFRESH_FREQ_MEM_PARTIAL, 1);
	std::println("[+] Performed partial cache refresh on DMA.");
}