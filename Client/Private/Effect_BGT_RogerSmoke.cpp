#include "stdafx.h"
#include "Effect_BGT_RogerSmoke.h"
#include "GameInstance.h"

CEffect_BGT_RogerSmoke::CEffect_BGT_RogerSmoke(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CEffect_None_Anim_Effect(pDevice, pDeviceContext)
{
}

CEffect_BGT_RogerSmoke::CEffect_BGT_RogerSmoke(const CEffect_BGT_RogerSmoke& rhs)
	: CEffect_None_Anim_Effect(rhs)
{
}

HRESULT CEffect_BGT_RogerSmoke::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_BGT_RogerSmoke::NativeConstruct(void* pArg)
{
	//	트랜스폼, 모델, 셰이더, 랜더러 추가
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	ZeroMemory(&m_BGTRogerSmokeDesc, sizeof(BGTROGERSMOKEDESC));
	memcpy(&m_BGTRogerSmokeDesc, pArg, sizeof(BGTROGERSMOKEDESC));

	SetMaterialIndex();

	XMStoreFloat3(&m_EffectDesc.vScaleOrigin, m_pTransform->Get_Scale());

	return S_OK;
}

_int CEffect_BGT_RogerSmoke::Tick(_double TimeDelta)
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

	return temp;
}

_int CEffect_BGT_RogerSmoke::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	if (m_EffectDesc.bGlow)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT_SECOND_GLOW, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT_SECOND, this);

	return 0;
}

HRESULT CEffect_BGT_RogerSmoke::Render()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix WorldMatrix;

	WorldMatrix = XMLoadFloat4x4(&m_EffectDesc.matBillboard);

	_float4x4 BillboardMatrix;

	XMStoreFloat4x4(&BillboardMatrix,
		XMMatrixRotationRollPitchYaw(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f)) *
		XMMatrixTranslation(m_BGTRogerSmokeDesc.fValueX, m_BGTRogerSmokeDesc.fValueY, m_BGTRogerSmokeDesc.fValueZ)
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

	//	최대 프레임 갯수(스프라이트) 가로
	if (FAILED(m_pShaderCom->Set_RawValue("g_iCX", &m_EffectDesc.iTexNumX, sizeof(int))))
		return E_FAIL;

	//	최대 프레임 갯수(스프라이트) 세로
	if (FAILED(m_pShaderCom->Set_RawValue("g_iCY", &m_EffectDesc.iTexNumY, sizeof(int))))
		return E_FAIL;

	_float fAlpha = 0.4f;
	if (FAILED(m_pShaderCom->Set_RawValue("g_Alpha", &fAlpha, sizeof(_float))))
		return E_FAIL;

	//	텍스처 프레임 바인드
	_float2 vSeqUV = { 0.f, 0.f };

	vSeqUV.x = ((int)(m_fTime * m_EffectDesc.iTexFrameMax) % m_EffectDesc.iTexNumX) / (_float)m_EffectDesc.iTexNumX;
	vSeqUV.y = floor(m_fTime * m_EffectDesc.iTexFrameMax / m_EffectDesc.iTexNumX) / (_float)m_EffectDesc.iTexNumY;

	if (1.f >= m_EffectDesc.fAlpha)
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_SeqUV", &vSeqUV, sizeof(_float2))))
			return E_FAIL;

		m_pModelCom->Bind_OnShader(m_pShaderCom, m_iMaterialIndex, aiTextureType_DIFFUSE, "g_DiffuseTexture");
		m_pModelCom->Render(m_pShaderCom, m_EffectDesc.iEffectPass, m_iMaterialIndex);
	}

	return S_OK;
}

void CEffect_BGT_RogerSmoke::SetMaterialIndex()
{
	switch (m_BGTRogerSmokeDesc.eIndex)
	{
	case SMOKE_00:
		m_iMaterialIndex = 0;
		break;
	case SMOKE_01:
		m_iMaterialIndex = 1;
		break;
	case SMOKE_02:
		m_iMaterialIndex = 2;
		break;
	}
}

CEffect_BGT_RogerSmoke* CEffect_BGT_RogerSmoke::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CEffect_BGT_RogerSmoke* pInstance = new CEffect_BGT_RogerSmoke(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CEffect_BGT_RogerSmoke");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEffect_BGT_RogerSmoke::Clone(void* pArg)
{
	CEffect_BGT_RogerSmoke* pInstance = new CEffect_BGT_RogerSmoke(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CEffect_BGT_RogerSmoke");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_BGT_RogerSmoke::Free()
{
	__super::Free();
}
