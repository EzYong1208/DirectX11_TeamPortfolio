#include "stdafx.h"
#include "Effect_BKN_Sanju_TenchijinAnim.h"
#include "GameInstance.h"

CEffect_BKN_Sanju_TenchijinAnim::CEffect_BKN_Sanju_TenchijinAnim(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CEffect_BillboardValueFix_Effect(pDevice, pDeviceContext)
{
}

CEffect_BKN_Sanju_TenchijinAnim::CEffect_BKN_Sanju_TenchijinAnim(const CEffect_BKN_Sanju_TenchijinAnim& rhs)
	: CEffect_BillboardValueFix_Effect(rhs)
{
}

HRESULT CEffect_BKN_Sanju_TenchijinAnim::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_BKN_Sanju_TenchijinAnim::NativeConstruct(void* pArg)
{
	//	트랜스폼, 모델, 셰이더, 랜더러 추가
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	XMStoreFloat3(&m_EffectDesc.vScaleOrigin, m_pTransform->Get_Scale());

	return S_OK;
}

_int CEffect_BKN_Sanju_TenchijinAnim::Tick(_double TimeDelta)
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

	SetAlphaCase();

	m_pTransform->Reset_Momentum();

	return temp;
}

_int CEffect_BKN_Sanju_TenchijinAnim::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT_FIRST, this);

	return 0;
}

HRESULT CEffect_BKN_Sanju_TenchijinAnim::Render()
{
	if (FAILED(SetUp_ConstantTable_BillBoarding()))
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

	if (FAILED(m_pShaderCom->Set_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	//	텍스처 프레임 바인드
	_float2 vSeqUV = { 0.f, 0.f };

	vSeqUV.x = ((int)(m_fTime * m_EffectDesc.iTexFrameMax) % m_EffectDesc.iTexNumX) / (_float)m_EffectDesc.iTexNumX;
	vSeqUV.y = floor(m_fTime * m_EffectDesc.iTexFrameMax / m_EffectDesc.iTexNumX) / (_float)m_EffectDesc.iTexNumY;

	if (1.f >= m_EffectDesc.fAlpha)
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_SeqUV", &vSeqUV, sizeof(_float2))))
			return E_FAIL;
		m_pModelCom->Bind_OnShader(m_pShaderCom, 0, aiTextureType_DIFFUSE, "g_DiffuseTexture");
		m_pModelCom->Render(m_pShaderCom, m_EffectDesc.iEffectPass, 0);
	}

	return S_OK;
}

void CEffect_BKN_Sanju_TenchijinAnim::SetAlphaCase()
{
	m_fAlpha = 0.1f;
}

CEffect_BKN_Sanju_TenchijinAnim* CEffect_BKN_Sanju_TenchijinAnim::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CEffect_BKN_Sanju_TenchijinAnim* pInstance = new CEffect_BKN_Sanju_TenchijinAnim(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CEffect_BKN_Sanju_TenchijinAnim");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEffect_BKN_Sanju_TenchijinAnim::Clone(void* pArg)
{
	CEffect_BKN_Sanju_TenchijinAnim* pInstance = new CEffect_BKN_Sanju_TenchijinAnim(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CEffect_BKN_Sanju_TenchijinAnim");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_BKN_Sanju_TenchijinAnim::Free()
{
	__super::Free();
}
