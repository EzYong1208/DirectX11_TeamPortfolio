#include "stdafx.h"
#include "Merged_Bird.h"
#include "GameInstance.h"
#include "Level_Loading.h"

CMerged_Bird::CMerged_Bird(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CMerged_Bird::CMerged_Bird(const CMerged_Bird& rhs)
	: CGameObject(rhs)
{
}

HRESULT CMerged_Bird::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMerged_Bird::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(nullptr)))
		return E_FAIL;

	//1. 게임인스턴스 준비
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	_uint iCurLevel = pGameInstance->Get_CurrentLevelIndex();

	//2. 예외처리
	if (iCurLevel == LEVEL_LOGO ||
		iCurLevel == LEVEL_MAIN_MENU)
	{
		MSGBOX("CMerged_Bird Error : CurLevel is invailed");
		return E_FAIL;
	}

	//3. 로딩 레벨의 경우
	if (iCurLevel == LEVEL_LOADING)
	{
		iCurLevel = dynamic_cast<CLevel_Loading*>(pGameInstance->Get_LoadingLevel())->Get_NextLevel();
	}
	RELEASE_INSTANCE(CGameInstance);

	m_iCurLevel = iCurLevel;

	SetUpBird();

	return S_OK;
}

void CMerged_Bird::SetUpBird()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	for (_uint i = 0; i < 15; ++i)
	{
		pGameInstance->Add_GameObject(
			m_iCurLevel,
			TEXT("Layer_Deco"),
			TEXT("Prototype_GameObject_Deco_Bird"));
	}

	RELEASE_INSTANCE(CGameInstance);
}

CMerged_Bird* CMerged_Bird::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CMerged_Bird* pInstance = new CMerged_Bird(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CMerged_Bird");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMerged_Bird::Clone(void* pArg)
{
	CMerged_Bird* pInstance = new CMerged_Bird(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CMerged_Bird");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMerged_Bird::Free()
{
	__super::Free();
}
