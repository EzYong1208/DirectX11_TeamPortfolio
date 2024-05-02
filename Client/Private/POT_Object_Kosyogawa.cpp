#include "stdafx.h"
#include "POT_Object_Kosyogawa.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "Command_Manager.h"
#include "Attack_Mask_Range.h"

CPOT_Object_Kosyogawa::CPOT_Object_Kosyogawa(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CActor(pDevice, pDeviceContext)
{
}

CPOT_Object_Kosyogawa::CPOT_Object_Kosyogawa(const CPOT_Object_Kosyogawa& rhs)
	: CActor(rhs)
{
}

HRESULT CPOT_Object_Kosyogawa::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPOT_Object_Kosyogawa::NativeConstruct(void* pArg)
{

	if (pArg != nullptr)
	{

		POT_KOSYOGAWA_INIT_DESC temp;
		memcpy(&temp, pArg, sizeof(POT_KOSYOGAWA_INIT_DESC));

		//pre_1. 액터 기본 컴포넌트들 준비
		DEBUG_BREAK(__super::NativeConstruct(&temp.tagActorDesc));
		m_pOwner = temp.pOwnner;
	}
	else
	{
		MSGBOX("아쎄이! 이글을 봤다면 구조체를 채워라");
		__debugbreak();
	}
	//pre_2. 서번트 모델 셋업
	DEBUG_BREAK(SetUp_Model());

	DEBUG_BREAK(SetUp_Colliders());

	//트랜스폼 설정(솔직히 별로 안중요함)
	CTransform::TRANSFORMDESC temp;
	ZeroMemory(&temp, sizeof(CTransform::TRANSFORMDESC));
	temp.fSpeedPerSec = 5.f;
	temp.fRotationPerSec = XMConvertToRadians(90.0f);
	temp.fJumpPerSec = 20.f;
	m_pTransform->Set_TransformDesc(temp);
	m_pTransform->Scaled(XMVectorSet(1.f, 1.f, 1.f, 0.f));
	m_pTransform->Set_Ratio(0.45f);
	m_pTransform->Set_CrossVaild(true);

	//첫상태 싱크
	//m_pStateMachine->Bind_State(CState_Machine::STATETYPE_OUTPUT, L"Linked_Owner", CState_Actor_RamW_LINK::Create(0.f));

	//빌보드
	XMStoreFloat3(&m_vExtraScale, ACT_POT_KOSYOGAWA_BILLBOARD_OFFSET_EXTRA_SCALE);
	XMStoreFloat3(&m_vLocalLocation, ACT_POT_KOSYOGAWA_BILLBOARD_OFFSET_LOCATION);
	XMStoreFloat3(&m_vExtraLocation, ACT_POT_KOSYOGAWA_BILLBOARD_OFFSET_EXTRA_LOCATION);
	XMStoreFloat3(&m_vLocalDegree, ACT_POT_KOSYOGAWA_BILLBOARD_OFFSET_ROTATION);
	XMStoreFloat3(&m_vExtraDegree, ACT_POT_KOSYOGAWA_BILLBOARD_OFFSET_EXTRA_ROTATION);

	XMStoreFloat3(&m_vOriginExtraScale, XMLoadFloat3(&m_vExtraScale));
	XMStoreFloat3(&m_vOriginLocalLocation, XMLoadFloat3(&m_vLocalLocation));
	XMStoreFloat3(&m_vOriginExtraLocation, XMLoadFloat3(&m_vExtraLocation));
	XMStoreFloat3(&m_vOriginLocalDegree, XMLoadFloat3(&m_vLocalDegree));
	XMStoreFloat3(&m_vOriginExtraDegree, XMLoadFloat3(&m_vExtraDegree));

	CNavigation_Line* pParentNavi = (CNavigation_Line*)m_pOwner->Get_Component(L"Com_Navigation_Line");
	_int iLineIdx = pParentNavi->Get_LineDesc().iCurrentIndex;
	m_pNavigation_Line->Update_LineIdx(iLineIdx);

	m_pTransform->Scaled(XMVectorSet(1.1f, 1.1f, 1.1f, 0.f));

#if defined(USE_IMGUI)
	//초기값설정

	_vector		vRight = m_pTransform->Get_State(CTransform::STATE_RIGHT);
	_vector		vUp = m_pTransform->Get_State(CTransform::STATE_UP);
	_vector		vLook = m_pTransform->Get_State(CTransform::STATE_LOOK);

	//1. 회전

	//3. 스케일
	m_vScale[0] = XMVectorGetX(m_pTransform->Get_Scale(CTransform::STATE_RIGHT));
	m_vScale[1] = XMVectorGetX(m_pTransform->Get_Scale(CTransform::STATE_UP));
	m_vScale[2] = XMVectorGetX(m_pTransform->Get_Scale(CTransform::STATE_LOOK));
	m_vScale[3] = 0.f;
	m_bShow_Window = true;
	

#endif
	m_tagActorDesc.bIsDnf = true;



	return S_OK;
}

_int CPOT_Object_Kosyogawa::Tick(_double TimeDelta)
{
	if (m_Dead == true)
		return 4;

	//빌보드 매트릭스 업데이트
	m_iLifeTime -= TimeDelta;
	if (m_iLifeTime <= 0) {
		m_Dead = true;
	}

	/*if (m_pTransform->GetTransfromDesc().Ratio >= 0.95 || m_pTransform->GetTransfromDesc().Ratio <= 0.05)
	{
		m_Dead = true;
	}*/



	// 현재 입력된 상태들을 지속합니다.
	if (FAILED(m_pStateMachine->Action(TimeDelta * m_fActionSpeed, this)))
	{
		MSGBOX("Player Failed : Action");
	}

	//// 모델들의 컴바인 행렬들을 업데이트합니다
	//if (FAILED(m_pModelSlotCom->Update_Unit_Models_Limit(TimeDelta)))
	//{
	//	MSGBOX("Player Failed : Update_Models");
	//}

	Update_BillboardMatrix();



	// 현재 누적된 이동량을 월드 매트릭스에 적용합니다.(커브 동작중에는 작동하지 않음)



	SetPos = false;
#if defined(USE_IMGUI)
	//Running_Window();
	if (m_bDelete)
	{
		Set_Release();
		return ISDEAD;
	}
#endif

	//콜라이더 업데이트
#pragma region XYZ Collider
	if (0 > m_pColliderSlotCom->Update_Colliders(m_pTransform->Get_WorldMatrix(), TimeDelta))
		return -1;
#pragma endregion

#pragma region XY Collider
	if (m_pCombat->Get_CombatDesc().eActorSide == CCombat::ACTOR_SIDE_LEFT)
	{
		if (0 > m_pColliderSlotCom->Update_Colliders(
			m_pTransform->Get_WorldMatrix(),
			TimeDelta,
			XMLoadFloat4x4(&m_matBillboard),
			g_iWinCX,
			g_iWinCY,
			false))
			return -1;

	}
	else
	{
		if (0 > m_pColliderSlotCom->Update_Colliders(
			m_pTransform->Get_WorldMatrix(),
			TimeDelta,
			XMLoadFloat4x4(&m_matBillboard),
			g_iWinCX,
			g_iWinCY,
			true))
			return -1;

	}
#pragma endregion

	//충돌 그룹 업데이트
	Update_CollisionGroup();
	if (0 > __super::Tick(TimeDelta))
		return -1;


	return 0;



}

_int CPOT_Object_Kosyogawa::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	if (m_pStateMachine->Check_State(CState_Machine::STATETYPE_INPUT) != ACTOR_DEFAULT_CURVED_TAG &&
		m_pStateMachine->Check_State(CState_Machine::STATETYPE_INPUT) != ACTOR_DEFAULT_CURVED_CHASE_TAG)
	{
		/*if (FAILED(m_pTransform->Reset_Momentum(m_pNavigation_Line)))
		{
			MSGBOX("Player Failed : Reset_Momentum_Line");
		}*/
		if (FAILED(m_pTransform->Reset_Momentum_Line(m_pNavigation_Line, m_tagActorDesc.bIsDnf)))
		{
			MSGBOX("Player Failed : Reset_Momentum_Line");
		}

	}
	return 0;
}

