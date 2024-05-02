#include "stdafx.h"
#include "Deco_Bird.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "Camera_Dynamic.h"

CDeco_Bird::CDeco_Bird(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CDeco_Bird::CDeco_Bird(const CDeco_Bird& rhs)
	: CGameObject(rhs)
{
}

HRESULT CDeco_Bird::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDeco_Bird::NativeConstruct(void* pArg)
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
		MSGBOX("CDeco_Bird Error : CurLevel is invailed");
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

	m_vRightScale = m_pTransform->Get_Scale(CTransform::STATE_RIGHT);
	m_vUpScale = m_pTransform->Get_Scale(CTransform::STATE_UP);
	m_vLookScale = m_pTransform->Get_Scale(CTransform::STATE_LOOK);

	m_vStartPos = XMVectorSet(145.f, -5.f, 55.f, 1.f);
	//m_vStartPos = XMVectorSet(1.f, 1.f, 50.f, 1.f);
	m_pTransform->Set_State(CTransform::STATE_POSITION, m_vStartPos);
	SetEndPos();

	return S_OK;
}

_int CDeco_Bird::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	m_fFrame += 12.0f * TimeDelta;

	if (m_fFrame >= 12.0f)
		m_fFrame = 0.f;

	Looking();
	Movement(TimeDelta);

	if (m_bEnd)
		return ISDEAD;

	return _int();
}

_int CDeco_Bird::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	//__super::Compute_CamDistance(m_pTransform);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BLEND, this);

	return _int();
}

HRESULT CDeco_Bird::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDeco_Bird::SetUp_Components()
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
		g_pShaderTag[Shader_VTXTEX],
		TEXT("Com_Shader"),
		(CComponent**)&m_pShaderCom)))
		return E_FAIL;

	//	For.Com_VIBuffer
	if (FAILED(__super::Add_Component(
		LEVEL_STATIC,
		TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"),
		(CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	//	For.Com_Texture
	if (FAILED(__super::Add_Component(
		m_iCurLevel,
		TEXT("Prototype_Component_Texture_Deco_Bird"),
		TEXT("Com_Texture"),
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CDeco_Bird::SetUp_ConstantTable()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_pTransform->Bind_OnShader(m_pShaderCom, "g_WorldMatrix");

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pTextureCom->SetUp_OnShader(m_pShaderCom, "g_DiffuseTexture", (_uint)m_fFrame)))
		return E_FAIL;

	m_pVIBufferCom->Render(m_pShaderCom, 0);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CDeco_Bird::SetEndPos()
{
	_float	fRandX = rand() % 3 * 10.f;
	_float	fRandY = rand() % 3 * 10.f;
	_float	fRandZ = rand() % 3 * 10.f;

	m_fRandX = rand() % 3 * 10.f + 50.f + fRandX;
	m_fRandY = rand() % 3 * 10.f + 20.f + fRandY;
	m_fRandZ = rand() % 3 * 10.f + fRandZ;

	_vector		BirdX = XMVectorSetX(m_vStartPos, XMVectorGetX(m_vStartPos) + m_fRandX);
	_vector		BirdY = XMVectorSetY(BirdX, XMVectorGetY(BirdX) + m_fRandY);
	m_vEndPos = XMVectorSetZ(BirdY, XMVectorGetZ(BirdY) + m_fRandZ);
}

void CDeco_Bird::Looking()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CCamera_Dynamic* pCamera = (CCamera_Dynamic*)pGameInstance->Get_GameObject(m_iCurLevel, L"Layer_Camera", 0);
	CTransform* pCamerTransform = dynamic_cast<CTransform*>(pCamera->Get_Component(TEXT("Com_Transform")));

	_vector		vRight = pCamerTransform->Get_State(CTransform::STATE_RIGHT);
	_vector		vUp = pCamerTransform->Get_State(CTransform::STATE_UP);
	_vector		vLook = pCamerTransform->Get_State(CTransform::STATE_LOOK);

	vRight = vRight * m_vRightScale;
	vUp = vUp * m_vUpScale;
	vLook = vLook * m_vLookScale;

	m_pTransform->Set_State(CTransform::STATE_RIGHT, vRight);
	m_pTransform->Set_State(CTransform::STATE_UP, vUp);
	m_pTransform->Set_State(CTransform::STATE_LOOK, vLook);

	RELEASE_INSTANCE(CGameInstance);
}

void CDeco_Bird::Movement(_double TimeDelta)
{
	_vector		vCurPos = m_pTransform->Get_State(CTransform::STATE_POSITION);

	vCurPos = XMVectorLerp(vCurPos, m_vEndPos, TimeDelta * 0.2f);
	m_pTransform->Set_State(CTransform::STATE_POSITION, vCurPos);

	if (XMVectorGetX(m_vEndPos) - 10.f <= XMVectorGetX(vCurPos) ||
		XMVectorGetY(m_vEndPos) - 5.f <= XMVectorGetY(vCurPos))
		m_bEnd = true;
}

CDeco_Bird* CDeco_Bird::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CDeco_Bird* pInstance = new CDeco_Bird(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CDeco_Bird");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CDeco_Bird::Clone(void* pArg)
{
	CDeco_Bird* pInstance = new CDeco_Bird(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CDeco_Bird");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CDeco_Bird::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
