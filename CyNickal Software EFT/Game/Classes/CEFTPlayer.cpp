#include "pch.h"
#include "CEFTPlayer.h"
#include "CPlayer.h"

void CEFTPlayer::SetInvalid()
{
	m_Flags |= 0x1;
}

bool CEFTPlayer::IsInvalid() const
{
	return m_Flags & 0x1;
}

void CEFTPlayer::PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh)
{
	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + offsetof(CPlayer, pPlayerBody), sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_PlayerBodyAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CEFTPlayer::PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t) || !m_PlayerBodyAddress)
		SetInvalid();

	if (IsInvalid())
		return;

	std::println("[CEFTPlayer] PlayerBody Address: 0x{:X}", m_PlayerBodyAddress);
	VMMDLL_Scatter_PrepareEx(vmsh, m_PlayerBodyAddress + offsetof(CPlayerBody, pSkeleton), sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_SkeletonRootAddress), reinterpret_cast<DWORD*>(&m_BytesRead));

}

void CEFTPlayer::PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t) || !m_SkeletonRootAddress)
		SetInvalid();

	if (IsInvalid())
		return;

	std::println("[CEFTPlayer] SkeletonRoot Address: 0x{:X}", m_SkeletonRootAddress);
	VMMDLL_Scatter_PrepareEx(vmsh, m_SkeletonRootAddress + offsetof(CSkeleton, pValues), sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_ValuesArrayAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CEFTPlayer::PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t) || !m_ValuesArrayAddress)
		SetInvalid();

	if (IsInvalid())
		return;

	std::println("[CEFTPlayer] ValuesArray Address: 0x{:X}", m_ValuesArrayAddress);
	VMMDLL_Scatter_PrepareEx(vmsh, m_ValuesArrayAddress + offsetof(CValues, pArr1), sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_Arr1Address), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CEFTPlayer::PrepareRead_5(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t) || !m_Arr1Address)
		SetInvalid();

	if (IsInvalid())
		return;

	std::println("[CEFTPlayer] m_Arr1Address: 0x{:X}", m_Arr1Address);
	VMMDLL_Scatter_PrepareEx(vmsh, m_Arr1Address + offsetof(CArr1, pBoneTransforms), sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_BoneTransformsAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CEFTPlayer::PrepareRead_6(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t) || !m_BoneTransformsAddress)
		SetInvalid();

	if (IsInvalid())
		return;

	std::println("[CEFTPlayer] m_BoneTransformsAddress: 0x{:X}", m_BoneTransformsAddress);
	VMMDLL_Scatter_PrepareEx(vmsh, m_BoneTransformsAddress + offsetof(CBoneTransforms, pBaseTransform), sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_BasePositionTransformAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CEFTPlayer::PrepareRead_7(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t) || !m_BasePositionTransformAddress)
		SetInvalid();

	if (IsInvalid())
		return;

	std::println("[CEFTPlayer] m_BasePositionTransformAddress: 0x{:X}", m_BasePositionTransformAddress);
	VMMDLL_Scatter_PrepareEx(vmsh, m_BasePositionTransformAddress + offsetof(CUnityTransform, pTransformHierarchy), sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_TransformHierarchyAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CEFTPlayer::PrepareRead_8(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t) || !m_BasePositionTransformAddress)
		SetInvalid();

	if (IsInvalid())
		return;

	std::println("[CEFTPlayer] m_TransformHierarchyAddress: 0x{:X}", m_TransformHierarchyAddress);
	VMMDLL_Scatter_PrepareEx(vmsh, m_TransformHierarchyAddress + offsetof(CTransformHierarchy, Position), sizeof(Vector3), reinterpret_cast<BYTE*>(&m_BasePosition), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CEFTPlayer::Finalize()
{
	if (m_BytesRead != sizeof(Vector3))
		SetInvalid();
}

void CEFTPlayer::QuickRead(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_TransformHierarchyAddress + offsetof(CTransformHierarchy, Position), sizeof(Vector3), reinterpret_cast<BYTE*>(&m_BasePosition), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CEFTPlayer::QuickFinalize()
{
	if (m_BytesRead != sizeof(Vector3))
		SetInvalid();
}