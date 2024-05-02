#include "stdafx.h"
#include "Effect_IRukaSan_Hit.h"
#include "GameInstance.h"

CEffect_IRukaSan_Hit::CEffect_IRukaSan_Hit(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CEffect_Anim_Effect(pDevice, pDeviceContext)
{
}

CEffect_IRukaSan_Hit::CEffect_IRukaSan_Hit(const CEffect_IRukaSan_Hit& rhs)
	: CEffect_Anim_Effect(rhs)
{
}

HRESULT CEffect_IRukaSan_Hit::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_IRukaSan_Hit::NativeConstruct(void* pArg)
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

_int CEffect_IRukaSan_Hit::Tick(_double TimeDelta)
{
	int temp = 0;

	m_fTime += (_float)TimeDelta * m_EffectDesc.fSpeed;

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

	if (ISDEAD == temp)
		DEBUG_BREAK(Create_Effect_IRukaSan_Roll());

	return temp;
}

_int CEffect_IRukaSan_Hit::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_SECOND, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_OUTLINE, this);

	return 0;
}

HRESULT CEffect_IRukaSan_Hit::Render()
{                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
	if (nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix WorldMatrix;

	WorldMatrix = XMLoadFloat4x4(&m_EffectDesc.matBillboard);

	_float4x4 BillboardMatrix;

	XMStoreFloat4x4(&BillboardMatrix, 
		XMMatrixRotationRollPitchYaw(XMConvertToRadians(90.f),XMConvertToRadians(-80.f), XMConvertToRadians(0.f)) * 
		XMMatrixTranslation(-0.05f, 0.15f, 0.2f)
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

HRESULT CEffect_IRukaSan_Hit::Render_OutLine()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix WorldMatrix;

	WorldMatrix = XMLoadFloat4x4(&m_EffectDesc.matBillboard);

	_float4x4 BillboardMatrix;

	XMStoreFloat4x4(&BillboardMatrix,
		XMMatrixRotationRollPitchYaw(XMConvertToRadians(90.f), XMConvertToRadians(-80.f), XMConvertToRadians(0.f)) *
		XMMatrixTranslation(-0.05f, 0.15f, 0.2f)
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

HRESULT CEffect_IRukaSan_Hit::SetUp_Components()
{
	return S_OK;
}

HRESULT CEffect_IRukaSan_Hit::Create_Effect_IRukaSan_Roll()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	ANIMEFFECTDESC		IRukaSanRollDesc;
	ZeroMemory(&IRukaSanRollDesc, sizeof(ANIMEFFECTDESC));
	IRukaSanRollDesc = EFFECT_IRUKASAN_ROLL;
	XMStoreFloat4x4(&IRukaSanRollDesc.EffectDesc.matBillboard, m_EffectDesc.m_pParent->Get_BillBoardMatrix());
	IRukaSanRollDesc.EffectDesc.m_pParent = m_EffectDesc.m_pParent;
	if (FAILED(pGameInstance->Add_GameObject(
		m_iCurLevel,
		g_pLayerTags[DYNAMIC_LAYER_EFFECT],
		TEXT("Prototype_GameObject_Effect_IRukaSan_Roll"),
		&IRukaSanRollDesc)))
		__debugbreak();

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CEffect_IRukaSan_Hit* CEffect_IRukaSan_Hit::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CEffect_IRukaSan_Hit* pInstance = new CEffect_IRukaSan_Hit(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CEffect_IRukaSan_Hit");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEffect_IRukaSan_Hit::Clone(void* pArg)
{
	CEffect_IRukaSan_Hit* pInstance = new CEffect_IRukaSan_Hit(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CEffect_IRukaSan_Hit");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_IRukaSan_Hit::Free()
{
	__super::Free();
}
