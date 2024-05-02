#include "stdafx.h"
#include "Effect_BGT_RKM_Bubble.h"
#include "GameInstance.h"

CEffect_BGT_RKM_Bubble::CEffect_BGT_RKM_Bubble(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CEffect_None_Anim_Effect(pDevice, pDeviceContext)
{
}

CEffect_BGT_RKM_Bubble::CEffect_BGT_RKM_Bubble(const CEffect_BGT_RKM_Bubble& rhs)
	: CEffect_None_Anim_Effect(rhs)
{
}

HRESULT CEffect_BGT_RKM_Bubble::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_BGT_RKM_Bubble::NativeConstruct(void* pArg)
{
	//	트랜스폼, 모델, 셰이더, 랜더러 추가
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	ZeroMemory(&m_BGTRKMBubbleDesc, sizeof(BGTRKMBUBBLEDESC));
	memcpy(&m_BGTRKMBubbleDesc, pArg, sizeof(BGTRKMBUBBLEDESC));

	SetMaterialIndex();

	XMStoreFloat3(&m_EffectDesc.vScaleOrigin, m_pTransform->Get_Scale());

	return S_OK;
}

_int CEffect_BGT_RKM_Bubble::Tick(_double TimeDelta)
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

_int CEffect_BGT_RKM_Bubble::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	if (m_EffectDesc.bGlow)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT_SECOND_GLOW, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT_SECOND, this);

	return 0;
}

HRESULT CEffect_BGT_RKM_Bubble::Render()
{
	DEBUG_BREAK(SetPosition());

	DEBUG_BREAK(SetUp_ConstantTable_BillBoarding());

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
	if (4  == m_iMaterialIndex)
	{
		vSeqUV.x = ((int)(m_fTime * m_EffectDesc.iTexFrameMax) % m_EffectDesc.iTexNumX) / (_float)m_EffectDesc.iTexNumX;
		vSeqUV.y = floor(m_fTime * m_EffectDesc.iTexFrameMax / m_EffectDesc.iTexNumX) / (_float)m_EffectDesc.iTexNumY;
	}
	else
	{
		m_EffectDesc.iTexNumX = 1;
		m_EffectDesc.iTexNumY = 1;
	}

	if (1.f >= m_EffectDesc.fAlpha)
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_SeqUV", &vSeqUV, sizeof(_float2))))
			return E_FAIL;
		m_pModelCom->Bind_OnShader(m_pShaderCom, m_iMaterialIndex, aiTextureType_DIFFUSE, "g_DiffuseTexture");
		m_pModelCom->Render(m_pShaderCom, m_EffectDesc.iEffectPass, m_iMaterialIndex);
	}

	return S_OK;
}

void CEffect_BGT_RKM_Bubble::SetMaterialIndex()
{
	switch (m_BGTRKMBubbleDesc.eIndex)
	{
	case BUBBLE_RETURN:
		m_iMaterialIndex = 4;
		break;
	default:
		if (m_BGTRKMBubbleDesc.bReverse)
			m_iMaterialIndex = rand() % 2 + 2;
		else
			m_iMaterialIndex = rand() % 2;
		break;
	}
}

HRESULT CEffect_BGT_RKM_Bubble::SetPosition()
{
	switch (m_BGTRKMBubbleDesc.eType)
	{
	case TYPE_GROUND:
		if (m_BGTRKMBubbleDesc.eIndex == BUBBLE_RETURN)
		{
			m_fValueX = -0.2f;
			m_fValueY = 0.f;
		}
		else
		{
			m_fValueX = 0.f;
			m_fValueY = 0.f;
		}
		break;

	case TYPE_AIR:
		if (m_BGTRKMBubbleDesc.eIndex == BUBBLE_RETURN)
		{
			m_fValueX = -0.23f;
			m_fValueY = 0.1f;
		}
		else
		{
			m_fValueX = -0.03f;
			m_fValueY = 0.1f;
		}
		break;
	}

	return S_OK;
}

HRESULT CEffect_BGT_RKM_Bubble::SetUp_ConstantTable_BillBoarding()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix WorldMatrix;

	WorldMatrix = m_EffectDesc.m_pParent->Get_BillBoardMatrix();

	_float4x4 BillboardMatrix;

	XMStoreFloat4x4(&BillboardMatrix,
		XMMatrixRotationRollPitchYaw(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f)) *
		XMMatrixTranslation(m_fValueX, m_fValueY, 0.f)
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

CEffect_BGT_RKM_Bubble* CEffect_BGT_RKM_Bubble::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CEffect_BGT_RKM_Bubble* pInstance = new CEffect_BGT_RKM_Bubble(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CEffect_BGT_RKM_Bubble");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEffect_BGT_RKM_Bubble::Clone(void* pArg)
{
	CEffect_BGT_RKM_Bubble* pInstance = new CEffect_BGT_RKM_Bubble(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CEffect_BGT_RKM_Bubble");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_BGT_RKM_Bubble::Free()
{
	__super::Free();
}
