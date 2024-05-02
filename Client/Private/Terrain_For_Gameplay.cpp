#include "stdafx.h"
#include "..\Public\Terrain_For_Gameplay.h"
#include "GameInstance.h"
#include "Light.h"
#include "Level_Loading.h"

CTerrain_For_Gameplay::CTerrain_For_Gameplay(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
	for (_uint i = 0; TEXTURETYPE_END > i; ++i)
		m_pTextureCom[i] = nullptr;
}

CTerrain_For_Gameplay::CTerrain_For_Gameplay(const CTerrain_For_Gameplay& rhs)
	: CGameObject(rhs)
{
	for (_uint i = 0; TEXTURETYPE_END > i; ++i)
		m_pTextureCom[i] = nullptr;

}

HRESULT CTerrain_For_Gameplay::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain_For_Gameplay::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct((CTransform::TRANSFORMDESC*)pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	//1. 터레인 크기 늘리기
	m_pTransform->Scaled(XMVectorSet(1.f, 1.f, 1.f, 0.f));
	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(-150.f, 0.f, 0.f, 1.f));

	return S_OK;
}

_int CTerrain_For_Gameplay::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	return _int();
}

_int CTerrain_For_Gameplay::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_FIRST, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_SHADOW, this);

	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

HRESULT CTerrain_For_Gameplay::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render(m_pShaderCom, 0)))
		return E_FAIL;
#ifdef _DEBUG
	m_pNavigation_Line->Render(m_pTransform);
#endif

	return S_OK;
}

HRESULT CTerrain_For_Gameplay::Render_Shadow()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	CPipeLine* pPipeLine = GET_INSTANCE(CPipeLine);
	_float4x4 matLightViewFloat4x4_TP, matLightProjFloat4x4_TP;
	matLightViewFloat4x4_TP = pPipeLine->Get_LightViewFloat4x4_TP();
	matLightProjFloat4x4_TP = pPipeLine->Get_LightProjFloat4x4_TP();
	if (FAILED(m_pShaderCom->Set_RawValue("g_LightViewMatrix", &matLightViewFloat4x4_TP, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_LightProjMatrix", &matLightProjFloat4x4_TP, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &matLightViewFloat4x4_TP, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &matLightProjFloat4x4_TP, sizeof(_float4x4))))
		return E_FAIL;
	RELEASE_INSTANCE(CPipeLine);

	if (FAILED(m_pVIBufferCom->Render(m_pShaderCom, 1)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain_For_Gameplay::SetUp_Components()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	//1. 게임인스턴스 준비
	_uint iCurLevel = pGameInstance->Get_CurrentLevelIndex();


	//2. 예외처리
	if (iCurLevel == LEVEL_LOGO ||
		iCurLevel == LEVEL_MAIN_MENU)
	{
		MSGBOX("CState_Player_Turn Error : CurLevel is invailed");
		return E_FAIL;
	}

	//3. 로딩 레벨의 경우
	if (iCurLevel == LEVEL_LOADING)
	{
		iCurLevel = dynamic_cast<CLevel_Loading*>(pGameInstance->Get_LoadingLevel())->Get_NextLevel();
	}

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(
		LEVEL_STATIC,
		TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"),
		(CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(
		LEVEL_STATIC,
		g_pShaderTag[Shader_VTXRGB_FOR_TERRAIN],
		TEXT("Com_Shader"),
		(CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Navigation_Line */
	CNavigation_Line::NAVIDESC tagNavi;
	ZeroMemory(&tagNavi, sizeof(CNavigation_Line::NAVIDESC));
	tagNavi.iCurrentIndex = -1;
	if (FAILED(__super::Add_Component(
		iCurLevel,
		TEXT("Prototype_Component_Navigation_Line"),
		TEXT("Com_Navigation_Line"),
		(CComponent**)&m_pNavigation_Line,
		&tagNavi)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(
		iCurLevel,
		TEXT("Prototype_Component_VIBuffer_Terrain"),
		TEXT("Com_VIBuffer"),
		(CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture_D */
	if (FAILED(__super::Add_Component(
		LEVEL_STATIC,
		TEXT("Prototype_Component_Texture_Tile_D"),
		TEXT("Com_Texture_D"),
		(CComponent**)&m_pTextureCom[TEXTURETYPE_DIFFUSE])))
		return E_FAIL;

	/* For.Com_Texture_N */
	if (FAILED(__super::Add_Component(
		LEVEL_STATIC,
		TEXT("Prototype_Component_Texture_Tile_N"),
		TEXT("Com_Texture_N"),
		(CComponent**)&m_pTextureCom[TEXTURETYPE_NORMAL])))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CTerrain_For_Gameplay::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransform->Bind_OnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_vCamPosition", &pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	//Diffuse
	if (FAILED(m_pTextureCom[TEXTURETYPE_DIFFUSE]->SetUp_OnShader(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;
	if (FAILED(m_pTextureCom[TEXTURETYPE_DIFFUSE]->SetUp_OnShader(m_pShaderCom, "g_SourDiffuseR", 1)))
		return E_FAIL;
	if (FAILED(m_pTextureCom[TEXTURETYPE_DIFFUSE]->SetUp_OnShader(m_pShaderCom, "g_SourDiffuseG", 2)))
		return E_FAIL;
	if (FAILED(m_pTextureCom[TEXTURETYPE_DIFFUSE]->SetUp_OnShader(m_pShaderCom, "g_SourDiffuseB", 3)))
		return E_FAIL;

	//Normal
	if (FAILED(m_pTextureCom[TEXTURETYPE_NORMAL]->SetUp_OnShader(m_pShaderCom, "g_NormalTexture", 0)))
		return E_FAIL;
	if (FAILED(m_pTextureCom[TEXTURETYPE_NORMAL]->SetUp_OnShader(m_pShaderCom, "g_SourNormalR", 1)))
		return E_FAIL;
	if (FAILED(m_pTextureCom[TEXTURETYPE_NORMAL]->SetUp_OnShader(m_pShaderCom, "g_SourNormalG", 2)))
		return E_FAIL;
	if (FAILED(m_pTextureCom[TEXTURETYPE_NORMAL]->SetUp_OnShader(m_pShaderCom, "g_SourNormalB", 3)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	// 모션 블러
	CPipeLine* pPipeLine = GET_INSTANCE(CPipeLine);

	_float4x4 OldWorldMatrix;
	XMStoreFloat4x4(&OldWorldMatrix, XMMatrixTranspose(m_pTransform->Get_OldWorldMatrix()));
	if (FAILED(m_pShaderCom->Set_RawValue("g_OldWorldMatrix", &OldWorldMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_OldViewMatrix", &pPipeLine->Get_OldTransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	RELEASE_INSTANCE(CPipeLine);

	return S_OK;	
}

CTerrain_For_Gameplay* CTerrain_For_Gameplay::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CTerrain_For_Gameplay*	pInstance = new CTerrain_For_Gameplay(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CTerrain_For_Gameplay");
		Safe_Release(pInstance);
	}
	return pInstance;

}

CGameObject* CTerrain_For_Gameplay::Clone(void* pArg)
{
	CTerrain_For_Gameplay*	pInstance = new CTerrain_For_Gameplay(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CTerrain_For_Gameplay");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTerrain_For_Gameplay::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pNavigation_Line);
	for (_uint i = 0; TEXTURETYPE_END > i; ++i)
		Safe_Release(m_pTextureCom[i]);
	Safe_Release(m_pShaderCom);
}
