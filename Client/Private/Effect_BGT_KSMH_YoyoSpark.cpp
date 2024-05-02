#include "stdafx.h"
#include "Effect_BGT_KSMH_YoyoSpark.h"
#include "GameInstance.h"

CEffect_BGT_KSMH_YoyoSpark::CEffect_BGT_KSMH_YoyoSpark(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CEffect_BillboardValueFix_Effect(pDevice, pDeviceContext)
{
}

CEffect_BGT_KSMH_YoyoSpark::CEffect_BGT_KSMH_YoyoSpark(const CEffect_BGT_KSMH_YoyoSpark& rhs)
	: CEffect_BillboardValueFix_Effect(rhs)
{
}

HRESULT CEffect_BGT_KSMH_YoyoSpark::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_BGT_KSMH_YoyoSpark::NativeConstruct(void* pArg)
{
	//	Ʈ������, ��, ���̴�, ������ �߰�
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	XMStoreFloat3(&m_EffectDesc.vScaleOrigin, m_pTransform->Get_Scale());

	return S_OK;
}

_int CEffect_BGT_KSMH_YoyoSpark::Tick(_double TimeDelta)
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

	temp = SetMaterialIndex();

	return temp;
}

_int CEffect_BGT_KSMH_YoyoSpark::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	if (m_EffectDesc.bGlow)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT_SECOND_GLOW, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT_SECOND, this);

	return 0;
}

HRESULT CEffect_BGT_KSMH_YoyoSpark::Render()
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

	_float fAlpha = 0.f;
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

_uint CEffect_BGT_KSMH_YoyoSpark::SetMaterialIndex()
{
	_double fParentCurAcc = m_EffectDesc.m_pParent->Get_ModelSlot()->Get_PlayAcc(MODEL_TAG_DEFAULT_BODY);
	_uint	iResult = 0;

	if (m_iCount == 0 && fParentCurAcc >= 5.f)
	{
		++m_iCount;
		m_iMaterialIndex = 0;
	}
	if (m_iCount == 1 && fParentCurAcc >= 20.f)
	{
		++m_iCount;
		if (m_iLoopTime < 2)
		{
			m_iLoopTime++;
			m_iCount = 0;
		}
		else
		{
			m_iMaterialIndex = 1;
			m_EffectDesc.fSpeed = 10.f;
		}
	}
	if (m_iCount == 2 && fParentCurAcc >= 25.f)
	{
		++m_iCount;
		iResult = ISDEAD;
	}

	return iResult;
}

CEffect_BGT_KSMH_YoyoSpark* CEffect_BGT_KSMH_YoyoSpark::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{

	CEffect_BGT_KSMH_YoyoSpark* pInstance = new CEffect_BGT_KSMH_YoyoSpark(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CEffect_BGT_KSMH_YoyoSpark");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEffect_BGT_KSMH_YoyoSpark::Clone(void* pArg)
{
	CEffect_BGT_KSMH_YoyoSpark* pInstance = new CEffect_BGT_KSMH_YoyoSpark(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CEffect_BGT_KSMH_YoyoSpark");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_BGT_KSMH_YoyoSpark::Free()
{
	__super::Free();
}