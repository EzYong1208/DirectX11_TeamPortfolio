#include "stdafx.h"
#include "Deco_Snow.h"
#include "GameInstance.h"
#include "Level_Loading.h"

CDeco_Snow::CDeco_Snow(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CDeco_Snow::CDeco_Snow(const CDeco_Snow& rhs)
	: CGameObject(rhs)
{
}

HRESULT CDeco_Snow::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDeco_Snow::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(nullptr)))
		return E_FAIL;

	//1. 게임인스턴스 준비
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	_uint iCurLevel = pGameInstance->Get_CurrentLevelIndex();

	//2. 예외처리
	if (iCurLevel == LEVEL_LOGO ||
		iCurLevel == LEVEL_MAIN_MENU)
	{
		MSGBOX("CDeco_Snow Error : CurLevel is invailed");
		return E_FAIL;
	}

	//3. 로딩 레벨의 경우
	if (iCurLevel == LEVEL_LOADING)
	{
		iCurLevel = dynamic_cast<CLevel_Loading*>(pGameInstance->Get_LoadingLevel())->Get_NextLevel();
	}
	RELEASE_INSTANCE(CGameInstance);

	m_iCurLevel = iCurLevel;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	return S_OK;
}

_int CDeco_Snow::Tick(_double TimeDelta)
{
	if (m_bFirst)
	{
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		CLevel* pCurLevel = pGameInstance->Get_CurLevel();
		_uint	iCurLevel = pGameInstance->Get_CurrentLevelIndex();
		MAP_TYPE	eCurMapType = static_cast<CBattle_Level*>(pCurLevel)->Get_CurMapType();

		if (MAP_TYPE_SNOW == eCurMapType || LEVEL_SNOW == iCurLevel)
		{
			m_fMaxX = -10.f;
			m_fSetX = 50.f;
			m_fMaxY = -20.f;
			m_fSetY = 50.f;

			m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(80.f, 30.f, 37.f, 1.f));
		}
		////	던파맵 생기면 그 렙으로 맞추고 스노우맵처럼 게임오브젝트 추가하기
		//if (LEVEL_DNF_SNOWMOUNTAIN == iCurLevel ||
		//	LEVEL_PJE == iCurLevel)
		//{
		//	m_fMaxX = -60.f;
		//	m_fSetX = 100.f;
		//	m_fMaxY = -40.f;
		//	m_fSetY = 50.f;

		//	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(50.f, 30.f, 5.f, 1.f));
		//}
		RELEASE_INSTANCE(CGameInstance);

		m_bFirst = false;
	}

	if (0 > __super::Tick(TimeDelta))
		return -1;

	return _int();
}

_int CDeco_Snow::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	CVIBuffer_Point_Instance::PARTICLEVALUE		ParticleValue;
	ParticleValue.fMaxX = m_fMaxX;
	ParticleValue.fSetX = m_fSetX;
	ParticleValue.fMaxY = m_fMaxY;
	ParticleValue.fSetY = m_fSetY;

	m_pVIBufferCom->UpdateByValue(TimeDelta, ParticleValue);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BLEND, this);

	return _int();
}

HRESULT CDeco_Snow::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render(m_pShaderCom, 0)))
		return E_FAIL;

	return S_OK;
}

HRESULT CDeco_Snow::SetUp_Components()
{
	//	For.Com_Renderer
	if (FAILED(__super::Add_Component(
		LEVEL_STATIC,
		TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"),
		(CComponent**)&m_pRendererCom)))
		return E_FAIL;

	//	For.Com_Shader
	if (FAILED(__super::Add_Component(
		LEVEL_STATIC,
		g_pShaderTag[Shader_PointInstance],
		TEXT("Com_Shader"),
		(CComponent**)&m_pShaderCom)))
		return E_FAIL;

	//	For.Com_Texture
	if (FAILED(__super::Add_Component(
		m_iCurLevel,
		TEXT("Prototype_Component_Texture_Deco_Glow"),
		TEXT("Com_Texture"),
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;

	//	For.Com_VIBuffer
	CVIBuffer_Point_Instance::PARTICLEDESC		ParticleDesc;

	ParticleDesc.fMinSpeed = 20.f;
	ParticleDesc.fMaxSpeed = 45.f;
	ParticleDesc.vMoveDir = _float3(-1.f, -1.f, 0.f);

	if (FAILED(__super::Add_Component(
		m_iCurLevel,
		TEXT("Prototype_Component_VIBuffer_Point_Instance"),
		TEXT("Com_VIBuffer"),
		(CComponent**)&m_pVIBufferCom,
		&ParticleDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CDeco_Snow::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransform->Bind_OnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue(
		"g_ViewMatrix",
		&pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW),
		sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue(
		"g_ProjMatrix",
		&pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ),
		sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->SetUp_OnShader(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;

	//	카메라 위치 던짐
	_float4		vCamPosition;
	XMStoreFloat4(&vCamPosition, pGameInstance->Get_CamPosition());

	if (FAILED(m_pShaderCom->Set_RawValue(
		"g_vCamPosition",
		&vCamPosition,
		sizeof(_float4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CDeco_Snow* CDeco_Snow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CDeco_Snow* pInstance = new CDeco_Snow(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CDeco_Snow");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CDeco_Snow::Clone(void* pArg)
{
	CDeco_Snow* pInstance = new CDeco_Snow(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CDeco_Snow");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CDeco_Snow::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
