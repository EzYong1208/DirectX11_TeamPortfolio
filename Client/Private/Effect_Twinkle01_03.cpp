#include "stdafx.h"
#include "Effect_Twinkle01_03.h"
#include "GameInstance.h"

CEffect_Twinkle01_03::CEffect_Twinkle01_03(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CEffect_None_Anim_Effect(pDevice, pDeviceContext)
{
}

CEffect_Twinkle01_03::CEffect_Twinkle01_03(const CEffect_Twinkle01_03& rhs)
	: CEffect_None_Anim_Effect(rhs)
{
}

HRESULT CEffect_Twinkle01_03::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Twinkle01_03::NativeConstruct(void* pArg)
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

_int CEffect_Twinkle01_03::Tick(_double TimeDelta)
{
	int temp = 0;

	m_fTime += (_float)TimeDelta * m_EffectDesc.fSpeed;
	m_fTimeDelta += TimeDelta;

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

_int CEffect_Twinkle01_03::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	//랜더러에 추가
	//if (m_EffectDesc.bGlow)
	//	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT_FIRST_GLOW, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT_SECOND, this);

	return 0;
}

HRESULT CEffect_Twinkle01_03::Render()
{
	//if (nullptr == m_pModelCom)
	//	return E_FAIL;

	//CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//_matrix WorldMatrix;

	//WorldMatrix = XMLoadFloat4x4(&m_EffectDesc.matBillboard);

	//_float4x4 BillboardMatrix;

	//XMStoreFloat4x4(&BillboardMatrix,
	//	XMMatrixRotationRollPitchYaw(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f)) *
	//	XMMatrixTranslation(0.f - (m_fTimeDelta * 1.2f), 0.f - (m_fTimeDelta * 0.5f), 0.f)
	//	* WorldMatrix);

	//_float4x4 ResultWorldMatrix;

	//XMStoreFloat4x4(&ResultWorldMatrix, XMMatrixTranspose(XMLoadFloat4x4(&BillboardMatrix)));

	//if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &ResultWorldMatrix, sizeof(_float4x4))))
	//	return E_FAIL;

	//if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
	//	return E_FAIL;

	//if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
	//	return E_FAIL;

	//RELEASE_INSTANCE(CGameInstance);

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
		m_pModelCom->Bind_OnShader(m_pShaderCom, 0, aiTextureType_DIFFUSE, "g_DiffuseTexture");
		m_pModelCom->Render(m_pShaderCom, m_EffectDesc.iEffectPass, 0);
	}

	return S_OK;
}

void CEffect_Twinkle01_03::Trigger_Activate(const _double& _fTimeDelta)
{
}

HRESULT CEffect_Twinkle01_03::SetUp_Components()
{
	return S_OK;
}

CEffect_Twinkle01_03* CEffect_Twinkle01_03::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CEffect_Twinkle01_03* pInstance = new CEffect_Twinkle01_03(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CEffect_Twinkle01_03");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEffect_Twinkle01_03::Clone(void* pArg)
{
	CEffect_Twinkle01_03* pInstance = new CEffect_Twinkle01_03(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CEffect_Twinkle01_03");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_Twinkle01_03::Free()
{
	__super::Free();
}
