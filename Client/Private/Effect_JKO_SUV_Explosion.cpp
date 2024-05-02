#include "stdafx.h"
#include "Effect_JKO_SUV_Explosion.h"
#include "GameInstance.h"

CEffect_JKO_SUV_Explosion::CEffect_JKO_SUV_Explosion(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CEffect_None_Anim_Effect(pDevice, pDeviceContext)
{
}

CEffect_JKO_SUV_Explosion::CEffect_JKO_SUV_Explosion(const CEffect_JKO_SUV_Explosion& rhs)
	: CEffect_None_Anim_Effect(rhs)
{
}

HRESULT CEffect_JKO_SUV_Explosion::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_JKO_SUV_Explosion::NativeConstruct(void* pArg)
{
	//	트랜스폼, 모델, 셰이더, 랜더러 추가 
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	XMStoreFloat3(&m_EffectDesc.vScaleOrigin, m_pTransform->Get_Scale());

	return S_OK;
}

_int CEffect_JKO_SUV_Explosion::Tick(_double TimeDelta)
{
	int temp = 0;

	m_fTime += (_float)TimeDelta * m_EffectDesc.fSpeed;

	m_fTimeDelta = TimeDelta;

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

	DEBUG_BREAK(CreateEffect_SmallExplosion());

	return temp;
}

_int CEffect_JKO_SUV_Explosion::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	if (m_EffectDesc.bGlow)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT_SECOND_GLOW, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT_SECOND, this);

	return 0;
}

HRESULT CEffect_JKO_SUV_Explosion::Render()
{
	DEBUG_BREAK(SetBillboardMatrix());

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

	_float fAlpha = 0.f;
	if (FAILED(m_pShaderCom->Set_RawValue("g_Alpha", &fAlpha, sizeof(_float))))
		return E_FAIL;

	//	텍스처 프레임 바인드
	_float2 vSeqUV = { 0.f, 0.f };

	m_EffectDesc.iTexNumX = 1;
	m_EffectDesc.iTexNumY = 1;

	if (1.f >= m_EffectDesc.fAlpha)
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_SeqUV", &vSeqUV, sizeof(_float2))))
			return E_FAIL;
		m_pModelCom->Bind_OnShader(m_pShaderCom, 0, aiTextureType_DIFFUSE, "g_DiffuseTexture");
		m_pModelCom->Render(m_pShaderCom, m_EffectDesc.iEffectPass, 0);
	}

	return S_OK;
}

HRESULT CEffect_JKO_SUV_Explosion::SetBillboardMatrix()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;
	 
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix WorldMatrix;

	WorldMatrix = XMLoadFloat4x4(&m_EffectDesc.matBillboard);

	_float4x4 BillboardMatrix;

	if (m_bGetBig)
	{
		if (1.5f <= m_fSize)
		{
			m_bGetBig = false;
		}

		m_fSize += m_fTimeDelta * 3.4f;
	}
	else
	{
		m_fSize -= m_fTimeDelta * 3.4f;
	}

	XMStoreFloat4x4(&BillboardMatrix,
		XMMatrixScaling(m_fSize, m_fSize, m_fSize) *
		XMMatrixRotationRollPitchYaw(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f)) *
		XMMatrixTranslation(0.1f, 0.3f, 0.f)
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

	return S_OK;
}

HRESULT CEffect_JKO_SUV_Explosion::CreateEffect_SmallExplosion()
{
	if (m_fSize >= 1.3f)
		m_bCreateEffect = true;

	if (m_iCount == 0 && m_bCreateEffect)
	{
		++m_iCount;
		m_bCreateEffect = false;

		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		RELEASE_INSTANCE(CGameInstance);
	}

	return S_OK;
}

CEffect_JKO_SUV_Explosion* CEffect_JKO_SUV_Explosion::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CEffect_JKO_SUV_Explosion* pInstance = new CEffect_JKO_SUV_Explosion(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CEffect_JKO_SUV_Explosion");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEffect_JKO_SUV_Explosion::Clone(void* pArg)
{
	CEffect_JKO_SUV_Explosion* pInstance = new CEffect_JKO_SUV_Explosion(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CEffect_JKO_SUV_Explosion");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_JKO_SUV_Explosion::Free()
{
	__super::Free();
}
