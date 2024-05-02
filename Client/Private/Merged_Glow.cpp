#include "stdafx.h"
#include "Merged_Glow.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "Deco_Glow.h"

CMerged_Glow::CMerged_Glow(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CMerged_Glow::CMerged_Glow(const CMerged_Glow& rhs)
	: CGameObject(rhs)
{
}

HRESULT CMerged_Glow::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMerged_Glow::NativeConstruct(void* pArg)
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
		MSGBOX("CMerged_Glow Error : CurLevel is invailed");
		return E_FAIL;
	}

	//3. 로딩 레벨의 경우
	if (iCurLevel == LEVEL_LOADING)
	{
		iCurLevel = dynamic_cast<CLevel_Loading*>(pGameInstance->Get_LoadingLevel())->Get_NextLevel();
	}
	RELEASE_INSTANCE(CGameInstance);

	m_iCurLevel = iCurLevel;

    DEBUG_BREAK(SetUpGlow());

	return S_OK;
}

HRESULT CMerged_Glow::SetUpGlow()
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    for (_uint i = 0; i < 10; ++i)
   {
       CDeco_Glow::GLOWDESC    GlowDesc;
       ZeroMemory(&GlowDesc, sizeof(CDeco_Glow::GLOWDESC));

       GlowDesc.eGlowArea = CDeco_Glow::AREA_UP;
       GlowDesc.eGlowMoveDir = CDeco_Glow::DIR_UP;
       if (FAILED(pGameInstance->Add_GameObject(
           m_iCurLevel,
           TEXT("Layer_Deco"),
           TEXT("Prototype_GameObject_Deco_Glow"),
           &GlowDesc)))
           return E_FAIL;

       GlowDesc.eGlowMoveDir = CDeco_Glow::DIR_DOWN;
       if (FAILED(pGameInstance->Add_GameObject(
           m_iCurLevel,
           TEXT("Layer_Deco"),
           TEXT("Prototype_GameObject_Deco_Glow"),
           &GlowDesc)))
           return E_FAIL;
   }

   for (_uint i = 0; i < 15; ++i)
   {
       CDeco_Glow::GLOWDESC    GlowDesc;
       ZeroMemory(&GlowDesc, sizeof(CDeco_Glow::GLOWDESC));

       GlowDesc.eGlowArea = CDeco_Glow::AREA_DOWN;
       GlowDesc.eGlowMoveDir = CDeco_Glow::DIR_UP;
       if (FAILED(pGameInstance->Add_GameObject(
           m_iCurLevel,
           TEXT("Layer_Deco"),
           TEXT("Prototype_GameObject_Deco_Glow"),
           &GlowDesc)))
           return E_FAIL;

       GlowDesc.eGlowMoveDir = CDeco_Glow::DIR_DOWN;
       if (FAILED(pGameInstance->Add_GameObject(
           m_iCurLevel,
           TEXT("Layer_Deco"),
           TEXT("Prototype_GameObject_Deco_Glow"),
           &GlowDesc)))
           return E_FAIL;
   }

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

CMerged_Glow* CMerged_Glow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
    CMerged_Glow* pInstance = new CMerged_Glow(pDevice, pDeviceContext);

    if (FAILED(pInstance->NativeConstruct_Prototype()))
    {
        MSGBOX("Failed to Created CMerged_Glow");
        Safe_Release(pInstance);
    }
    return pInstance;
}

CGameObject* CMerged_Glow::Clone(void* pArg)
{
    CMerged_Glow* pInstance = new CMerged_Glow(*this);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        MSGBOX("Failed to Created CMerged_Glow");
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CMerged_Glow::Free()
{
    __super::Free();
}
