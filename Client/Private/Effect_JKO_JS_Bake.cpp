#include "stdafx.h"
#include "Effect_JKO_JS_Bake.h"
#include "GameInstance.h"

CEffect_JKO_JS_Bake::CEffect_JKO_JS_Bake(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CEffect_None_Anim_Effect(pDevice, pDeviceContext)
{
}

CEffect_JKO_JS_Bake::CEffect_JKO_JS_Bake(const CEffect_JKO_JS_Bake& rhs)
	: CEffect_None_Anim_Effect(rhs)
{
}

HRESULT CEffect_JKO_JS_Bake::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_JKO_JS_Bake::NativeConstruct(void* pArg)
{
	//	트랜스폼, 모델, 셰이더, 랜더러 추가
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	XMStoreFloat3(&m_EffectDesc.vScaleOrigin, m_pTransform->Get_Scale());

	return S_OK;
}

_int CEffect_JKO_JS_Bake::Tick(_double TimeDelta)
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

	temp = SetMaterialIndex();

	m_pTransform->Reset_Momentum();

	return temp;
}

_int CEffect_JKO_JS_Bake::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	if (m_EffectDesc.bGlow)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT_SECOND_GLOW, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT_SECOND, this);

	return 0;
}

HRESULT CEffect_JKO_JS_Bake::Render()
{
	if (FAILED(SetUp_ConstantTable_BillBoarding()))
		return E_FAIL;

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

	vSeqUV.x = ((int)(m_fTime * m_EffectDesc.iTexFrameMax) % m_EffectDesc.iTexNumX) / (_float)m_EffectDesc.iTexNumX;
	vSeqUV.y = floor(m_fTime * m_EffectDesc.iTexFrameMax / m_EffectDesc.iTexNumX) / (_float)m_EffectDesc.iTexNumY;

	if (1.f >= m_EffectDesc.fAlpha)
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_SeqUV", &vSeqUV, sizeof(_float2))))
			return E_FAIL;

		m_pModelCom->Bind_OnShader(m_pShaderCom, m_iMaterialIndex, aiTextureType_DIFFUSE, "g_DiffuseTexture");
		m_pModelCom->Render(m_pShaderCom, m_EffectDesc.iEffectPass, m_iMaterialIndex);
		m_pModelCom->Bind_OnShader(m_pShaderCom, m_iMaterialIndex + 4, aiTextureType_DIFFUSE, "g_DiffuseTexture");
		m_pModelCom->Render(m_pShaderCom, m_EffectDesc.iEffectPass, m_iMaterialIndex + 4);
	}

	return S_OK;
}

_uint CEffect_JKO_JS_Bake::SetMaterialIndex()
{
	_double fParentCurAcc = m_EffectDesc.m_pParent->Get_ModelSlot()->Get_PlayAcc(MODEL_TAG_DEFAULT_BODY);
	_uint	iResult = 0;

	if (m_iCount == 0 && fParentCurAcc >= 15.f)
	{
		++m_iCount;
		m_iMaterialIndex = 0;
	}
	if (m_iCount == 1 && fParentCurAcc >= 21.f)
	{
		++m_iCount;
		m_iMaterialIndex = 1;
	}
	if (m_iCount == 2 && fParentCurAcc >= 26.f)
	{
		++m_iCount;
		m_iMaterialIndex = 2;
	}
	if (m_iCount == 3 && fParentCurAcc >= 31.f)
	{
		++m_iCount;
		m_iMaterialIndex = 3;
	}
	if (m_iCount == 4 && fParentCurAcc >= 36.f)
	{
		++m_iCount;
		iResult = ISDEAD;
	}

	return iResult;
}

CEffect_JKO_JS_Bake* CEffect_JKO_JS_Bake::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CEffect_JKO_JS_Bake* pInstance = new CEffect_JKO_JS_Bake(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CEffect_JKO_JS_Bake");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEffect_JKO_JS_Bake::Clone(void* pArg)
{
	CEffect_JKO_JS_Bake* pInstance = new CEffect_JKO_JS_Bake(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CEffect_JKO_JS_Bake");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_JKO_JS_Bake::Free()
{
	__super::Free();
}
