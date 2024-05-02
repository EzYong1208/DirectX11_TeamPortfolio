#include "stdafx.h"
#include "Effect_MAY_MayDynamic.h"
#include "GameInstance.h"

CEffect_MAY_MayDynamic::CEffect_MAY_MayDynamic(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CEffect_Anim_Effect(pDevice, pDeviceContext)
{
}

CEffect_MAY_MayDynamic::CEffect_MAY_MayDynamic(const CEffect_MAY_MayDynamic& rhs)
	: CEffect_Anim_Effect(rhs)
{
}

HRESULT CEffect_MAY_MayDynamic::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_MAY_MayDynamic::NativeConstruct(void* pArg)
{
	// 트랜스폼, 모델, 셰이더, 랜더러 추가
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	XMStoreFloat3(&m_EffectDesc.vScaleOrigin, m_pTransform->Get_Scale());

	m_pModelCom->SetUp_AnimIndex(0);
	CCombat* pParentCombat = static_cast<CCombat*>(m_EffectDesc.m_pParent->Get_Component(TEXT("Com_Combat")));

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CTransform* pPlayerTransform = (CTransform*)pGameInstance->Get_Component(m_iCurLevel, g_pLayerTags[DYNAMIC_LAYER_PLAYER_L], CGameObject::m_pTransformTag);
	RELEASE_INSTANCE(CGameInstance);

	_double PlayerRatio = pPlayerTransform->GetTransfromDesc().Ratio;
	CTransform* pParentTransform = (CTransform*)m_EffectDesc.m_pParent->Get_Component(CGameObject::m_pTransformTag);
	_double ParentRatio = pParentTransform->GetTransfromDesc().Ratio;

	if (PlayerRatio < ParentRatio)
		m_ActorSide = CCombat::ACTOR_SIDE_RIGHT;
	else
		m_ActorSide = CCombat::ACTOR_SIDE_LEFT;

	return S_OK;
}

_int CEffect_MAY_MayDynamic::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	m_pModelCom->Update_CombinedTransformationMatrices_Limit(TimeDelta);
	m_pModelCom->Set_Anim_ExtarSpeed(0, 4.f);

	m_fCurAcc = m_pModelCom->Get_PlayAcc();

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

	temp = FrameLock();

	m_pTransform->Reset_Momentum();

	return temp;
}

_int CEffect_MAY_MayDynamic::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_SECOND, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_OUTLINE, this);

	return 0;
}

HRESULT CEffect_MAY_MayDynamic::Render()
{
	DEBUG_BREAK(SetUp_ConstantTable_BillBoarding());

	_uint iNumMaterials = m_pModelCom->Get_NumMaterials();

	//CCombat* pParentCombat = static_cast<CCombat*>(m_EffectDesc.m_pParent->Get_Component(TEXT("Com_Combat")));
	_uint iPass = 0;
	if (m_ActorSide == CCombat::ACTOR_SIDE_LEFT)
		iPass = 3;
	else
		iPass = 0;

	for (_uint i = 0; iNumMaterials > i; ++i)
	{
		m_pModelCom->Bind_OnShader(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture");
		m_pModelCom->Bind_OnShader(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture");
		m_pModelCom->Render(m_pShaderCom, iPass, i, "g_BoneMatrices");
	}

	return S_OK;
}

HRESULT CEffect_MAY_MayDynamic::Render_OutLine()
{
	DEBUG_BREAK(SetUp_ConstantTable_BillBoarding());

	_uint iNumMaterials = m_pModelCom->Get_NumMaterials();

	_uint iPass = 0;
	if (m_ActorSide == CCombat::ACTOR_SIDE_LEFT)
		iPass = 2;
	else
		iPass = 4;

	for (_uint i = 0; iNumMaterials > i; ++i)
	{
		m_pModelCom->Bind_OnShader(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture");
		m_pModelCom->Bind_OnShader(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture");
		m_pModelCom->Render(m_pShaderCom, iPass, i, "g_BoneMatrices");
	}

	return S_OK;
}

HRESULT CEffect_MAY_MayDynamic::SetUp_ConstantTable_BillBoarding()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix ViewMatrix, ViewMatrixInverse;

	ViewMatrix = pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_VIEW);
	ViewMatrixInverse = XMMatrixInverse(nullptr, ViewMatrix);

	_float4x4 matViewInverse;

	XMStoreFloat4x4(&matViewInverse, ViewMatrixInverse);
	_matrix WorldMatrix;

	WorldMatrix = m_EffectDesc.m_pParent->Get_BillBoardMatrix();

	_float4x4 BillboardMatrix;
	XMStoreFloat4x4(&BillboardMatrix, XMMatrixScaling(2.f, 2.f, 2.f) * WorldMatrix);

	if(m_ActorSide == CCombat::ACTOR_SIDE_LEFT)
		XMStoreFloat3((_float3*)BillboardMatrix.m[0], XMLoadFloat3(&(*(_float3*)matViewInverse.m[0])) * 2.f);
	else
		XMStoreFloat3((_float3*)BillboardMatrix.m[0], -XMLoadFloat3(&(*(_float3*)matViewInverse.m[0])) * 2.f);

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

	return S_OK;
}

_uint CEffect_MAY_MayDynamic::FrameLock()
{
	_uint	iResult = 0;

	if (m_fCurAcc >= 140.f)
		iResult = ISDEAD;

	return iResult;
}

CEffect_MAY_MayDynamic* CEffect_MAY_MayDynamic::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CEffect_MAY_MayDynamic* pInstance = new CEffect_MAY_MayDynamic(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CEffect_IRukaSan_Anim");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEffect_MAY_MayDynamic::Clone(void* pArg)
{
	CEffect_MAY_MayDynamic* pInstance = new CEffect_MAY_MayDynamic(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CEffect_IRukaSan_Anim");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_MAY_MayDynamic::Free()
{
	__super::Free();
}
