#include "stdafx.h"
#include "Effect_BGT_RKM_FloorFire.h"
#include "GameInstance.h"

CEffect_BGT_RKM_FloorFire::CEffect_BGT_RKM_FloorFire(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CEffect_None_Anim_Effect(pDevice, pDeviceContext)
{
}

CEffect_BGT_RKM_FloorFire::CEffect_BGT_RKM_FloorFire(const CEffect_BGT_RKM_FloorFire& rhs)
	: CEffect_None_Anim_Effect(rhs)
{
}

HRESULT CEffect_BGT_RKM_FloorFire::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_BGT_RKM_FloorFire::NativeConstruct(void* pArg)
{
	//	트랜스폼, 모델, 셰이더, 랜더러 추가
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	XMStoreFloat3(&m_EffectDesc.vScaleOrigin, m_pTransform->Get_Scale());

	return S_OK;
}

_int CEffect_BGT_RKM_FloorFire::Tick(_double TimeDelta)
{
	int temp = 0;

	DEBUG_BREAK(CreateEffect_Ember(TimeDelta));

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

_int CEffect_BGT_RKM_FloorFire::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	if (m_EffectDesc.bGlow)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT_SECOND_GLOW, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT_SECOND, this);
	
	return 0;
}

HRESULT CEffect_BGT_RKM_FloorFire::Render()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix WorldMatrix;

	//	부모가 있을경우, 부모의 빌보드 행렬을 바인드한다 
	if (m_EffectDesc.m_pParent != nullptr)
	{
		WorldMatrix = m_EffectDesc.m_pParent->Get_BillBoardMatrix();
	}
	else
	{
		WorldMatrix = XMLoadFloat4x4(&m_EffectDesc.matBillboard);
	}

	_float4x4 BillboardMatrix;

	XMStoreFloat4x4(&BillboardMatrix,
		XMMatrixRotationRollPitchYaw(XMConvertToRadians(25.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f)) *
		XMMatrixTranslation(0.2f, -0.05f, 0.f)
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

	vSeqUV.x = ((int)(m_fTime * m_EffectDesc.iTexFrameMax) % m_EffectDesc.iTexNumY) / (_float)m_EffectDesc.iTexNumX;
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

HRESULT CEffect_BGT_RKM_FloorFire::CreateEffect_Ember(_double TimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		
	m_fTimer += TimeDelta;

	NONE_ANIM_EFFECT_DESC	NoneAnimEffectDesc;
	ZeroMemory(&NoneAnimEffectDesc, sizeof(NONE_ANIM_EFFECT_DESC));

	/*if (m_iCount == 0 && m_fTimer >= 0.15f)
	{
		++m_iCount;

		NoneAnimEffectDesc = EFFECT_BGT_RKM_EMBER;
		XMStoreFloat4x4(&NoneAnimEffectDesc.EffectDesc.matBillboard, static_cast<CActor*>(m_EffectDesc.m_pParent)->Get_BillBoardMatrix());
		NoneAnimEffectDesc.EffectDesc.m_pParent = static_cast<CActor*>(m_EffectDesc.m_pParent);
		if (FAILED(pGameInstance->Add_GameObject(
			m_iCurLevel,
			g_pLayerTags[DYNAMIC_LAYER_EFFECT],
			TEXT("Prototype_GameObject_Effect_BGT_RKM_Ember"),
			&NoneAnimEffectDesc)))
			__debugbreak();
	}
	if (m_iCount == 1 && m_fTimer >= 0.2f)
	{
		++m_iCount;

		NoneAnimEffectDesc = EFFECT_BGT_RKM_EMBER;
		XMStoreFloat4x4(&NoneAnimEffectDesc.EffectDesc.matBillboard, static_cast<CActor*>(m_EffectDesc.m_pParent)->Get_BillBoardMatrix());
		NoneAnimEffectDesc.EffectDesc.m_pParent = static_cast<CActor*>(m_EffectDesc.m_pParent);
		if (FAILED(pGameInstance->Add_GameObject(
			m_iCurLevel,
			g_pLayerTags[DYNAMIC_LAYER_EFFECT],
			TEXT("Prototype_GameObject_Effect_BGT_RKM_Ember"),
			&NoneAnimEffectDesc)))
			__debugbreak();
	}
	if (m_iCount == 2 && m_fTimer >= 0.25f)
	{
		++m_iCount;

		NoneAnimEffectDesc = EFFECT_BGT_RKM_EMBER;
		XMStoreFloat4x4(&NoneAnimEffectDesc.EffectDesc.matBillboard, static_cast<CActor*>(m_EffectDesc.m_pParent)->Get_BillBoardMatrix());
		NoneAnimEffectDesc.EffectDesc.m_pParent = static_cast<CActor*>(m_EffectDesc.m_pParent);
		if (FAILED(pGameInstance->Add_GameObject(
			m_iCurLevel,
			g_pLayerTags[DYNAMIC_LAYER_EFFECT],
			TEXT("Prototype_GameObject_Effect_BGT_RKM_Ember"),
			&NoneAnimEffectDesc)))
			__debugbreak();
	}
	if (m_iCount == 3 && m_fTimer >= 0.3f)
	{
		++m_iCount;

		NoneAnimEffectDesc = EFFECT_BGT_RKM_EMBER;
		XMStoreFloat4x4(&NoneAnimEffectDesc.EffectDesc.matBillboard, static_cast<CActor*>(m_EffectDesc.m_pParent)->Get_BillBoardMatrix());
		NoneAnimEffectDesc.EffectDesc.m_pParent = static_cast<CActor*>(m_EffectDesc.m_pParent);
		if (FAILED(pGameInstance->Add_GameObject(
			m_iCurLevel,
			g_pLayerTags[DYNAMIC_LAYER_EFFECT],
			TEXT("Prototype_GameObject_Effect_BGT_RKM_Ember"),
			&NoneAnimEffectDesc)))
			__debugbreak();
	}*/

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CEffect_BGT_RKM_FloorFire* CEffect_BGT_RKM_FloorFire::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CEffect_BGT_RKM_FloorFire* pInstance = new CEffect_BGT_RKM_FloorFire(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CEffect_BGT_RKM_FloorFire");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEffect_BGT_RKM_FloorFire::Clone(void* pArg)
{
	CEffect_BGT_RKM_FloorFire* pInstance = new CEffect_BGT_RKM_FloorFire(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CEffect_BGT_RKM_FloorFire");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_BGT_RKM_FloorFire::Free()
{
	__super::Free();
}
