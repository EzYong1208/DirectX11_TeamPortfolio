#include "stdafx.h"
#include "Effect_None_Anim_Effect.h"
#include "GameInstance.h"
#include "Level_Loading.h"


CEffect_None_Anim_Effect::CEffect_None_Anim_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CEffect(pDevice, pDeviceContext)
{
}


CEffect_None_Anim_Effect::CEffect_None_Anim_Effect(const CEffect_None_Anim_Effect& rhs)
	: CEffect(rhs)
{
}


HRESULT CEffect_None_Anim_Effect::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}


HRESULT CEffect_None_Anim_Effect::NativeConstruct(void* pArg)
{
	//	1. �����ν��Ͻ� �غ�
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	_uint iCurLevel = pGameInstance->Get_CurrentLevelIndex();

	//	2. ����ó��
	if (iCurLevel == LEVEL_LOGO ||
		iCurLevel == LEVEL_MAIN_MENU)
	{
		MSGBOX("CState_Player_Turn Error : CurLevel is invailed");
		return E_FAIL;
	}

	//	3. �ε� ������ ���
	if (iCurLevel == LEVEL_LOADING)
	{
		iCurLevel = dynamic_cast<CLevel_Loading*>(pGameInstance->Get_LoadingLevel())->Get_NextLevel();
	}
	RELEASE_INSTANCE(CGameInstance);

	m_iCurLevel = iCurLevel;

	/////	�� ���� �޾ƿ��� ����
	//	1. EffectDesc �ʱ�ȭ, GameObject�ʱ�ȭ : Ʈ������ ������Ʈ, ������
	if (pArg)
	{
		NONE_ANIM_EFFECT_DESC temp;
		memcpy(&temp, pArg, sizeof(NONE_ANIM_EFFECT_DESC));

		if (FAILED(__super::NativeConstruct(&temp.EffectDesc)))
			return E_FAIL;

		//	����Ʈ �� ������Ʈ �߰�
		/* For.Com_Model */
		if (FAILED(__super::Add_Component(
			m_iCurLevel,
			temp.SubEffectDesc.pModel_Tag,
			TEXT("Com_Model"),
			(CComponent**)&m_pModelCom)))
			return E_FAIL;

	}
	else
	{
		if (FAILED(__super::NativeConstruct(pArg)))
			return E_FAIL;
	}

	//	2. CStatic_Object �ʱ�ȭ : ��ִ� ���̴�
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	return S_OK;
}


_int CEffect_None_Anim_Effect::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	if (m_EffectDesc.bTrriger == true)
		m_EffectDesc.fLifetime -= (_float)TimeDelta;

	if (m_EffectDesc.fLifetime <= 0.f)
		m_EffectDesc.IsDead = true;

	if (m_EffectDesc.IsDead)
		return ISDEAD;
	else
		return 0;
}


_int CEffect_None_Anim_Effect::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	return 0;
}


HRESULT CEffect_None_Anim_Effect::Render()
{

	return S_OK;
}


_bool CEffect_None_Anim_Effect::Check_Ground(_fvector _vPos)
{
	return true;
}


void CEffect_None_Anim_Effect::Trigger_Activate(const _double& _fTimeDelta)
{
}


HRESULT CEffect_None_Anim_Effect::SetUp_Components()
{
	//	1. ���̴� �߰�
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(
		LEVEL_STATIC,
		g_pShaderTag[Shader_NonAnimModel],
		TEXT("Com_Shader"),
		(CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}


HRESULT CEffect_None_Anim_Effect::SetUp_ConstantTable_BillBoarding()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix WorldMatrix;

	//	�θ� �������, �θ��� ������ ����� ���ε��Ѵ�
	if (m_EffectDesc.m_pParent != nullptr)
	{
		WorldMatrix = m_EffectDesc.m_pParent->Get_BillBoardMatrix();
	}
	else
	{
		WorldMatrix = XMLoadFloat4x4(&m_EffectDesc.matBillboard);
	}

	_float4x4 BillboardMatrix;
	XMStoreFloat4x4(&BillboardMatrix, WorldMatrix);

	_float4x4 ResultWorldMatrix;
	XMStoreFloat4x4(&ResultWorldMatrix, XMMatrixTranspose(XMLoadFloat4x4(&BillboardMatrix)));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &ResultWorldMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vCamPosition", &pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	//	HDR
	if (FAILED(m_pShaderCom->Set_RawValue("g_bHDR", &g_bHDR, sizeof(_bool))))
		return E_FAIL;

	//	�̹̽ú� & �۷ο�
	if (FAILED(m_pShaderCom->Set_RawValue("g_bEmissive", &m_EffectDesc.bGlow, sizeof(_bool))))
		return E_FAIL;

	return S_OK;
}


HRESULT CEffect_None_Anim_Effect::SetUp_ConstantTable()
{
	return S_OK;
}


CEffect_None_Anim_Effect* CEffect_None_Anim_Effect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CEffect_None_Anim_Effect* pInstance = new CEffect_None_Anim_Effect(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CEffect_None_Anim_Effect");
		Safe_Release(pInstance);
	}
	return pInstance;
}


CGameObject* CEffect_None_Anim_Effect::Clone(void* pArg)
{
	CEffect_None_Anim_Effect* pInstance = new CEffect_None_Anim_Effect(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CEffect_None_Anim_Effect");
		Safe_Release(pInstance);
	}
	return pInstance;
}


void CEffect_None_Anim_Effect::Free()
{
	__super::Free();
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pTextureCom);
}