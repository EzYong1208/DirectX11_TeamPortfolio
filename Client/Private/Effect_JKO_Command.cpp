#include "stdafx.h"
#include "Effect_JKO_Command.h"
#include "GameInstance.h"

CEffect_JKO_Command::CEffect_JKO_Command(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CEffect_None_Anim_Effect(pDevice, pDeviceContext)
{
}

CEffect_JKO_Command::CEffect_JKO_Command(const CEffect_JKO_Command& rhs)
	: CEffect_None_Anim_Effect(rhs)
{
}

HRESULT CEffect_JKO_Command::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_JKO_Command::NativeConstruct(void* pArg)
{
	//	트랜스폼, 모델, 셰이더, 랜더러 추가
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	ZeroMemory(&m_JKOCommandDesc, sizeof(JKOCOMMANDDESC));
	memcpy(&m_JKOCommandDesc, pArg, sizeof(JKOCOMMANDDESC));

	SetMaterialIndex();

	XMStoreFloat3(&m_EffectDesc.vScaleOrigin, m_pTransform->Get_Scale());

	return S_OK;
}

_int CEffect_JKO_Command::Tick(_double TimeDelta)
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

_int CEffect_JKO_Command::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	if (m_EffectDesc.bGlow)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT_SECOND_GLOW, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT_SECOND, this);

	return 0;
}

HRESULT CEffect_JKO_Command::Render()
{
	DEBUG_BREAK(SetPosition());

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
	if (4 <= m_iMaterialIndex)
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

void CEffect_JKO_Command::SetMaterialIndex()
{
	switch (m_JKOCommandDesc.eIndex)
	{
	case COMMAND_DEFEND:
		m_iMaterialIndex = 0;
		break;
	case COMMAND_ATTACK:
		m_iMaterialIndex = 1;
		break;
	case COMMAND_RECOVER:
		m_iMaterialIndex = 2;
		break;
	case COMMAND_EXPLOSION:
		m_iMaterialIndex = 3;
		break;
	case COMMAND_DEFENDBUBBLE:
		m_iMaterialIndex = 4;
		break;
	case COMMAND_ATTACKBUBBLE:
		m_iMaterialIndex = 5;
		break;
	case COMMAND_RECOVERBUBBLE:
		m_iMaterialIndex = 6;
		break;
	case COMMAND_EXPLOSIONBUBBLE:
		m_iMaterialIndex = 7;
		break;
	}
}

HRESULT CEffect_JKO_Command::SetPosition()
{
	switch (m_JKOCommandDesc.eType)
	{
	case TYPE_GROUND:
		if (FAILED(SetUp_ConstantTable_BillBoarding()))
			return E_FAIL;
		break;

	case TYPE_AIR:
		if (nullptr == m_pModelCom)
			return E_FAIL;

		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		
		_matrix WorldMatrix;

		WorldMatrix = m_EffectDesc.m_pParent->Get_BillBoardMatrix();

		_float4x4 BillboardMatrix;

		XMStoreFloat4x4(&BillboardMatrix,
			XMMatrixRotationRollPitchYaw(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f)) *
			XMMatrixTranslation(0.3f, 0.f, 0.f)
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
		break;
	}

	return S_OK;
}

CEffect_JKO_Command* CEffect_JKO_Command::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CEffect_JKO_Command* pInstance = new CEffect_JKO_Command(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CEffect_JKO_Command");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEffect_JKO_Command::Clone(void* pArg)
{
	CEffect_JKO_Command* pInstance = new CEffect_JKO_Command(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CEffect_JKO_Command");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_JKO_Command::Free()
{
	__super::Free();
}
