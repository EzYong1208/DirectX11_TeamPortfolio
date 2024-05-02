#include "stdafx.h"
#include "Effect_IRukaSan_Roll.h"
#include "GameInstance.h"
#include "Effect_None_Anim_Effect.h"
#include "Player_May.h"

CEffect_IRukaSan_Roll::CEffect_IRukaSan_Roll(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CEffect_Anim_Effect(pDevice, pDeviceContext)
{
}

CEffect_IRukaSan_Roll::CEffect_IRukaSan_Roll(const CEffect_IRukaSan_Roll& rhs)
	: CEffect_Anim_Effect(rhs)
{
}

HRESULT CEffect_IRukaSan_Roll::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_IRukaSan_Roll::NativeConstruct(void* pArg)
{
	//	트랜스폼, 모델, 셰이더, 랜더러 추가
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	//	트레일 텍스처 추가
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	XMStoreFloat3(&m_EffectDesc.vScaleOrigin, m_pTransform->Get_Scale());

	return S_OK;
}

_int CEffect_IRukaSan_Roll::Tick(_double TimeDelta)
{
	int temp = 0;

	m_fTime += (_float)TimeDelta * m_EffectDesc.fSpeed;
	m_fTimeDelta += TimeDelta * 4.f;

	switch (m_iSequence)
	{
	case 0:

		if (m_EffectDesc.fLifetime >= 0.f)
			m_EffectDesc.fLifetime -= (_float)TimeDelta;
		else
			temp = ISDEAD;

		break;

	default:
		break;
	}

	m_pTransform->Reset_Momentum();

	if (0.35f >= m_EffectDesc.fLifetime &&
		!m_bCreateOnce)
		DEBUG_BREAK(Create_Effect_Splash());
		

	return temp;
}

_int CEffect_IRukaSan_Roll::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_SECOND, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_OUTLINE, this);

	return 0;
}

HRESULT CEffect_IRukaSan_Roll::Render()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix WorldMatrix;

	WorldMatrix = XMLoadFloat4x4(&m_EffectDesc.matBillboard);

	_float4x4 BillboardMatrix;

	//XMStoreFloat4x4(&BillboardMatrix,
	//	XMMatrixRotationRollPitchYaw(XMConvertToRadians(90.f), XMConvertToRadians(-80.f), XMConvertToRadians(0.f)) *
	//	XMMatrixTranslation(0.75f, 1.3f, 1.3f)
	//	* WorldMatrix);

	XMStoreFloat4x4(&BillboardMatrix,
		XMMatrixRotationRollPitchYaw(XMConvertToRadians(90.f + (m_fTimeDelta * 170)), XMConvertToRadians(-80.f), XMConvertToRadians(0.f)) *
		XMMatrixTranslation(0.75f - (m_fTimeDelta * 0.7f), 1.3f - (m_fTimeDelta * 1.2f), 1.3f)
		* WorldMatrix);

	_float4x4 ResultWorldMatrix;

	XMStoreFloat4x4(&ResultWorldMatrix, XMMatrixTranspose(XMLoadFloat4x4(&BillboardMatrix)));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &ResultWorldMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);


	_uint iNumMaterials = m_pModelCom->Get_NumMaterials();

	CCombat* pParentCombat = static_cast<CCombat*>(m_EffectDesc.m_pParent->Get_Component(TEXT("Com_Combat")));
	_uint iPass = 0;
	if (pParentCombat->Get_CombatDesc().eActorSide == CCombat::ACTOR_SIDE_LEFT)
		iPass = 6;
	else
		iPass = 8;

	for (_uint i = 0; iNumMaterials > i; ++i)
	{
		m_pModelCom->Bind_OnShader(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture");
		m_pModelCom->Render(m_pShaderCom, iPass, i);
	}

	return S_OK;
}

