#include "stdafx.h"
#include "Effect_Particle_Effect.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "VIBuffer_Point_Instance_Effect.h"


CEffect_Particle_Effect::CEffect_Particle_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CEffect(pDevice, pDeviceContext)
{
}


CEffect_Particle_Effect::CEffect_Particle_Effect(const CEffect_Particle_Effect& rhs)
	: CEffect(rhs)
{
}


HRESULT CEffect_Particle_Effect::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}


HRESULT CEffect_Particle_Effect::NativeConstruct(void* pArg)
{
	//1. 게임인스턴스 준비
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
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
	RELEASE_INSTANCE(CGameInstance);

	m_iCurLevel = iCurLevel;

	/////모델 정보 받아오기 부터
	//1. EffectDesc 초기화, GameObject초기화 : 트랜스폼 컴포넌트, 랜더러
	if (pArg)
	{
		PARTICLE_EFFECT_DESC temp;
		memcpy(&temp, pArg, sizeof(PARTICLE_EFFECT_DESC));

		if (FAILED(__super::NativeConstruct(&temp.EffectDesc)))
			return E_FAIL;

		//이펙트 모델 컴포넌트 추가
		/* For.Com_VIBuffer */
		CVIBuffer_Point_Instance_Effect::PARTICLEDESC ParticleDesc;
		ZeroMemory(&ParticleDesc, sizeof(CVIBuffer_Point_Instance_Effect::PARTICLEDESC));
		ParticleDesc.iBufferIndex = m_EffectDesc.iEffectIndex;
		ParticleDesc.fMinSpeed = 2.f;
		ParticleDesc.fMaxSpeed = 10.f;
		if (FAILED(__super::Add_Component(
			m_iCurLevel,
			TEXT("Prototype_Component_VIBuffer_Point_Instance_Effect"),
			TEXT("Com_VIBuffer"),
			(CComponent**)&m_pVIBufferCom,
			&ParticleDesc)))
			return E_FAIL;

		/* For.Com_Texture */
		if (FAILED(__super::Add_Component(
			m_iCurLevel,
			temp.SubEffectDesc.pTexture_Tag,
			TEXT("Com_Texture"),
			(CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(__super::NativeConstruct(pArg)))
			return E_FAIL;
	}

	//2. CStatic_Object 초기화 : 논애니 셰이더
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	return S_OK;
}


_int CEffect_Particle_Effect::Tick(_double TimeDelta)
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


_int CEffect_Particle_Effect::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	return 0;
}


HRESULT CEffect_Particle_Effect::Render()
{

	return S_OK;
}


_bool CEffect_Particle_Effect::Check_Ground(_fvector _vPos)
{
	return true;
}


void CEffect_Particle_Effect::Trigger_Activate(const _double& _fTimeDelta)
{
}


HRESULT CEffect_Particle_Effect::SetUp_Components()
{
	//1. 셰이더 추가
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(
		LEVEL_STATIC,
		g_pShaderTag[Shader_PointInstance],
		TEXT("Com_Shader"),
		(CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}


HRESULT CEffect_Particle_Effect::SetUp_ConstantTable_BillBoarding()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix WorldMatrix;

	//부모가 있을경우, 부모의 빌보드 행렬을 바인드한다
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

	// HDR
	if (FAILED(m_pShaderCom->Set_RawValue("g_bHDR", &g_bHDR, sizeof(_bool))))
		return E_FAIL;

	// 이미시브 & 글로우
	if (FAILED(m_pShaderCom->Set_RawValue("g_bEmissive", &m_EffectDesc.bGlow, sizeof(_bool))))
		return E_FAIL;

	return S_OK;
}


HRESULT CEffect_Particle_Effect::SetUp_ConstantTable()
{
	return S_OK;
}


CEffect_Particle_Effect* CEffect_Particle_Effect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CEffect_Particle_Effect* pInstance = new CEffect_Particle_Effect(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CEffect_Particle_Effect");
		Safe_Release(pInstance);
	}
	return pInstance;
}


CGameObject* CEffect_Particle_Effect::Clone(void* pArg)
{
	CEffect_Particle_Effect* pInstance = new CEffect_Particle_Effect(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CEffect_Particle_Effect");
		Safe_Release(pInstance);
	}
	return pInstance;
}


void CEffect_Particle_Effect::Free()
{
	__super::Free();
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}