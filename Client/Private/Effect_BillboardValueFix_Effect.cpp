#include "stdafx.h"
#include "Effect_BillboardValueFix_Effect.h"
#include "GameInstance.h"
#include "Level_Loading.h"

CEffect_BillboardValueFix_Effect::CEffect_BillboardValueFix_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CEffect(pDevice, pDeviceContext)
{
}

CEffect_BillboardValueFix_Effect::CEffect_BillboardValueFix_Effect(const CEffect_BillboardValueFix_Effect& rhs)
	: CEffect(rhs)
{
}

HRESULT CEffect_BillboardValueFix_Effect::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_BillboardValueFix_Effect::NativeConstruct(void* pArg)
{
	//	1. 게임인스턴스 준비
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	_uint iCurLevel = pGameInstance->Get_CurrentLevelIndex();

	//	2. 예외처리
	if (iCurLevel == LEVEL_LOGO ||
		iCurLevel == LEVEL_MAIN_MENU)
	{
		MSGBOX("CEffect_BillboardValueFix_Effect Error : CurLevel is invailed");
		return E_FAIL;
	}

	//	3. 로딩 레벨의 경우
	if (iCurLevel == LEVEL_LOADING)
	{
		iCurLevel = dynamic_cast<CLevel_Loading*>(pGameInstance->Get_LoadingLevel())->Get_NextLevel();
	}
	RELEASE_INSTANCE(CGameInstance);

	m_iCurLevel = iCurLevel;

	ZeroMemory(&m_ValueFixDesc, sizeof(VALUEFIXEFFECTDESC));
	memcpy(&m_ValueFixDesc, pArg, sizeof(VALUEFIXEFFECTDESC));

	if (FAILED(__super::NativeConstruct(&m_ValueFixDesc.EffectDesc)))
		return E_FAIL;

	//	For.Com_Model
	if (FAILED(__super::Add_Component(
		m_iCurLevel,
		m_ValueFixDesc.SubEffectDesc.pModel_Tag,
		TEXT("Com_Model"),
		(CComponent**)&m_pModelCom)))
		return E_FAIL;

	//	For.Com_Shader
	if (FAILED(__super::Add_Component(
		LEVEL_STATIC,
		g_pShaderTag[Shader_NonAnimModel],
		TEXT("Com_Shader"),
		(CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

_int CEffect_BillboardValueFix_Effect::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	if (m_EffectDesc.bTrriger == true)
		m_EffectDesc.fLifetime -= (_float)TimeDelta;

	if (m_EffectDesc.fLifetime <= 0.f)
		m_EffectDesc.IsDead = true;

	if (m_ValueFixDesc.bMove)
	{
		m_fTimeDeltaX += TimeDelta * m_ValueFixDesc.fTimeX;
		m_fTimeDeltaY += TimeDelta * m_ValueFixDesc.fTimeY;
		m_fTimeDeltaZ += TimeDelta * m_ValueFixDesc.fTimeZ;
	}

	if (m_EffectDesc.IsDead)
		return ISDEAD;
	else
		return 0;
}

_int CEffect_BillboardValueFix_Effect::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	return 0;
}

HRESULT CEffect_BillboardValueFix_Effect::Render()
{
	//	색상 바인드
	if (FAILED(m_pShaderCom->Set_RawValue("g_vRGBA", &m_EffectDesc.vRGBA, sizeof(_float4))))
		return E_FAIL;

	//	최대 프레임 갯수(스프라이트) 가로
	if (FAILED(m_pShaderCom->Set_RawValue("g_iCX", &m_EffectDesc.iTexNumX, sizeof(int))))
		return E_FAIL;

	//	최대 프레임 갯수(스프라이트) 세로
	if (FAILED(m_pShaderCom->Set_RawValue("g_iCY", &m_EffectDesc.iTexNumY, sizeof(int))))
		return E_FAIL;

	_float fAlpha = 0.f;
	if (FAILED(m_pShaderCom->Set_RawValue("g_Alpha", &fAlpha, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_BillboardValueFix_Effect::SetUp_ConstantTable_BillBoarding()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix WorldMatrix;

	//	부모가 있을경우, 부모의 빌보드 행렬을 바인드한다
	if (m_EffectDesc.m_pParent != nullptr)
	{
		WorldMatrix = m_EffectDesc.m_pParent->Get_BillBoardMatrix();
	}
	else
	{
		WorldMatrix = XMLoadFloat4x4(&m_EffectDesc.matBillboard);
	}

	_float4x4 BillboardMatrix;

	XMStoreFloat4x4(&BillboardMatrix,
		XMMatrixRotationRollPitchYaw(XMConvertToRadians(0.f + m_ValueFixDesc.fRoll), XMConvertToRadians(0.f + m_ValueFixDesc.fPitch), XMConvertToRadians(0.f + m_ValueFixDesc.fYaw)) *
		XMMatrixTranslation(0.f + m_ValueFixDesc.fValueX + m_fTimeDeltaX, 0.f + m_ValueFixDesc.fValueY + m_fTimeDeltaY, 0.f + m_ValueFixDesc.fValueZ + m_fTimeDeltaZ)
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

	//	HDR
	if (FAILED(m_pShaderCom->Set_RawValue("g_bHDR", &g_bHDR, sizeof(_bool))))
		return E_FAIL;

	//	이미시브 & 글로우
	if (FAILED(m_pShaderCom->Set_RawValue("g_bEmissive", &m_EffectDesc.bGlow, sizeof(_bool))))
		return E_FAIL;

	return S_OK;
}

CEffect_BillboardValueFix_Effect* CEffect_BillboardValueFix_Effect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CEffect_BillboardValueFix_Effect* pInstance = new CEffect_BillboardValueFix_Effect(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CEffect_BillboardValueFix_Effect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEffect_BillboardValueFix_Effect::Clone(void* pArg)
{
	CEffect_BillboardValueFix_Effect* pInstance = new CEffect_BillboardValueFix_Effect(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CEffect_BillboardValueFix_Effect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_BillboardValueFix_Effect::Free()
{
	Safe_Release((m_pShaderCom));
	Safe_Release((m_pModelCom));
	__super::Free();

}
