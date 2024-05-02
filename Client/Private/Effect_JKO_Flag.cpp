#include "stdafx.h"
#include "Effect_JKO_Flag.h"
#include "GameInstance.h"

CEffect_JKO_Flag::CEffect_JKO_Flag(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CEffect_Anim_Effect(pDevice, pDeviceContext)
{
}

CEffect_JKO_Flag::CEffect_JKO_Flag(const CEffect_JKO_Flag& rhs)
	: CEffect_Anim_Effect(rhs)
{
}

HRESULT CEffect_JKO_Flag::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_JKO_Flag::NativeConstruct(void* pArg)
{
	// Æ®·£½ºÆû, ¸ðµ¨, ¼ÎÀÌ´õ, ·£´õ·¯ Ãß°¡ 
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	XMStoreFloat3(&m_EffectDesc.vScaleOrigin, m_pTransform->Get_Scale());

	m_pModelCom->SetUp_AnimIndex(INDEX_00);
	m_pModelCom->Set_Anim_ExtarSpeed(INDEX_00, 3.f);

	return S_OK;
}

_int CEffect_JKO_Flag::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	m_pModelCom->Update_CombinedTransformationMatrices(TimeDelta);

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

_int CEffect_JKO_Flag::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_SECOND, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_OUTLINE, this);

	return 0;
}

HRESULT CEffect_JKO_Flag::Render()
{
	if (FAILED(SetUp_ConstantTable_BillBoarding()))
		return E_FAIL;

	_uint iNumMaterials = m_pModelCom->Get_NumMaterials();

	CCombat* pParentCombat = static_cast<CCombat*>(m_EffectDesc.m_pParent->Get_Component(TEXT("Com_Combat")));
	_uint iPass = 0;
	if (pParentCombat->Get_CombatDesc().eActorSide == CCombat::ACTOR_SIDE_LEFT)
		iPass = 0;
	else
		iPass = 3;

	for (_uint i = 0; iNumMaterials > i; ++i)
	{
		m_pModelCom->Bind_OnShader(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture");
		m_pModelCom->Bind_OnShader(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture");
		m_pModelCom->Render(m_pShaderCom, iPass, i, "g_BoneMatrices");
	}

	return S_OK;
}

HRESULT CEffect_JKO_Flag::Render_OutLine()
{
	if (FAILED(SetUp_ConstantTable_BillBoarding()))
		return E_FAIL;

	_uint iNumMaterials = m_pModelCom->Get_NumMaterials();

	CCombat* pParentCombat = static_cast<CCombat*>(m_EffectDesc.m_pParent->Get_Component(TEXT("Com_Combat")));
	_uint iPass = 0;
	if (pParentCombat->Get_CombatDesc().eActorSide == CCombat::ACTOR_SIDE_LEFT)
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

_uint CEffect_JKO_Flag::FrameLock()
{
	_double		fCurAcc = 0.f;
	fCurAcc = m_pModelCom->Get_PlayAcc();

	CTransform* pParentTransform = static_cast<CTransform*>(m_EffectDesc.m_pParent->Get_Component(TEXT("Com_Transform")));
	_uint		iResult = 0;

	if (m_iFrameLockCount == 0 && fCurAcc >= 5.f)
	{
		++m_iFrameLockCount;
		m_pModelCom->Activate_FrameLock(0.f, 15.f);
		m_pModelCom->Set_Anim_TimeAcc(INDEX_00, 5.f - 4);
	}
	if (m_iFrameLockCount == 1 && fCurAcc >= 5.f)
	{
		++m_iFrameLockCount;
		m_pModelCom->Activate_FrameLock(5.f, 110.f);
		m_pModelCom->Set_Anim_TimeAcc(INDEX_00, 10.f - 4);
	}
	if (m_iFrameLockCount == 2 && fCurAcc >= 10.f)
	{
		++m_iFrameLockCount;
		m_pModelCom->Activate_FrameLock(10.f, 115.f);
		m_pModelCom->Set_Anim_TimeAcc(INDEX_00, 15.f - 4);
	}
	if (m_iFrameLockCount == 3 && fCurAcc >= 15.f)
	{
		++m_iFrameLockCount;
		m_pModelCom->Activate_FrameLock(15.f, 120.f);
		m_pModelCom->Set_Anim_TimeAcc(INDEX_00, 20.f - 6);
	}
	if (m_iFrameLockCount == 4 && fCurAcc >= 20.f)
	{
		++m_iFrameLockCount;
		m_pModelCom->Activate_FrameLock(20.f, 125.f);
		m_pModelCom->Set_Anim_TimeAcc(INDEX_00, 25.f - 6);
	}
	if (m_iFrameLockCount == 5 && fCurAcc >= 25.f)
	{
		++m_iFrameLockCount;
		m_pModelCom->Activate_FrameLock(25.f, 130.f);
		m_pModelCom->Set_Anim_TimeAcc(INDEX_00, 30.f - 8);
	}
	if (m_iFrameLockCount == 6 && fCurAcc >= 30.f)
	{
		++m_iFrameLockCount;
		m_pModelCom->Activate_FrameLock(30.f, 135.f);
		m_pModelCom->Set_Anim_TimeAcc(INDEX_00, 35.f - 6);
		iResult = ISDEAD;
	}
	//if (m_iFrameLockCount == 7 && fCurAcc >= 35.f)
	//{
	//	++m_iFrameLockCount;
	//	m_pModelCom->Activate_FrameLock(35.f, 140.f);
	//	m_pModelCom->Set_Anim_TimeAcc(INDEX_00, 40.f - 8);
	//	iResult = ISDEAD;
	//}
	//if (m_iFrameLockCount == 8 && fCurAcc >= 40.f)
	//{
	//	++m_iFrameLockCount;
	//	m_pModelCom->Activate_FrameLock(40.f, 145.f);
	//	m_pModelCom->Set_Anim_TimeAcc(INDEX_00, 45.f - 2);
	//	iResult = ISDEAD;
	//}
	//if (m_iFrameLockCount == 9 && fCurAcc >= 45.f)
	//{
	//	++m_iFrameLockCount;
	//	m_pModelCom->Activate_FrameLock(45.f, 150.f);
	//	iResult = ISDEAD;
	//}

	return iResult;
}

CEffect_JKO_Flag* CEffect_JKO_Flag::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CEffect_JKO_Flag* pInstance = new CEffect_JKO_Flag(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CEffect_JKO_Flag");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEffect_JKO_Flag::Clone(void* pArg)
{
	CEffect_JKO_Flag* pInstance = new CEffect_JKO_Flag(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CEffect_JKO_Flag");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_JKO_Flag::Free()
{
	__super::Free();
}
