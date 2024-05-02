#include "stdafx.h"
#include "Effect_Splashcylinder2.h"
#include "GameInstance.h"

CEffect_Splashcylinder2::CEffect_Splashcylinder2(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CEffect_None_Anim_Effect(pDevice, pDeviceContext)
{
}

CEffect_Splashcylinder2::CEffect_Splashcylinder2(const CEffect_Splashcylinder2& rhs)
	: CEffect_None_Anim_Effect(rhs)
{
}

HRESULT CEffect_Splashcylinder2::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Splashcylinder2::NativeConstruct(void* pArg)
{
	//	Ʈ������, ��, ���̴�, ������ �߰�
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	//	Ʈ���� �ؽ�ó �߰�
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	XMStoreFloat3(&m_EffectDesc.vScaleOrigin, m_pTransform->Get_Scale());

	return S_OK;
}

_int CEffect_Splashcylinder2::Tick(_double TimeDelta)
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

_int CEffect_Splashcylinder2::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	//�������� �߰�
	//if (m_EffectDesc.bGlow)
	//	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT_FIRST_GLOW, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT_SECOND, this);
	m_FlashTimeDelta = TimeDelta;

	return 0;
}

HRESULT CEffect_Splashcylinder2::Render()
{
	if (FAILED(SetUp_ConstantTable_BillBoarding()))
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

	//m_EffectDesc.fAlpha += m_FlashTimeDelta * 4.5f;
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

void CEffect_Splashcylinder2::Trigger_Activate(const _double& _fTimeDelta)
{
}

HRESULT CEffect_Splashcylinder2::SetUp_Components()
{
	return S_OK;
}

CEffect_Splashcylinder2* CEffect_Splashcylinder2::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CEffect_Splashcylinder2* pInstance = new CEffect_Splashcylinder2(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CEffect_Splashcylinder2");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEffect_Splashcylinder2::Clone(void* pArg)
{
	CEffect_Splashcylinder2* pInstance = new CEffect_Splashcylinder2(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CEffect_Splashcylinder2");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_Splashcylinder2::Free()
{
	__super::Free();
}
