#include "stdafx.h"
#include "Effect_IRukaSan_Anim.h"
#include "GameInstance.h"

CEffect_IRukaSan_Anim::CEffect_IRukaSan_Anim(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CEffect_Anim_Effect(pDevice, pDeviceContext)
{
}

CEffect_IRukaSan_Anim::CEffect_IRukaSan_Anim(const CEffect_IRukaSan_Anim& rhs)
	: CEffect_Anim_Effect(rhs)
{
}

HRESULT CEffect_IRukaSan_Anim::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_IRukaSan_Anim::NativeConstruct(void* pArg)
{
	// 트랜스폼, 모델, 셰이더, 랜더러 추가
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	XMStoreFloat3(&m_EffectDesc.vScaleOrigin, m_pTransform->Get_Scale());

	m_pModelCom->SetUp_AnimIndex(0);

	return S_OK;
}

_int CEffect_IRukaSan_Anim::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	m_pModelCom->Update_CombinedTransformationMatrices(TimeDelta);
	m_pModelCom->Set_Anim_ExtarSpeed(0, 3.f);

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

	_double	CurrentAcc = m_pModelCom->Get_PlayAcc();

	if (28 <= CurrentAcc)
		temp = ISDEAD;

	if (m_bHit)
		temp = ISDEAD;

	m_pTransform->Reset_Momentum();

	return temp;
}

_int CEffect_IRukaSan_Anim::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_SECOND, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_OUTLINE, this);

	return 0;
}

HRESULT CEffect_IRukaSan_Anim::Render()
{
	if (FAILED(SetUp_ConstantTable_BillBoarding()))
		return E_FAIL;

	_uint iPass = 0;

	if (!m_EffectDesc.bTrriger)
	{
		CCombat* pParentCombat = static_cast<CCombat*>(m_EffectDesc.m_pParent->Get_Component(TEXT("Com_Combat")));
		
		if (pParentCombat->Get_CombatDesc().eActorSide == CCombat::ACTOR_SIDE_LEFT)
			iPass = 0;
		else
			iPass = 3;
	}
	//	포템킨용
	else
	{
		CCombat* pParentCombat = static_cast<CCombat*>(m_EffectDesc.m_pParent->Get_Component(TEXT("Com_Combat")));

		if (pParentCombat->Get_CombatDesc().eActorSide == CCombat::ACTOR_SIDE_LEFT)
			iPass = 3;
		else
			iPass = 0;
	}

	_uint iNumMaterials = m_pModelCom->Get_NumMaterials();
	for (_uint i = 0; iNumMaterials > i; ++i)
	{
		m_pModelCom->Bind_OnShader(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture");
		m_pModelCom->Bind_OnShader(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture");
		m_pModelCom->Render(m_pShaderCom, iPass, i, "g_BoneMatrices");
	}

	return S_OK;
}

HRESULT CEffect_IRukaSan_Anim::Render_OutLine()
{
	if (FAILED(SetUp_ConstantTable_BillBoarding()))
		return E_FAIL;

	_uint iPass = 0;

	if (!m_EffectDesc.bTrriger)
	{
		CCombat* pParentCombat = static_cast<CCombat*>(m_EffectDesc.m_pParent->Get_Component(TEXT("Com_Combat")));
		
		if (pParentCombat->Get_CombatDesc().eActorSide == CCombat::ACTOR_SIDE_LEFT)
			iPass = 2;
		else
			iPass = 4;
	}
	//	포템킨용
	else
	{
		CCombat* pParentCombat = static_cast<CCombat*>(m_EffectDesc.m_pParent->Get_Component(TEXT("Com_Combat")));

		if (pParentCombat->Get_CombatDesc().eActorSide == CCombat::ACTOR_SIDE_LEFT)
			iPass = 4;
		else
			iPass = 2;
	}

	_uint iNumMaterials = m_pModelCom->Get_NumMaterials();

	for (_uint i = 0; iNumMaterials > i; ++i)
	{
		m_pModelCom->Bind_OnShader(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture");
		m_pModelCom->Bind_OnShader(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture");
		m_pModelCom->Render(m_pShaderCom, iPass, i, "g_BoneMatrices");
	}

	return S_OK;
}

HRESULT CEffect_IRukaSan_Anim::SetUp_Components()
{
	return S_OK;
}

CEffect_IRukaSan_Anim* CEffect_IRukaSan_Anim::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CEffect_IRukaSan_Anim* pInstance = new CEffect_IRukaSan_Anim(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CEffect_IRukaSan_Anim");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEffect_IRukaSan_Anim::Clone(void* pArg)
{
	CEffect_IRukaSan_Anim* pInstance = new CEffect_IRukaSan_Anim(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CEffect_IRukaSan_Anim");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_IRukaSan_Anim::Free()
{
	__super::Free();
}