HRESULT CEffect_IRukaSan_Roll::Render_OutLine()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix WorldMatrix;

	WorldMatrix = XMLoadFloat4x4(&m_EffectDesc.matBillboard);

	_float4x4 BillboardMatrix;

	XMStoreFloat4x4(&BillboardMatrix,
		XMMatrixRotationRollPitchYaw(XMConvertToRadians(90.f + (m_fTimeDelta * 170)), XMConvertToRadians(-80.f), XMConvertToRadians(0.f)) *
		XMMatrixTranslation(0.75f - (m_fTimeDelta * 0.7f), 1.3f - (m_fTimeDelta * 1.2f), 1.3f)
		* WorldMatrix);

	_float4x4 ResultWorldMatrix;

	XMStoreFloat4x4(&ResultWorldMatrix, XMMatrixTranspose(XMLoadFloat4x4(&BillboardMatrix)));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &ResultWorldMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	// HDR
	if (FAILED(m_pShaderCom->Set_RawValue("g_bHDR", &g_bHDR, sizeof(_bool))))
		return E_FAIL;

	// 이미시브 & 글로우
	if (FAILED(m_pShaderCom->Set_RawValue("g_bEmissive", &m_EffectDesc.bGlow, sizeof(_bool))))
		return E_FAIL;

	_uint iNumMaterials = m_pModelCom->Get_NumMaterials();

	CCombat* pParentCombat = static_cast<CCombat*>(m_EffectDesc.m_pParent->Get_Component(TEXT("Com_Combat")));
	_uint iPass = 0;
	if (pParentCombat->Get_CombatDesc().eActorSide == CCombat::ACTOR_SIDE_LEFT)
		iPass = 7;
	else
		iPass = 9;

	for (_uint i = 0; iNumMaterials > i; ++i)
	{
		m_pModelCom->Bind_OnShader(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture");
		m_pModelCom->Render(m_pShaderCom, iPass, i);
	}

	return S_OK;
}

HRESULT CEffect_IRukaSan_Roll::SetUp_Components()
{
	return S_OK;
}

HRESULT CEffect_IRukaSan_Roll::Create_Effect_Splash()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//NONE_ANIM_EFFECT_DESC temp;
	//ZeroMemory(&temp, sizeof(NONE_ANIM_EFFECT_DESC));
	//temp = EFFECT_SPLASHCYLINDER1;
	//XMStoreFloat4x4(&temp.EffectDesc.matBillboard, m_EffectDesc.m_pParent->Get_BillBoardMatrix());
	////temp.EffectDesc.m_pParent = static_cast<CActor*>(_pObject);
	//if (FAILED(pGameInstance->Add_GameObject(
	//	m_iCurLevel,
	//	g_pLayerTags[DYNAMIC_LAYER_EFFECT],
	//	TEXT("Prototype_GameObject_Effect_Splashcylinder1"),
	//	&temp)))
	//	__debugbreak();

	//ZeroMemory(&temp, sizeof(NONE_ANIM_EFFECT_DESC));
	//temp = EFFECT_WATERRING;
	//XMStoreFloat4x4(&temp.EffectDesc.matBillboard, m_EffectDesc.m_pParent->Get_BillBoardMatrix());
	////temp.EffectDesc.m_pParent = static_cast<CActor*>(_pObject);
	//if (FAILED(pGameInstance->Add_GameObject(
	//	m_iCurLevel,
	//	g_pLayerTags[DYNAMIC_LAYER_EFFECT],
	//	TEXT("Prototype_GameObject_Effect_WaterRing"),
	//	&temp)))
	//	__debugbreak();

	CCombat* pParentCombat = static_cast<CCombat*>(m_EffectDesc.m_pParent->Get_Component(TEXT("Com_Combat")));
	_uint ChannelType = 0;

	if (pParentCombat->Get_CombatDesc().eActorSide == CCombat::ACTOR_SIDE_LEFT)
		ChannelType = CSoundMgr::PLAYER_L;
	else
		ChannelType = CSoundMgr::PLAYER_R;

	//	물
	pGameInstance->PlaySound_W(
		L"SE_BTL_MAY_01.ogg", 
		0.005f * ACT_May_DEFAULT_VOLUME, 
		(CSoundMgr::CHNNELTYPE)ChannelType);

	NONE_ANIM_EFFECT_DESC temp;
	ZeroMemory(&temp, sizeof(NONE_ANIM_EFFECT_DESC));
	temp = EFFECT_SPLASHCYLINDER3;
	XMStoreFloat4x4(&temp.EffectDesc.matBillboard, m_EffectDesc.m_pParent->Get_BillBoardMatrix());
	//temp.EffectDesc.m_pParent = static_cast<CActor*>(_pObject);
	if (FAILED(pGameInstance->Add_GameObject(
		m_iCurLevel,
		g_pLayerTags[DYNAMIC_LAYER_EFFECT],
		TEXT("Prototype_GameObject_Effect_Splashcylinder3"),
		&temp)))
		__debugbreak();

	m_bCreateOnce = true;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CEffect_IRukaSan_Roll* CEffect_IRukaSan_Roll::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CEffect_IRukaSan_Roll* pInstance = new CEffect_IRukaSan_Roll(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CEffect_IRukaSan_Roll");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEffect_IRukaSan_Roll::Clone(void* pArg)
{
	CEffect_IRukaSan_Roll* pInstance = new CEffect_IRukaSan_Roll(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CEffect_IRukaSan_Roll");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_IRukaSan_Roll::Free()
{
	__super::Free();
}
