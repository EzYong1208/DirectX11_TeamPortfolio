#include "stdafx.h"
#include "Effect_MAY_SlashSpark02.h"
#include "GameInstance.h"

CEffect_MAY_SlashSpark02::CEffect_MAY_SlashSpark02(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CEffect_BillboardValueFix_Effect(pDevice, pDeviceContext)
{
}

CEffect_MAY_SlashSpark02::CEffect_MAY_SlashSpark02(const CEffect_MAY_SlashSpark02& rhs)
	: CEffect_BillboardValueFix_Effect(rhs)
{
}

HRESULT CEffect_MAY_SlashSpark02::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_MAY_SlashSpark02::NativeConstruct(void* pArg)
{
	//	트랜스폼, 모델, 셰이더, 랜더러 추가
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	XMStoreFloat3(&m_EffectDesc.vScaleOrigin, m_pTransform->Get_Scale());

	return S_OK;
}

_int CEffect_MAY_SlashSpark02::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

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

_int CEffect_MAY_SlashSpark02::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	if (m_EffectDesc.bGlow)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT_SECOND_GLOW, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT_SECOND, this);

	return 0;
}

HRESULT CEffect_MAY_SlashSpark02::Render()
{
	if (FAILED(SetUp_ConstantTable_BillBoarding()))
		return E_FAIL;

	if (FAILED(__super::Render()))
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

CEffect_MAY_SlashSpark02* CEffect_MAY_SlashSpark02::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CEffect_MAY_SlashSpark02* pInstance = new CEffect_MAY_SlashSpark02(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CEffect_MAY_SlashSpark02");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEffect_MAY_SlashSpark02::Clone(void* pArg)
{
	CEffect_MAY_SlashSpark02* pInstance = new CEffect_MAY_SlashSpark02(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CEffect_MAY_SlashSpark02");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_MAY_SlashSpark02::Free()
{
	__super::Free();
}
