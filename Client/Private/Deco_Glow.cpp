#include "stdafx.h"
#include "Deco_Glow.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "Camera_Dynamic.h"

CDeco_Glow::CDeco_Glow(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CDeco_Glow::CDeco_Glow(const CDeco_Glow& rhs)
	: CGameObject(rhs)
{
}

HRESULT CDeco_Glow::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDeco_Glow::NativeConstruct(void* pArg)
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
		MSGBOX("CDeco_Glow Error : CurLevel is invailed");
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

	if (nullptr != pArg)
		memcpy(&m_GlowDesc, pArg, sizeof(GLOWDESC));

	_vector		Scale = XMVectorSet(0.15f, 0.15f, 0.15f, 1.f);
	m_pTransform->Scaled(Scale);

	m_vRightScale = m_pTransform->Get_Scale(CTransform::STATE_RIGHT);
	m_vUpScale = m_pTransform->Get_Scale(CTransform::STATE_UP);
	m_vLookScale = m_pTransform->Get_Scale(CTransform::STATE_LOOK);

	SetStartPos();
	SetEndPos();

	return S_OK;
}

_int CDeco_Glow::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	Looking();
	Scaling(TimeDelta);
	Movement(TimeDelta);

	return _int();
}

_int CDeco_Glow::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BLEND, this);

	return _int();
}

HRESULT CDeco_Glow::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDeco_Glow::SetUp_Components()
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
		TEXT("Prototype_Component_Texture_Deco_Glow"),
		TEXT("Com_Texture"),
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CDeco_Glow::SetUp_ConstantTable()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_pTransform->Bind_OnShader(m_pShaderCom, "g_WorldMatrix");

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pTextureCom->SetUp_OnShader(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	m_pVIBufferCom->Render(m_pShaderCom, 0);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CDeco_Glow::SetStartPos()
{
	m_fRandTimeAcc = rand() % 5 + 1;

	_float	fRandValueX = rand() % 2;
	_float	fRandValueY = rand() % 2;
	_float	fRandValueZ = rand() % 2;

	if (0.f == fRandValueX)
		fRandValueX = -1.f;
	if (0.f == fRandValueY)
		fRandValueY = -1.f;
	if (0.f == fRandValueZ)
		fRandValueZ = -1.f;

	_float	fRandX = (rand() % 3 + fRandValueX) * 1.5f;
	_float	fRandY = (rand() % 3 + fRandValueY);
	_float	fRandZ = (rand() % 3 + fRandValueZ) * 1.5f;

	if (AREA_UP == m_GlowDesc.eGlowArea)
		m_vStartPos = XMVectorSet(159.f + fRandX, 0.f + fRandY, 37.f + fRandZ, 1.f);
	else if (AREA_DOWN == m_GlowDesc.eGlowArea)
		m_vStartPos = XMVectorSet(154.f + fRandX, -7.5f + fRandY, 66.f + fRandZ, 1.f);

	m_pTransform->Set_State(CTransform::STATE_POSITION, m_vStartPos);
}

void CDeco_Glow::SetEndPos()
{
	switch (m_GlowDesc.eGlowMoveDir)
	{
	case DIR_UP:
		m_fValue = 1.f;
		break;

	case DIR_DOWN:
		m_fValue = -1.f;
		break;
	}

	_float	fRandX = rand() % 3 * 1.5f;
	_float	fRandZ = rand() % 3 * 1.5f;
	_float	fRandValueX = rand() % 2;
	_float	fRandValueZ = rand() % 2;

	if (0.f == fRandValueX)
		fRandValueX = -1.f;
	if (0.f == fRandValueZ)
		fRandValueZ = -1.f;

	m_fRandX = (rand() % 3 + fRandX) * m_fValue * fRandValueX;
	m_fRandY = 0.5f * m_fValue;
	m_fRandZ = (rand() % 3 + fRandZ) * m_fValue * fRandValueZ;

	_vector		GlowX = XMVectorSetX(m_vStartPos, XMVectorGetX(m_vStartPos) + m_fRandX);
	_vector		GlowY = XMVectorSetY(GlowX, XMVectorGetY(GlowX) + m_fRandY);
	m_vEndPos = XMVectorSetZ(GlowY, XMVectorGetZ(GlowY) + m_fRandZ);
}

void CDeco_Glow::Looking()
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

void CDeco_Glow::Scaling(_double TimeDelta)
{
	if (m_bGetBig)
	{
		if (0.1f <= m_fSize)
		{
			m_bGetBig = false;
			m_bGetSmall = true;
		}

		m_fSize += TimeDelta * 0.01f * m_fRandTimeAcc;
	}

	if (m_bGetSmall)
	{
		if (0.f >= m_fSize)
		{
			m_bGetSmall = false;
			m_bGetBig = true;
		}

		m_fSize -= TimeDelta * 0.01f * m_fRandTimeAcc;
	}

	_vector		Scale = XMVectorSet(m_fSize, m_fSize, m_fSize, 1.f);
	m_pTransform->Scaled(Scale);
}

void CDeco_Glow::Movement(_double TimeDelta)
{
	if (false == m_bEndPos)
	{
		_vector		vCurPos = m_pTransform->Get_State(CTransform::STATE_POSITION);

		vCurPos = XMVectorLerp(vCurPos, m_vEndPos, TimeDelta * 0.01f);
		m_pTransform->Set_State(CTransform::STATE_POSITION, vCurPos);

		switch (m_GlowDesc.eGlowMoveDir)
		{
		case DIR_UP:
			if (XMVectorGetY(m_vEndPos) - 0.3f <= XMVectorGetY(vCurPos))
				m_bEndPos = true;
			break;

		case DIR_DOWN:
			if (XMVectorGetY(m_vEndPos) + 0.3f >= XMVectorGetY(vCurPos))
				m_bEndPos = true;
			break;
		}
	}
	else
	{
		m_pTransform->Set_State(CTransform::STATE_POSITION, m_vStartPos);
		m_bEndPos = false;
	}
}

CDeco_Glow* CDeco_Glow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CDeco_Glow* pInstance = new CDeco_Glow(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CDeco_Glow");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CDeco_Glow::Clone(void* pArg)
{
	CDeco_Glow* pInstance = new CDeco_Glow(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CDeco_Glow");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CDeco_Glow::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
