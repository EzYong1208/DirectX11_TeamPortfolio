#include "stdafx.h"
#include "Effect_Drop01_05.h"
#include "GameInstance.h"

CEffect_Drop01_05::CEffect_Drop01_05(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CEffect_None_Anim_Effect(pDevice, pDeviceContext)
{
}

CEffect_Drop01_05::CEffect_Drop01_05(const CEffect_Drop01_05& rhs)
	: CEffect_None_Anim_Effect(rhs)
{
}

HRESULT CEffect_Drop01_05::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Drop01_05::NativeConstruct(void* pArg)
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

_int CEffect_Drop01_05::Tick(_double TimeDelta)
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

_int CEffect_Drop01_05::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	//�������� �߰�
	//if (m_EffectDesc.bGlow)
	//	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT_FIRST_GLOW, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT_SECOND, this);

	return 0;
}

HRESULT CEffect_Drop01_05::Render()
{
	//if (nullptr == m_pModelCom)
	//	return E_FAIL;

	//CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//_matrix WorldMatrix;

	//WorldMatrix = XMLoadFloat4x4(&m_EffectDesc.matBillboard);

	//_float4x4 BillboardMatrix;

	//XMStoreFloat4x4(&BillboardMatrix,
	//	XMMatrixRotationRollPitchYaw(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f + (m_fTimeDelta * 170))) *
	//	XMMatrixTranslation(0.f, 0.f, 0.f)
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

void CEffect_Drop01_05::Trigger_Activate(const _double& _fTimeDelta)
{
}

HRESULT CEffect_Drop01_05::SetUp_Components()
{
	return S_OK;
}

CEffect_Drop01_05* CEffect_Drop01_05::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CEffect_Drop01_05* pInstance = new CEffect_Drop01_05(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CEffect_Drop01_05");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEffect_Drop01_05::Clone(void* pArg)
{
	CEffect_Drop01_05* pInstance = new CEffect_Drop01_05(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CEffect_Drop01_05");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_Drop01_05::Free()
{
	__super::Free();
}
