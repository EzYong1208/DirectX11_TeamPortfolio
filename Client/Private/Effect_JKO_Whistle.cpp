#include "stdafx.h"
#include "Effect_JKO_Whistle.h"
#include "GameInstance.h"

CEffect_JKO_Whistle::CEffect_JKO_Whistle(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CEffect_Anim_Effect(pDevice, pDeviceContext)
{
}

CEffect_JKO_Whistle::CEffect_JKO_Whistle(const CEffect_JKO_Whistle& rhs)
	: CEffect_Anim_Effect(rhs)
{
}

HRESULT CEffect_JKO_Whistle::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_JKO_Whistle::NativeConstruct(void* pArg)
{
	// Æ®·£½ºÆû, ¸ðµ¨, ¼ÎÀÌ´õ, ·£´õ·¯ Ãß°¡ 
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	ZeroMemory(&m_JKOWhistleDesc, sizeof(JKOWHISTLEDESC));
	memcpy(&m_JKOWhistleDesc, pArg, sizeof(JKOWHISTLEDESC));

	XMStoreFloat3(&m_EffectDesc.vScaleOrigin, m_pTransform->Get_Scale());

	m_pModelCom->SetUp_AnimIndex(m_JKOWhistleDesc.eIndex);
	m_pModelCom->Set_Anim_ExtarSpeed(m_JKOWhistleDesc.eIndex, 3.f);

	return S_OK;
}

_int CEffect_JKO_Whistle::Tick(_double TimeDelta)
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

_int CEffect_JKO_Whistle::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_SECOND, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_OUTLINE, this);

	return 0;
}

HRESULT CEffect_JKO_Whistle::Render()
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

HRESULT CEffect_JKO_Whistle::Render_OutLine()
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

_uint CEffect_JKO_Whistle::FrameLock()
{
	_double		fCurAcc = 0.f;
	fCurAcc = m_pModelCom->Get_PlayAcc();

	_uint		iResult = 0;

	switch (m_JKOWhistleDesc.eIndex)
	{
	case INDEX_432:
		if (fCurAcc >= 30.f)
			iResult = ISDEAD;
		break;

	case INDEX_433:
		if (fCurAcc >= 30.f)
			iResult = ISDEAD;
		break;
		
	case INDEX_502:
		if (m_iFrameLockCount == 0 && fCurAcc >= 5.f)
		{
			++m_iFrameLockCount;
			m_pModelCom->Activate_FrameLock(0.f, 15.f);
			m_pModelCom->Set_Anim_TimeAcc(m_JKOWhistleDesc.eIndex, 5.f - 4);
		}
		if (m_iFrameLockCount == 1 && fCurAcc >= 5.f)
		{
			++m_iFrameLockCount;
			m_pModelCom->Activate_FrameLock(5.f, 110.f);
			m_pModelCom->Set_Anim_TimeAcc(m_JKOWhistleDesc.eIndex, 10.f - 4);
		}
		if (m_iFrameLockCount == 2 && fCurAcc >= 10.f)
		{
			++m_iFrameLockCount;
			m_pModelCom->Activate_FrameLock(10.f, 115.f);
			m_pModelCom->Set_Anim_TimeAcc(m_JKOWhistleDesc.eIndex, 15.f - 4);
		}
		if (m_iFrameLockCount == 3 && fCurAcc >= 15.f)
		{
			++m_iFrameLockCount;
			m_pModelCom->Activate_FrameLock(15.f, 120.f);
			m_pModelCom->Set_Anim_TimeAcc(m_JKOWhistleDesc.eIndex, 20.f - 6);
		}
		if (m_iFrameLockCount == 4 && fCurAcc >= 20.f)
		{
			++m_iFrameLockCount;
			m_pModelCom->Activate_FrameLock(20.f, 125.f);
			m_pModelCom->Set_Anim_TimeAcc(m_JKOWhistleDesc.eIndex, 25.f - 6);
		}
		if (m_iFrameLockCount == 5 && fCurAcc >= 25.f)
		{
			++m_iFrameLockCount;
			m_pModelCom->Activate_FrameLock(25.f, 130.f);
			m_pModelCom->Set_Anim_TimeAcc(m_JKOWhistleDesc.eIndex, 30.f - 8);
		}
		if (m_iFrameLockCount == 6 && fCurAcc >= 30.f)
		{
			++m_iFrameLockCount;
			m_pModelCom->Activate_FrameLock(30.f, 135.f);
			m_pModelCom->Set_Anim_TimeAcc(m_JKOWhistleDesc.eIndex, 35.f - 6);
			iResult = ISDEAD;
		}
		break;
	}

	return iResult;
}

CEffect_JKO_Whistle* CEffect_JKO_Whistle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CEffect_JKO_Whistle* pInstance = new CEffect_JKO_Whistle(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CEffect_JKO_Whistle");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEffect_JKO_Whistle::Clone(void* pArg)
{
	CEffect_JKO_Whistle* pInstance = new CEffect_JKO_Whistle(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CEffect_JKO_Whistle");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_JKO_Whistle::Free()
{
	__super::Free();
}
