#include "stdafx.h"
#include "Effect_JKO_5H.h"
#include "GameInstance.h"

CEffect_JKO_5H::CEffect_JKO_5H(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CEffect_None_Anim_Effect(pDevice, pDeviceContext)
{
}

CEffect_JKO_5H::CEffect_JKO_5H(const CEffect_JKO_5H& rhs)
	: CEffect_None_Anim_Effect(rhs)
{
}

HRESULT CEffect_JKO_5H::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_JKO_5H::NativeConstruct(void* pArg)
{
	//	Ʈ������, ��, ���̴�, ������ �߰�
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	ZeroMemory(&m_JKO5HDesc, sizeof(JKO5HDESC));
	memcpy(&m_JKO5HDesc, pArg, sizeof(JKO5HDESC));

	SetMaterialIndex();

	XMStoreFloat3(&m_EffectDesc.vScaleOrigin, m_pTransform->Get_Scale());

	return S_OK;
}

_int CEffect_JKO_5H::Tick(_double TimeDelta)
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

_int CEffect_JKO_5H::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	if (m_EffectDesc.bGlow)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT_SECOND_GLOW, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT_SECOND, this);

	return 0;
}

HRESULT CEffect_JKO_5H::Render()
{
	if (FAILED(SetUp_ConstantTable_BillBoarding()))
		return E_FAIL;

	// HDR
	if (FAILED(m_pShaderCom->Set_RawValue("g_bHDR", &g_bHDR, sizeof(_bool))))
		return E_FAIL;

	// �̹̽ú� & �۷ο�
	if (FAILED(m_pShaderCom->Set_RawValue("g_bEmissive", &m_EffectDesc.bGlow, sizeof(_bool))))
		return E_FAIL;

	//	���� ���ε�
	if (FAILED(m_pShaderCom->Set_RawValue("g_vRGBA", &m_EffectDesc.vRGBA, sizeof(_float4))))
		return E_FAIL;

	//	�ִ� ������ ����(��������Ʈ) ����
	if (FAILED(m_pShaderCom->Set_RawValue("g_iCX", &m_EffectDesc.iTexNumX, sizeof(int))))
		return E_FAIL;

	//	�ִ� ������ ����(��������Ʈ) ���� 
	if (FAILED(m_pShaderCom->Set_RawValue("g_iCY", &m_EffectDesc.iTexNumY, sizeof(int))))
		return E_FAIL;

	_float fAlpha = 0.5f;
	if (FAILED(m_pShaderCom->Set_RawValue("g_Alpha", &fAlpha, sizeof(_float))))
		return E_FAIL;

	//	�ؽ�ó ������ ���ε� 
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

void CEffect_JKO_5H::SetMaterialIndex()
{
	switch (m_JKO5HDesc.eIndex)
	{
	case WIND_00:
		m_iMaterialIndex = 0;
		break;
	case WIND_01:
		m_iMaterialIndex = 1;
		break;
	case WIND_02:
		m_iMaterialIndex = 2;
		break;
	case WIND_03:
		m_iMaterialIndex = 3;
		break;
	}
}

CEffect_JKO_5H* CEffect_JKO_5H::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CEffect_JKO_5H* pInstance = new CEffect_JKO_5H(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CEffect_JKO_5H");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEffect_JKO_5H::Clone(void* pArg)
{
	CEffect_JKO_5H* pInstance = new CEffect_JKO_5H(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CEffect_JKO_5H");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_JKO_5H::Free()
{
	__super::Free();
}