HRESULT CPOT_Object_Kosyogawa::Render()
{
	//1. 부모 객체 : 상수들 바인드
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (nullptr == m_pModelSlotCom)
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);




#ifdef _DEBUG
	CTransform* pTerrainTransform = (CTransform*)pGameInstance->Get_Component(m_iCurLevel, g_pLayerTags[STATIC_LAYER_TERRAIN], L"Com_Transform", 0);
	if (pTerrainTransform != nullptr)
		m_pNavigation_Line->Render(pTerrainTransform);
#endif

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CPOT_Object_Kosyogawa::Render_Shadow()
{
	//1. 부모 객체 : 상수들 바인드
	if (FAILED(__super::Render()))
		return E_FAIL;

	CPipeLine* pPipeLine = GET_INSTANCE(CPipeLine);
	_float4x4 matLightViewFloat4x4_TP, matLightProjFloat4x4_TP;
	matLightViewFloat4x4_TP = pPipeLine->Get_LightViewFloat4x4_TP();
	matLightProjFloat4x4_TP = pPipeLine->Get_LightProjFloat4x4_TP();
	if (FAILED(m_pShaderCom->Set_RawValue("g_LightViewMatrix", &matLightViewFloat4x4_TP, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_LightProjMatrix", &matLightProjFloat4x4_TP, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &matLightViewFloat4x4_TP, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &matLightProjFloat4x4_TP, sizeof(_float4x4))))
		return E_FAIL;
	RELEASE_INSTANCE(CPipeLine);

	if (nullptr == m_pModelSlotCom)
		return E_FAIL;




	return S_OK;
}

HRESULT CPOT_Object_Kosyogawa::Render_OutLine()
{
	//1. 부모 객체 : 상수들 바인드
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (nullptr == m_pModelSlotCom)
		return E_FAIL;

	_uint iOutLinePass = 0;

	if (m_bMirrorBillBoard == true)
	{
		if (m_pCombat->Get_CombatDesc().eActorSide == CCombat::ACTOR_SIDE_LEFT)
			iOutLinePass = 4;
		else
			iOutLinePass = 2;


	}
	else
	{
		if (m_pCombat->Get_CombatDesc().eActorSide == CCombat::ACTOR_SIDE_LEFT)
			iOutLinePass = 2;
		else
			iOutLinePass = 4;


	}

	/*if (m_pCombat->Get_CombatDesc().eActorSide == CCombat::ACTOR_SIDE_LEFT)
		iOutLinePass = 2;
	else
		iOutLinePass = 4;*/



	return S_OK;
}

_int CPOT_Object_Kosyogawa::Check_CombatState()
{


	return 0;
}

HRESULT CPOT_Object_Kosyogawa::SetUp_Components()
{


	return S_OK;

}

HRESULT CPOT_Object_Kosyogawa::SetUp_Colliders()
{
	//1. 붙힐뼈를 가진 모델
	CModel* pParentModel = nullptr;

	//2. 콜라이더 초기화 정보
	CCollider::COLLIDERDESC ColliderDesc;

#pragma region COLLIDER_RANGE
	pParentModel = nullptr;
	//if (pParentModel == nullptr)
	//    __debugbreak();


#pragma endregion
#pragma region COLLIDER_ATTACK
	// 무기 공격 박스
	/*pParentModel = nullptr;
	pParentModel = m_pModelSlotCom->Find_Model(MODEL_TAG_DEFAULT_BODY);

	if (pParentModel == nullptr)
		__debugbreak();*/

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	//ColliderDesc.pSocketNode = pParentModel->Find_HierarchyNode(ACT_DNF_CON_COLLIDER_ATTACK_01_Parent);
	XMStoreFloat4(&ColliderDesc.vRotation, XMQuaternionIdentity());
	XMStoreFloat3(&ColliderDesc.vScale, ACT_POT_KOSYOGAWA_COLLIDER_ATTACK_01_Scale);
	XMStoreFloat4(&ColliderDesc.vTranslation, ACT_POT_KOSYOGAWA_COLLIDER_ATTACK_01_Translation);

	DEBUG_BREAK(m_pColliderSlotCom->Insert_Collider(
		CColliderSlot::COLLIDER_BODY_WEAPON,
		ACT_POT_KOSYOGAWA_COLLIDER_ATTACK_01_NAME,
		LEVEL_STATIC,
		TEXT("Prototype_Component_Collider_Rect"),
		&ColliderDesc
	));





#pragma endregion
	return S_OK;

}

HRESULT CPOT_Object_Kosyogawa::SetUp_Model()
{


	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//Model 추가 =============================================================================
	//Main 모델
	CModel* pParentModel = nullptr;



	RELEASE_INSTANCE(CGameInstance);

	return S_OK;

}

void CPOT_Object_Kosyogawa::Update_CollisionGroup()
{
	//시작시 좌측일때
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (m_tagActorDesc.bIsRight == false)
		pGameInstance->Add_Collision_Group(CCollision_Interactor::COLLISION_GROUP_PLAYER_LEFT_SUB, this);
	else
		pGameInstance->Add_Collision_Group(CCollision_Interactor::COLLISION_GROUP_PLAYER_RIGHT_SUB, this);

	RELEASE_INSTANCE(CGameInstance);
}
void CPOT_Object_Kosyogawa::Set_Pos(_vector Pos)
{
	m_pTransform->Set_State(CTransform::STATE_POSITION, Pos);
}
void CPOT_Object_Kosyogawa::Set_Ratio(_double _Ratio)
{
	m_pTransform->Set_Ratio(_Ratio);
}
void CPOT_Object_Kosyogawa::Set_ExHeight(_double _ExHegiht)
{
	m_pTransform->Set_ExtraHeight(_ExHegiht);

}
CPOT_Object_Kosyogawa* CPOT_Object_Kosyogawa::Create(ID3D11Device* pDevice,
	ID3D11DeviceContext* pDeviceContext)
{
	CPOT_Object_Kosyogawa* pInstance = new CPOT_Object_Kosyogawa(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CPOT_Object_Kosyogawa");
		Safe_Release(pInstance);
	}
	return pInstance;

}

CGameObject* CPOT_Object_Kosyogawa::Clone(void* pArg)
{
	CPOT_Object_Kosyogawa* pInstance = new CPOT_Object_Kosyogawa(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CPOT_Object_Kosyogawa");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPOT_Object_Kosyogawa::Free()
{

	__super::Free();
}
