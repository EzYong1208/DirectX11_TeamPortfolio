#include "stdafx.h"
#include "Effect_IronMaidon.h"
#include "GameInstance.h"

CEffect_IronMaidon::CEffect_IronMaidon(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CEffect_Anim_Effect(pDevice, pDeviceContext)
{
}

CEffect_IronMaidon::CEffect_IronMaidon(const CEffect_IronMaidon& rhs)
	: CEffect_Anim_Effect(rhs)
{
}

HRESULT CEffect_IronMaidon::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_IronMaidon::NativeConstruct(void* pArg)
{
	// 트랜스폼, 모델, 셰이더, 랜더러 추가 
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	XMStoreFloat3(&m_EffectDesc.vScaleOrigin, m_pTransform->Get_Scale());

	m_pModelCom->SetUp_AnimIndex(INDEX_00);
	m_pModelCom->Set_Anim_ExtarSpeed(INDEX_00, 3.f);

	return S_OK;
}

_int CEffect_IronMaidon::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	m_pModelCom->Update_CombinedTransformationMatrices(TimeDelta);

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

	temp = FrameLock();

	m_pTransform->Reset_Momentum();

	return temp;
}

_int CEffect_IronMaidon::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_SECOND, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_OUTLINE, this);

	return 0;
}

HRESULT CEffect_IronMaidon::Render()
{
	/*if (FAILED(SetUp_ConstantTable_BillBoarding()))
		return E_FAIL;*/

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
		XMMatrixScaling(1.3f, 1.3f, 1.3f) *
		XMMatrixRotationRollPitchYaw(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f)) *
		XMMatrixTranslation(0.f, 0.f, 0.f)
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

	_uint iNumMaterials = m_pModelCom->Get_NumMaterials();

	CCombat* pParentCombat = static_cast<CCombat*>(m_EffectDesc.m_pParent->Get_Component(TEXT("Com_Combat")));
	_uint iPass = 0;
	if (pParentCombat->Get_CombatDesc().eActorSide == CCombat::ACTOR_SIDE_LEFT)
		iPass = 0;
	else
		iPass = 3;

	for (_uint i = 0; iNumMaterials > i; ++i)
	{
		m_pModelCom->Bind_OnShader(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture");
		m_pModelCom->Bind_OnShader(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture");
		m_pModelCom->Render(m_pShaderCom, iPass, i, "g_BoneMatrices");
	}

	return S_OK;
}

HRESULT CEffect_IronMaidon::Render_OutLine()
{
	/*if (FAILED(SetUp_ConstantTable_BillBoarding()))
		return E_FAIL;*/

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
		XMMatrixScaling(1.3f, 1.3f, 1.3f) *
		XMMatrixRotationRollPitchYaw(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f)) *
		XMMatrixTranslation(0.f, 0.f, 0.f)
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

	_uint iNumMaterials = m_pModelCom->Get_NumMaterials();

	CCombat* pParentCombat = static_cast<CCombat*>(m_EffectDesc.m_pParent->Get_Component(TEXT("Com_Combat")));
	_uint iPass = 0;
	if (pParentCombat->Get_CombatDesc().eActorSide == CCombat::ACTOR_SIDE_LEFT)
		iPass = 2;
	else
		iPass = 4;

	for (_uint i = 0; iNumMaterials > i; ++i)
	{
		m_pModelCom->Bind_OnShader(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture");
		m_pModelCom->Bind_OnShader(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture");
		m_pModelCom->Render(m_pShaderCom, iPass, i, "g_BoneMatrices");
	}

	return S_OK;
}

_uint CEffect_IronMaidon::FrameLock()
{
	_double		fCurAcc = 0.f;
	fCurAcc = m_pModelCom->Get_PlayAcc();
	
	CTransform* pParentTransform = static_cast<CTransform*>(m_EffectDesc.m_pParent->Get_Component(TEXT("Com_Transform")));
	_uint		iResult = 0;

	_float	temp = 2.f;
	if (m_iFrameLockCount == 0 && fCurAcc >= 5.f)
	{
		++m_iFrameLockCount;
		m_pModelCom->Activate_FrameLock(0.f, 5.f);
		m_pModelCom->Set_Anim_TimeAcc(INDEX_00, 5.f - temp);
	}
	if (m_iFrameLockCount == 1 && fCurAcc >= 5.f)
	{
		++m_iFrameLockCount;
		m_pModelCom->Activate_FrameLock(5.f, 10.f);
		m_pModelCom->Set_Anim_TimeAcc(INDEX_00, 10.f - 4);
	}
	if (m_iFrameLockCount == 2 && fCurAcc >= 10.f)
	{
		++m_iFrameLockCount;
		m_pModelCom->Activate_FrameLock(10.f, 15.f);
		m_pModelCom->Set_Anim_TimeAcc(INDEX_00, 15.f - 6);
	}
	if (m_iFrameLockCount == 3 && fCurAcc >= 15.f)
	{
		++m_iFrameLockCount;
		m_pModelCom->Activate_FrameLock(15.f, 20.f);
		m_pModelCom->Set_Anim_TimeAcc(INDEX_00, 20.f - 4);
	}
	if (m_iFrameLockCount == 4 && fCurAcc >= 20.f)
	{
		++m_iFrameLockCount;
		m_pModelCom->Activate_FrameLock(20.f, 25.f);
		m_pModelCom->Set_Anim_TimeAcc(INDEX_00, 25.f - 4);
	}
	if (m_iFrameLockCount == 5 && fCurAcc >= 25.f)
	{
		++m_iFrameLockCount;
		m_pModelCom->Activate_FrameLock(25.f, 30.f);
		m_pModelCom->Set_Anim_TimeAcc(INDEX_00, 30.f - 4);
	}
	if (m_iFrameLockCount == 6 && fCurAcc >= 30.f)
	{
		++m_iFrameLockCount;
		m_pModelCom->Activate_FrameLock(30.f, 35.f);
		m_pModelCom->Set_Anim_TimeAcc(INDEX_00, 35.f - 4);
	}
	if (m_iFrameLockCount == 7 && fCurAcc >= 35.f)
	{
		++m_iFrameLockCount;
		m_pModelCom->Activate_FrameLock(35.f, 40.f);
		m_pModelCom->Set_Anim_TimeAcc(INDEX_00, 40.f - temp);
	}
	if (m_iFrameLockCount == 8 && fCurAcc >= 40.f)
	{
		++m_iFrameLockCount;
		m_pModelCom->Activate_FrameLock(40.f, 45.f);
		m_pModelCom->Set_Anim_TimeAcc(INDEX_00, 45.f - 4);
	}
	if (m_iFrameLockCount == 9 && fCurAcc >= 45.f)
	{
		++m_iFrameLockCount;
		m_pModelCom->Activate_FrameLock(45.f, 50.f);
		m_pModelCom->Set_Anim_TimeAcc(INDEX_00, 50.f - 4);
	}
	if (m_iFrameLockCount == 10 && fCurAcc >= 50.f)
	{
		++m_iFrameLockCount;
		m_pModelCom->Activate_FrameLock(50.f, 55.f);
		m_pModelCom->Set_Anim_TimeAcc(INDEX_00, 55.f - 4);
	}
	if (m_iFrameLockCount == 11 && fCurAcc >= 55.f)
	{
		++m_iFrameLockCount;
		m_pModelCom->Activate_FrameLock(55.f, 60.f);
		m_pModelCom->Set_Anim_TimeAcc(INDEX_00, 60.f - 4);
	}
	if (m_iFrameLockCount == 12 && fCurAcc >= 60.f)
	{
		++m_iFrameLockCount;
		m_pModelCom->Activate_FrameLock(60.f, 65.f);
		m_pModelCom->Set_Anim_TimeAcc(INDEX_00, 65.f - temp);
	}
	if (m_iFrameLockCount == 13 && fCurAcc >= 65.f)
	{
		++m_iFrameLockCount;
		m_pModelCom->Activate_FrameLock(65.f, 70.f);
		m_pModelCom->Set_Anim_TimeAcc(INDEX_00, 70.f - temp);
	}
	if (m_iFrameLockCount == 14 && fCurAcc >= 70.f)
	{
		++m_iFrameLockCount;
		m_pModelCom->Activate_FrameLock(70.f, 75.f);
		m_pModelCom->Set_Anim_TimeAcc(INDEX_00, 75.f - temp);
	}
	if (m_iFrameLockCount == 15 && fCurAcc >= 75.f)
	{
		++m_iFrameLockCount;
		m_pModelCom->Activate_FrameLock(75.f, 80.f);
		m_pModelCom->Set_Anim_TimeAcc(INDEX_00, 80.f - temp);
	}
	if (m_iFrameLockCount == 16 && fCurAcc >= 80.f)
	{
		++m_iFrameLockCount;
		m_pModelCom->Activate_FrameLock(80.f, 85.f);
		m_pModelCom->Set_Anim_TimeAcc(INDEX_00, 85.f - temp);
	}
	if (m_iFrameLockCount == 17 && fCurAcc >= 85.f)
	{
		++m_iFrameLockCount;
		m_pModelCom->Activate_FrameLock(85.f, 90.f);
		m_pModelCom->Set_Anim_TimeAcc(INDEX_00, 90.f - temp);
	}
	if (m_iFrameLockCount == 18 && fCurAcc >= 90.f)
	{
		++m_iFrameLockCount;
		m_pModelCom->Activate_FrameLock(90.f, 95.f);
		m_pModelCom->Set_Anim_TimeAcc(INDEX_00, 95.f - temp);
	}
	if (m_iFrameLockCount == 19 && fCurAcc >= 95.f)
	{
		++m_iFrameLockCount;
		m_pModelCom->Activate_FrameLock(95.f, 100.f);
		m_pModelCom->Set_Anim_TimeAcc(INDEX_00, 100.f - temp);
	}
	if (m_iFrameLockCount == 20 && fCurAcc >= 100.f)
	{
		++m_iFrameLockCount;
		m_pModelCom->Activate_FrameLock(100.f, 105.f);
		m_pModelCom->Set_Anim_TimeAcc(INDEX_00, 105.f - temp);
	}
	if (m_iFrameLockCount == 21 && fCurAcc >= 105.f)
	{
		++m_iFrameLockCount;
		m_pModelCom->Activate_FrameLock(105.f, 110.f);
		m_pModelCom->Set_Anim_TimeAcc(INDEX_00, 110.f - temp);
	}
	if (m_iFrameLockCount == 22 && fCurAcc >= 110.f)
	{
		++m_iFrameLockCount;
		m_pModelCom->Activate_FrameLock(110.f, 115.f);
		m_pModelCom->Set_Anim_TimeAcc(INDEX_00, 115.f - temp);
	}
	if (m_iFrameLockCount == 23 && fCurAcc >= 115.f)
	{
		++m_iFrameLockCount;
		m_pModelCom->Activate_FrameLock(115.f, 120.f);
		m_pModelCom->Set_Anim_TimeAcc(INDEX_00, 120.f - temp);
	}
	if (m_iFrameLockCount == 24 && fCurAcc >= 120.f)
	{
		++m_iFrameLockCount;
		m_pModelCom->Activate_FrameLock(120.f, 125.f);
		m_pModelCom->Set_Anim_TimeAcc(INDEX_00, 125.f - temp);
	}
	if (m_iFrameLockCount == 25 && fCurAcc >= 125.f)
	{
		++m_iFrameLockCount;
		m_pModelCom->Activate_FrameLock(125.f, 130.f);
		m_pModelCom->Set_Anim_TimeAcc(INDEX_00, 130.f - temp);
	}
	if (m_iFrameLockCount == 26 && fCurAcc >= 130.f)
	{
		++m_iFrameLockCount;
		m_pModelCom->Activate_FrameLock(130.f, 135.f);
		m_pModelCom->Set_Anim_TimeAcc(INDEX_00, 135.f - temp);
	}
	if (m_iFrameLockCount == 27 && fCurAcc >= 135.f)
	{
		++m_iFrameLockCount;
		m_pModelCom->Activate_FrameLock(135.f, 140.f);
		m_pModelCom->Set_Anim_TimeAcc(INDEX_00, 140.f - temp);
	}
	if (m_iFrameLockCount == 28 && fCurAcc >= 140.f)
	{
		++m_iFrameLockCount;
		m_pModelCom->Activate_FrameLock(140.f, 145.f);
		m_pModelCom->Set_Anim_TimeAcc(INDEX_00, 145.f - temp);
	}
	if (m_iFrameLockCount == 29 && fCurAcc >= 145.f)
	{
		++m_iFrameLockCount;
		m_pModelCom->Activate_FrameLock(145.f, 150.f);
		m_pModelCom->Set_Anim_TimeAcc(INDEX_00, 150.f - temp);
	}
	if (m_iFrameLockCount == 30 && fCurAcc >= 150.f)
	{
		++m_iFrameLockCount;
		m_pModelCom->Activate_FrameLock(150.f, 155.f);
		m_pModelCom->Set_Anim_TimeAcc(INDEX_00, 155.f - temp);
	}
	if (m_iFrameLockCount == 31 && fCurAcc >= 155.f)
	{
		++m_iFrameLockCount;
		m_pModelCom->Activate_FrameLock(155.f, 160.f);
		m_pModelCom->Set_Anim_TimeAcc(INDEX_00, 160.f - temp);
	}
	if (m_iFrameLockCount == 32 && fCurAcc >= 160.f)
	{
		++m_iFrameLockCount;
		m_pModelCom->Activate_FrameLock(160.f, 165.f);
		m_pModelCom->Set_Anim_TimeAcc(INDEX_00, 165.f - temp);
		m_pModelCom->Set_Anim_ExtarSpeed(INDEX_00, 0.f);
	}
	if (m_iFrameLockCount == 33 && pParentTransform->GetTransfromDesc().ExtraHeight <= 0)
	{
		m_iFrameLockCount = 35;
		m_pModelCom->Set_Anim_TimeAcc(INDEX_00, 175.f);
		m_pModelCom->Set_Anim_ExtarSpeed(INDEX_00, 3.5f);
	}
	if (m_iFrameLockCount == 35 && fCurAcc >= 175.f)
	{
		++m_iFrameLockCount;
		m_pModelCom->Activate_FrameLock(175.f, 180.f);
		m_pModelCom->Set_Anim_TimeAcc(INDEX_00, 180.f - 20);
	}
	if (m_iFrameLockCount == 36 && fCurAcc >= 180.f)
	{
		++m_iFrameLockCount;
		m_pModelCom->Activate_FrameLock(180.f, 185.f);
		m_pModelCom->Set_Anim_TimeAcc(INDEX_00, 185.f - 4);
	}
	if (m_iFrameLockCount == 37 && fCurAcc >= 185.f)
	{
		++m_iFrameLockCount;
		iResult = ISDEAD;
	}

	return iResult;
}

CEffect_IronMaidon* CEffect_IronMaidon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CEffect_IronMaidon* pInstance = new CEffect_IronMaidon(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CEffect_IronMaidon");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEffect_IronMaidon::Clone(void* pArg)
{
	CEffect_IronMaidon* pInstance = new CEffect_IronMaidon(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CEffect_IronMaidon");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_IronMaidon::Free()
{
	__super::Free();
}
