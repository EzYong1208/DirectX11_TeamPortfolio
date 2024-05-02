#include "stdafx.h"
#include "Effect_MAY_Anchor_Sit_Attack1.h"
#include "GameInstance.h"

CEffect_MAY_Anchor_Sit_Attack1::CEffect_MAY_Anchor_Sit_Attack1(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CEffect_None_Anim_Effect(pDevice, pDeviceContext)
{
}

CEffect_MAY_Anchor_Sit_Attack1::CEffect_MAY_Anchor_Sit_Attack1(const CEffect_MAY_Anchor_Sit_Attack1& rhs)
	: CEffect_None_Anim_Effect(rhs)
{
}

HRESULT CEffect_MAY_Anchor_Sit_Attack1::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_MAY_Anchor_Sit_Attack1::NativeConstruct(void* pArg)
{
	//	트랜스폼, 모델, 셰이더, 랜더러 추가
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	//	트레일 텍스처 추가
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	XMStoreFloat3(&m_EffectDesc.vScaleOrigin, m_pTransform->Get_Scale());

	return S_OK;
}

_int CEffect_MAY_Anchor_Sit_Attack1::Tick(_double TimeDelta)
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

_int CEffect_MAY_Anchor_Sit_Attack1::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	//랜더러에 추가
	//if (m_EffectDesc.bGlow)
	//	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT_FIRST_GLOW, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT_SECOND, this);
	m_FlashTimeDelta = TimeDelta;

	return 0;
}

HRESULT CEffect_MAY_Anchor_Sit_Attack1::Render()
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

	//	텍스처 프레임 바인드
	_float2 vSeqUV = { 0.f, 0.f };
	if (m_EffectDesc.iTexFrameMax > 1)
	{
		vSeqUV.x = 0;
		vSeqUV.y = 0;
	}
	else
	{
		m_EffectDesc.iTexNumX = 1;
		m_EffectDesc.iTexNumY = 1;
	}

	if (FAILED(m_pShaderCom->Set_RawValue("g_SeqUV", &vSeqUV, sizeof(_float2))))
		return E_FAIL;

	m_pModelCom->Bind_OnShader(m_pShaderCom, 0, aiTextureType_DIFFUSE, "g_DiffuseTexture");
	m_pModelCom->Render(m_pShaderCom, m_EffectDesc.iEffectPass, 0);

	return S_OK;
}

void CEffect_MAY_Anchor_Sit_Attack1::Trigger_Activate(const _double& _fTimeDelta)
{
}

HRESULT CEffect_MAY_Anchor_Sit_Attack1::SetUp_Components()
{
	return S_OK;
}

CEffect_MAY_Anchor_Sit_Attack1* CEffect_MAY_Anchor_Sit_Attack1::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CEffect_MAY_Anchor_Sit_Attack1* pInstance = new CEffect_MAY_Anchor_Sit_Attack1(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CEffect_MAY_Anchor_Sit_Attack1");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEffect_MAY_Anchor_Sit_Attack1::Clone(void* pArg)
{
	CEffect_MAY_Anchor_Sit_Attack1* pInstance = new CEffect_MAY_Anchor_Sit_Attack1(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CEffect_MAY_Anchor_Sit_Attack1");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_MAY_Anchor_Sit_Attack1::Free()
{
	__super::Free();
}
