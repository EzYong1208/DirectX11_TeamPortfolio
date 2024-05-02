#include "stdafx.h"
#include "Effect_IRukaSan_Return.h"
#include "GameInstance.h"
#include "Effect_None_Anim_Effect.h"

CEffect_IRukaSan_Return::CEffect_IRukaSan_Return(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CEffect_Anim_Effect(pDevice, pDeviceContext)
{
}

CEffect_IRukaSan_Return::CEffect_IRukaSan_Return(const CEffect_IRukaSan_Return& rhs)
	: CEffect_Anim_Effect(rhs)
{
}

HRESULT CEffect_IRukaSan_Return::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_IRukaSan_Return::NativeConstruct(void* pArg)
{
	// Æ®·£½ºÆû, ¸ðµ¨, ¼ÎÀÌ´õ, ·£´õ·¯ Ãß°¡
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	XMStoreFloat3(&m_EffectDesc.vScaleOrigin, m_pTransform->Get_Scale());

	return S_OK;
}

_int CEffect_IRukaSan_Return::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	int temp = 0;

	m_fTime += (_float)TimeDelta * m_EffectDesc.fSpeed;
	m_fTimeDelta += TimeDelta * 2.f;

	switch (m_iSequence)
	{
	case 0:

		if (m_EffectDesc.fLifetime >= 0.f)
		{
			m_EffectDesc.fLifetime -= (_float)TimeDelta;
			//m_BoundTimeDelta += TimeDelta;
			//m_pTransform->Bound(TimeDelta, m_BoundTimeDelta, XMVectorSet(0.f, 0.f, 0.f, 0.f), 0.f, 2.f);
			//m_pTransform->Reset_Momentum();
			//_vector vWorldPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
			//XMStoreFloat4x4(&m_EffectDesc.matBillboard, m_EffectDesc.m_pParent->Get_BillBoardMatrix());
			//XMStoreFloat3((_float3*)&m_EffectDesc.matBillboard._41, vWorldPos);
			//m_pTransform->KnockBack(TimeDelta, m_BoundTimeDelta, m_pTransform->Get_State(CTransform::STATE_RIGHT)*0.1f, 10.f, 8.f);
			
		}
		else
			temp = ISDEAD;

		break;

	default:
		break;
	}

	if (m_bHit)
		temp = ISDEAD;

	m_pTransform->Reset_Momentum();

	if (!m_bCreateOnce)
		DEBUG_BREAK(Create_Effect_Splash());

	return temp;
}

_int CEffect_IRukaSan_Return::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_SECOND, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_OUTLINE, this);

	return 0;
}

HRESULT CEffect_IRukaSan_Return::Render()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix WorldMatrix;

	WorldMatrix = XMLoadFloat4x4(&m_EffectDesc.matBillboard);

	_float4x4 BillboardMatrix;

	//XMMatrixRotationRollPitchYaw(XMConvertToRadians(75.f), XMConvertToRadians(-90.f), XMConvertToRadians(-80.f)) *

	XMStoreFloat4x4(&BillboardMatrix,
		XMMatrixRotationRollPitchYaw(XMConvertToRadians(75.f - (m_fTimeDelta * 90)), XMConvertToRadians(-90.f), XMConvertToRadians(-80.f)) *
		XMMatrixTranslation(-0.3f, 0.f - (m_fTimeDelta * 2.f), 0.5f)
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

HRESULT CEffect_IRukaSan_Return::Render_OutLine()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix WorldMatrix;

	WorldMatrix = XMLoadFloat4x4(&m_EffectDesc.matBillboard);

	_float4x4 BillboardMatrix;

	XMStoreFloat4x4(&BillboardMatrix,
		XMMatrixRotationRollPitchYaw(XMConvertToRadians(75.f - (m_fTimeDelta * 90)), XMConvertToRadians(-90.f), XMConvertToRadians(-80.f)) *
		XMMatrixTranslation(-0.3f, 0.f - (m_fTimeDelta * 2.f), 0.5f)
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

HRESULT CEffect_IRukaSan_Return::SetUp_Components()
{
	return S_OK;
}

HRESULT CEffect_IRukaSan_Return::Create_Effect_Splash()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

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

CEffect_IRukaSan_Return* CEffect_IRukaSan_Return::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CEffect_IRukaSan_Return* pInstance = new CEffect_IRukaSan_Return(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CEffect_IRukaSan_Return");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEffect_IRukaSan_Return::Clone(void* pArg)
{
	CEffect_IRukaSan_Return* pInstance = new CEffect_IRukaSan_Return(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CEffect_IRukaSan_Return");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_IRukaSan_Return::Free()
{
	__super::Free();
}
