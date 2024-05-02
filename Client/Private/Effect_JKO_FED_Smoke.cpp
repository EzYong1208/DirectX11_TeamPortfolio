#include "stdafx.h"
#include "Effect_JKO_FED_Smoke.h"
#include "GameInstance.h"

CEffect_JKO_FED_Smoke::CEffect_JKO_FED_Smoke(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CEffect_None_Anim_Effect(pDevice, pDeviceContext)
{
}

CEffect_JKO_FED_Smoke::CEffect_JKO_FED_Smoke(const CEffect_JKO_FED_Smoke& rhs)
	: CEffect_None_Anim_Effect(rhs)
{
}

HRESULT CEffect_JKO_FED_Smoke::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_JKO_FED_Smoke::NativeConstruct(void* pArg)
{
	//	트랜스폼, 모델, 셰이더, 랜더러 추가
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	XMStoreFloat3(&m_EffectDesc.vScaleOrigin, m_pTransform->Get_Scale());

	if (nullptr == m_EffectDesc.m_pParent)
		m_bStart = true;
	else
		m_bStart = false;

	return S_OK;
}

_int CEffect_JKO_FED_Smoke::Tick(_double TimeDelta)
{
	int temp = 0;

	m_fTime += (_float)TimeDelta * m_EffectDesc.fSpeed;

	switch (m_iSequence)
	{
	case 0:

		if (m_bStart)
		{
			if (m_EffectDesc.fLifetime >= 0.f)
				m_EffectDesc.fLifetime -= (_float)TimeDelta;
			else
				temp = ISDEAD;
		}

		break;

	default:
		break;
	}

	m_pTransform->Reset_Momentum();

	return temp;
}

_int CEffect_JKO_FED_Smoke::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	if (!m_bStart && 4.f < m_fTime)
	{
		m_fTime = 0.f;
		m_bStart = true;
		XMStoreFloat4x4(&m_EffectDesc.matBillboard, m_EffectDesc.m_pParent->Get_BillBoardMatrix());
		m_EffectDesc.m_pParent = nullptr;
	}

	//랜더러에 추가
	if (m_bStart)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONLIGHT, this);

	return 0;
}

HRESULT CEffect_JKO_FED_Smoke::Render()
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
		XMMatrixRotationRollPitchYaw(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f)) *
		XMMatrixTranslation(1.f, 0.f, 0.f)
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

	//	색상 바인드
	if (FAILED(m_pShaderCom->Set_RawValue("g_vRGBA", &m_EffectDesc.vRGBA, sizeof(_float4))))
		return E_FAIL;

	m_pModelCom->Bind_OnShader(m_pShaderCom, _uint(m_fTime), aiTextureType_DIFFUSE, "g_DiffuseTexture");
	m_pModelCom->Render(m_pShaderCom, m_EffectDesc.iEffectPass, _uint(m_fTime));

	return S_OK;
}

CEffect_JKO_FED_Smoke* CEffect_JKO_FED_Smoke::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CEffect_JKO_FED_Smoke* pInstance = new CEffect_JKO_FED_Smoke(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CEffect_JKO_FED_Smoke");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_JKO_FED_Smoke::Clone(void* pArg)
{
	CEffect_JKO_FED_Smoke* pInstance = new CEffect_JKO_FED_Smoke(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CEffect_JKO_FED_Smoke");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_JKO_FED_Smoke::Free()
{
	__super::Free();
}
