#include "stdafx.h"
#include "StaticObject.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "Static_Light.h"
#include "Shader_Manager.h"
#include "Battle_Level.h"

#if defined(USE_IMGUI)
#include "ImGui_Manager.h"
static CGameObject* pSelectedObject = nullptr;
#endif

CStaticObject::CStaticObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
    : CGameObject(pDevice, pDeviceContext)
{
}

CStaticObject::CStaticObject(const CStaticObject& rhs)
    : CGameObject(rhs)
{
}

HRESULT CStaticObject::NativeConstruct_Prototype()
{
    if (FAILED(__super::NativeConstruct_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CStaticObject::NativeConstruct(void* pArg)
{
	//1. 게임인스턴스 준비
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	_uint iCurLevel = pGameInstance->Get_CurrentLevelIndex();


	//2. 예외처리
	if (iCurLevel == LEVEL_LOGO ||
		iCurLevel == LEVEL_MAIN_MENU)
	{
		MSGBOX("CState_Player_Turn Error : CurLevel is invailed");
		return E_FAIL;
	}

	//3. 로딩 레벨의 경우
	if (iCurLevel == LEVEL_LOADING)
	{
		iCurLevel = dynamic_cast<CLevel_Loading*>(pGameInstance->Get_LoadingLevel())->Get_NextLevel();
	}

	RELEASE_INSTANCE(CGameInstance);

	//1. GameObject 초기화 : 트랜스폼 컴포넌트를 
	if (FAILED(__super::NativeConstruct((CTransform::TRANSFORMDESC*)pArg)))
		return E_FAIL;

	//3. 초기화 구조체로 값설정
	if (pArg)
	{
		OBJECTDESC temp;
		memcpy(&temp, pArg, sizeof(OBJECTDESC));
		m_iModelIndex = temp.iModelNumber;
		m_iLayerIndex = temp.iLayerNumber;

		//4. 모델 컴포넌트 추가
		/* For.Com_Model */
		DEBUG_BREAK(__super::Add_Component(
			iCurLevel,
			g_pModelTags[m_iModelIndex],
			TEXT("Com_Model"),
			(CComponent**)&m_pModelCom))

		m_pTransform->Set_TransformMatrix(temp.matTransformMatrix);

	}
	else
	{
		//4. 모델 컴포넌트 추가(디폴트 0)
		/* For.Com_Model */
		if (FAILED(__super::Add_Component(
			LEVEL_STATIC,
			g_pModelTags[m_iModelIndex],
			TEXT("Com_Model"),
			(CComponent**)&m_pModelCom)))
			return E_FAIL;
	}

	//2. CStatic_Object 초기화 : 논애니 셰이더, 랜더러
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	CCollider::COLLIDERDESC			ColliderDesc;
	/* For.Com_Sphere */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float4(0.f, 0.3f, 0.f, 1.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_Sphere"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;

#if defined(USE_IMGUI)
	//초기값설정

	_vector		vRight = m_pTransform->Get_State(CTransform::STATE_RIGHT);
	_vector		vUp = m_pTransform->Get_State(CTransform::STATE_UP);
	_vector		vLook = m_pTransform->Get_State(CTransform::STATE_LOOK);


	//1. 회전
	_float roll, yaw, pitch;

	roll = XMVectorGetX(XMVector3Dot(XMVector3Normalize(XMVectorSet(XMVectorGetX(vRight), XMVectorGetY(vRight), 0.f, 0.f)), XMVectorSet(1.f, 0.f, 0.f, 0.f)));
	yaw = XMVectorGetX(XMVector3Dot(XMVector3Normalize(XMVectorSet(XMVectorGetX(vLook), 0.f, XMVectorGetZ(vLook), 0.f)), XMVectorSet(0.f, 0.f, 1.f, 0.f)));
	pitch = XMVectorGetX(XMVector3Dot(XMVector3Normalize(XMVectorSet(0.f, XMVectorGetY(vLook), XMVectorGetZ(vLook), 0.f)), XMVectorSet(0.f, 0.f, 1.f, 0.f)));

	m_vDegree[0] = XMConvertToDegrees(acos(roll));
	m_vDegree[1] = XMConvertToDegrees(acos(yaw));
	m_vDegree[2] = XMConvertToDegrees(acos(pitch));
	m_vDegree[3] = 0.f;

	//2. 위치
	m_vLocation[0] = XMVectorGetX(m_pTransform->Get_State(CTransform::STATE_POSITION));
	m_vLocation[1] = XMVectorGetY(m_pTransform->Get_State(CTransform::STATE_POSITION));
	m_vLocation[2] = XMVectorGetZ(m_pTransform->Get_State(CTransform::STATE_POSITION));
	m_vLocation[3] = 1.f;

	//3. 스케일
	m_vScale[0] = XMVectorGetX(m_pTransform->Get_Scale(CTransform::STATE_RIGHT));
	m_vScale[1] = XMVectorGetX(m_pTransform->Get_Scale(CTransform::STATE_UP));
	m_vScale[2] = XMVectorGetX(m_pTransform->Get_Scale(CTransform::STATE_LOOK));
	m_vScale[3] = 0.f;

#endif

	SetUp_Range();
	Model_Identitiy();

	return S_OK;

}

_int CStaticObject::Tick(_double TimeDelta)
{
	// 카메라모션블러
	if (nullptr != m_pTransform)
		m_pTransform->Tick_OldWorldMatrix();

	if (0 > __super::Tick(TimeDelta))
		return -1;
	Model_Identitiy_Update(TimeDelta);
	Model_Rotation(TimeDelta * 0.05f);
#if defined(USE_IMGUI)
	Running_Window();
	if (m_bDelete)
	{
		Set_Release();
		return ISDEAD;
	}
#endif

	return 0;
}

_int CStaticObject::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_vector vCamPos = pGameInstance->Get_CamPosition();
	switch (m_iModelIndex)
	{
	case SM_Mayship_Sky:
	case SM_Ma_Sky:
	case SM_Dam_Sky:
	case SM_DK_Sky:
		m_pTransform->Set_State(CTransform::STATE_POSITION, vCamPos);
		if (true == pGameInstance->isIn_WorldSpace(m_pTransform->Get_State(CTransform::STATE_POSITION), m_fMaxScale))
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);
		break;
	default:
		if (true == pGameInstance->isIn_WorldSpace(m_pTransform->Get_State(CTransform::STATE_POSITION), m_fMaxScale))
		{
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_FIRST, this);
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_SHADOW, this);
		}
		break;
	}

#ifdef _DEBUG
	m_pSphereCom->Update_Transform_DEBUG(XMMatrixScaling(m_fRadius, m_fRadius, m_fRadius), m_pTransform->Get_WorldMatrix());
	//m_pRendererCom->Add_Debug(m_pSphereCom);
#endif // _DEBUG

	RELEASE_INSTANCE(CGameInstance);

	return 0;
}

HRESULT CStaticObject::Render()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	_bool bAnimMotionBlur = true;
	//m_pShaderCom->Set_RawValue("g_bAnimMotionBlur", &bAnimMotionBlur, sizeof(_bool));

	_uint iNumMaterials = m_pModelCom->Get_NumMaterials();

	for (_uint i = 0; iNumMaterials > i; ++i)
	{
		int temp = 0;
		m_pModelCom->Bind_OnShader(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture");
		_bool bNormalTexture = false;
		m_pModelCom->Bind_OnShader(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", &bNormalTexture);
		m_pShaderCom->Set_RawValue("g_bNormalTexture", &bNormalTexture, sizeof(_bool));
		if (m_pModelCom->Get_ModelType() != CModel::TYPE_NONANIM)
		{
			switch (m_iModelIndex)
			{
			case SM_Sn_Robot:
			case SM_Sn_RobotAn:
				if (1 == i)
				{
					_bool bEmissive = true;
					m_pShaderCom->Set_RawValue("g_bEmissive", &bEmissive, sizeof(_bool));
					m_pModelCom->Render(m_pShaderCom, 5, i, "g_BoneMatrices");
					bEmissive = false;
					m_pShaderCom->Set_RawValue("g_bEmissive", &bEmissive, sizeof(_bool));
				}
				else
					m_pModelCom->Render(m_pShaderCom, 5, i, "g_BoneMatrices");
				break;
			default:
				m_pModelCom->Render(m_pShaderCom, 5, i, "g_BoneMatrices");
				break;
			}
		}
		else
		{
			switch (m_iModelIndex)
			{
			case SM_Mayship_Sky:
			case SM_Ma_Sky:
			case SM_Dam_Sky:
				m_pModelCom->Render(m_pShaderCom, 3, i, "g_BoneMatrices");
				break;
			case SM_DK_Sky:
			{
				CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
				CLevel* pCurLevel = pGameInstance->Get_CurLevel();
				_uint iCurLevel = pGameInstance->Get_CurrentLevelIndex();
				if (MAP_TYPE_SNOW == static_cast<CBattle_Level*>(pCurLevel)->Get_CurMapType() || LEVEL_SNOW == iCurLevel)
					m_pModelCom->Render(m_pShaderCom, 14, i, "g_BoneMatrices");
				else
					m_pModelCom->Render(m_pShaderCom, 3, i, "g_BoneMatrices");
				RELEASE_INSTANCE(CGameInstance);
			}
				break;
			case SM_DK_Gate01: // 이미시브 & 글로우
			case SM_DK_Gate02:
			case SM_DK_Gate03:
			case SM_DK_Gate04:
			case SM_DK_GateDifferent:
				if (0 == i)
				{
					_bool bEmissive = true;
					m_pShaderCom->Set_RawValue("g_bEmissive", &bEmissive, sizeof(_bool));
					m_pModelCom->Render(m_pShaderCom, m_iPassIndex, i);
					bEmissive = false;
					m_pShaderCom->Set_RawValue("g_bEmissive", &bEmissive, sizeof(_bool));
				}
				else
					m_pModelCom->Render(m_pShaderCom, m_iPassIndex, i);
				break;
			//case SM_Sn_Corridor:
			//	if (2 == i)
			//	{
			//		_bool bEmissive = true;
			//		m_pShaderCom->Set_RawValue("g_bEmissive", &bEmissive, sizeof(_bool));
			//		m_pModelCom->Render(m_pShaderCom, m_iPassIndex, i);
			//		bEmissive = false;
			//		m_pShaderCom->Set_RawValue("g_bEmissive", &bEmissive, sizeof(_bool));
			//	}
			//	else
			//		m_pModelCom->Render(m_pShaderCom, m_iPassIndex, i);
			//	break;
			case SM_DK_MainGate:
				if (3 < i)
				{
					_bool bEmissive = true;
					m_pShaderCom->Set_RawValue("g_bEmissive", &bEmissive, sizeof(_bool));
					m_pModelCom->Render(m_pShaderCom, m_iPassIndex, i);
					bEmissive = false;
					m_pShaderCom->Set_RawValue("g_bEmissive", &bEmissive, sizeof(_bool));
				}
				else
					m_pModelCom->Render(m_pShaderCom, m_iPassIndex, i);
				break;
			default:
				m_pModelCom->Render(m_pShaderCom, m_iPassIndex, i);
				break;
			}
		}

		bNormalTexture = false;
		m_pShaderCom->Set_RawValue("g_bNormalTexture", &bNormalTexture, sizeof(_bool));
	}

	return S_OK;
}

HRESULT CStaticObject::Render_Shadow()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
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

	_uint iNumMaterials = m_pModelCom->Get_NumMaterials();

	for (_uint i = 0; iNumMaterials > i; ++i)
	{
		int temp = 0;
		m_pModelCom->Bind_OnShader(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture");
		m_pModelCom->Bind_OnShader(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture");
		if (m_pModelCom->Get_ModelType() != CModel::TYPE_NONANIM)
			m_pModelCom->Render(m_pShaderCom, 1, i, "g_BoneMatrices");
		else
			m_pModelCom->Render(m_pShaderCom, 1, i);
	}

	return S_OK;
}

void CStaticObject::SetUp_Range()
{
	//절두체 컬링을 위한 범위지정 -> 툴에서 보고 입력
	switch (m_iModelIndex)
	{
	//	Sky
	case SM_Mayship_Sky:
	case SM_Ma_Sky:
	case SM_Dam_Sky:
	case SM_DK_Sky:
		m_fRadius = 1000.f;
		break;

	case SM_Tree_Japanese_Maple_1:
		break;
	//	Mayship
	case SM_Mayship_Rope00:
	case SM_Mayship_Rope01:
	case SM_Mayship_Rope02:
	case SM_Mayship_Rope03:
	case SM_Mayship_Rope04:
	case SM_Mayship_Rope05:
		m_fRadius = 25.f;
		break;
	case SM_Mayship_Ei:
	case SM_Mayship_Aj:
	case SM_Mayship_Kr:
	case SM_Mayship_Ir:
		m_fRadius = 30.f;
		break;
	case SM_Mayship_Scaffold00:
	case SM_Mayship_Scaffold01:
	case SM_Mayship_Scaffold02:
	case SM_Mayship_Scaffold04:
	case SM_Mayship_Scaffold05:
		m_fRadius = 50.f;
		break;
	case SM_Mayship_ScaffoldMarge:
		m_fRadius = 100.f;
		break;
	case SM_Mayship_Whale:
		m_fRadius = 120.f;
		break;

	//	Snow
	case SM_Sn_ContainerUpper:
		m_fRadius = 0.1f;
		break;
	case SM_Sn_ContainerA:
	case SM_Sn_ContainerB:
	case SM_Sn_Corridor:
	case SM_Sn_CorridorA:
	case SM_Sn_CorridorD:
	case SM_Sn_CorridorE:
	case SM_Sn_RockS:
	case SM_Sn_SnowBike:
	case SM_Sn_Exit:
		m_fRadius = 5.f;
		break;
	case SM_Sn_RockM:
		m_fRadius = 8.f;
		break;
	case SM_Sn_FloorB:
	case SM_Sn_ContainerUpperCorner:
		m_fRadius = 10.f;
		break;
	case SM_Sn_RockL:
	case SM_Sn_SoftWoodA1:
	case SM_Sn_SoftWoodA2:
	case SM_Sn_SoftWoodB1:
	case SM_Sn_SoftWoodB2:
	case SM_Sn_SoftWoodB3:
	case SM_Sn_SoftWoodB4:
		m_fRadius = 20.f;
		break;
	case SM_Sn_Stairs:
	case SM_Sn_CorridorF:
	case SM_Sn_CorridorG:
	case SM_Sn_Prop:
		m_fRadius = 30.f;
		break;
	case SM_Sn_Bastion01:
	case SM_Sn_Bastion02:
	case SM_Sn_Fence:
		m_fRadius = 100.f;
		break;
	case SM_Sn_Mountain2:
	case SM_Sn_Robot:
	case SM_Sn_RobotAn:
		m_fRadius = 3000.f;
		break;

	//	Market
	case SM_Ma_Rose:
	case SM_Ma_RoseB:
	case SM_Ma_RoseC:
	case SM_Ma_RoseD:
		m_fRadius = 0.5f;
		break;
	case SM_Ma_EaselA:
	case SM_Ma_PictureA:
	case SM_Ma_PictureB:
	case SM_Ma_PictureC:
	case SM_Ma_PictureD:
	case SM_Ma_PictureE:
	case SM_Ma_PictureF:
		m_fRadius = 3.f;
		break;
	case SM_Ma_FenceA:
	case SM_Ma_FenceAA:
	case SM_Ma_FenceAB:
		m_fRadius = 15.f;
		break;
	case SM_Ma_Boat:
	case SM_Ma_ChurchA01:
	case SM_Ma_PillarA:
	case SM_Ma_RockM:
	case SM_Ma_RockS:
	case SM_Ma_Statue:
	case SM_Ma_HardWood:
	case SM_Ma_WallA00:
	case SM_Ma_SineA:
	case SM_Ma_SineA02:
	case SM_Ma_Rope01:
	case SM_Ma_FloorBrickA:
	case SM_Ma_FloorBrickB:
	case SM_Ma_FloorA:
	case SM_Ma_FloorAA:
	case SM_Ma_FloorBB:
	case SM_Ma_Fence00:
	case SM_Ma_BridgeB01:
	case SM_Ma_FloatingRing:
		m_fRadius = 10.f;
		break;
	case SM_Ma_HouseA:
	case SM_Ma_HouseA01:
	case SM_Ma_HouseA03:
	case SM_Ma_HouseB:
	case SM_Ma_HouseC:
	case SM_Ma_HouseC2F3F:
	case SM_Ma_HouseC03:
	case SM_Ma_HouseD:
	case SM_Ma_HouseD01:
	case SM_Ma_HouseD02:
	case SM_Ma_HouseD03:
	case SM_Ma_HouseD04:
	case SM_Ma_HouseD06:
	case SM_Ma_FarHouse04:
		m_fRadius = 20.f;
		break;
	case SM_Ma_CarpetB:
	case SM_Ma_CarpetC:
	case SM_Ma_CarpetD:
	case SM_Ma_ChurchB:
	case SM_Ma_FloorB01:
	case SM_Ma_FloorC02:
	case SM_Ma_FloorD02:
	case SM_Ma_HouseFlower:
	case SM_Ma_RockL:
	case SM_Ma_RockWood:
	case SM_Ma_WallA01:
		m_fRadius = 30.f;
		break;
	case SM_Ma_BridgeA:
	case SM_Ma_Ship:
	case SM_Ma_Water:
		m_fRadius = 500.f;
		break;

	//	Dam
	case SM_Dam_Around:
	case SM_Dam_AroundFountain:
	case SM_Dam_Coach:
	case SM_Dam_FenceA:
	case SM_Dam_FencePoint:
	case SM_Dam_Joint01:
	case SM_Dam_Joint02:
	case SM_Dam_WingGround:
		m_fRadius = 5.f;
		break;
	case SM_Dam_FenceB:
		m_fRadius = 8.f;
		break;
	case SM_Dam_GardenStep:
	case SM_Dam_DeerBody:
	case SM_Dam_DeerWingAll:
	case SM_Dam_DeerWingGlassFlame:
	case SM_Dam_ElectricLight:
	case SM_Dam_Stairs:
	case SM_Dam_StairsCup:
		m_fRadius = 10.f;
		break;
	case SM_Dam_AroundCornerC:
		m_fRadius = 15.f;
		break;
	case SM_Dam_FloorA:
	case SM_Dam_FloorB:
	case SM_Dam_CastleFlying:
	case SM_Dam_CastleRoof:
	case SM_Dam_CastleWindowUnder:
	case SM_Dam_CastleWindowUp:
	case SM_Dam_SteepleLow:
	case SM_Dam_UnderPillar:
		m_fRadius = 30.f;
		break;
	case SM_Dam_DeerPillar:
	case SM_Dam_FarHouse01:
	case SM_Dam_FarHouse02:
	case SM_Dam_FarHouse03:
	case SM_Dam_FarHouse04:
	case SM_Dam_SteepleLowPillar:
	case SM_Dam_SteepleUp:
		m_fRadius = 100.f;
		break;
	case SM_Dam_StationA:
	case SM_Dam_StationB:
	case SM_Dam_StationC:
	case SM_Dam_StationD:
	case SM_Dam_StationF:
	case SM_Dam_StationG:
	case SM_Dam_Goddess:
		m_fRadius = 500.f;
		break;

	//	Dark
	case SM_DK_BigWood:
	case SM_DK_TreeDead:
	case SM_DK_Gate01:
	case SM_DK_Gate02:
	case SM_DK_Gate03:
	case SM_DK_Gate04:
	case SM_DK_GateDifferent:
	case SM_DK_ScaffoldArea1:
		m_fRadius = 60.f;
		break;
	case SM_DK_BridgeShort:
	case SM_DK_MainGate:
	case SM_DK_DemonBody02:
	case SM_DK_DemonHeadA:
	case SM_DK_DemonHeadB:
	case SM_DK_GothicBuilding04:
		m_fRadius = 100.f;
		break;
	case SM_DK_BridgeArea0:
		m_fRadius = 400.f;
		break;
	case SM_DK_GothicBuilding01:
	case SM_DK_GothicBuilding02:
	case SM_DK_GothicBuilding03:
	case SM_DK_GothicBuildingArea1:
	case SM_DK_GothicWall:
	case SM_DK_GothicWall01:
	case SM_DK_GothicWallBuilding:
	case SM_DK_GothicWallBuilding01:
	case SM_DK_GothicWallBuilding01Far:
	case SM_DK_GothicWallBuildingFar:
	case SM_DK_GothicBuildingClear:
	case SM_DK_MainDemon01:
	case SM_DK_GothicBuilding04L:
	case SM_DK_DemonSickle:
		m_fRadius = 1000.f;
		break;
	}

	//XYZ 중 가장 큰 값을 기준으로 콜라이더 원을생성
	_vector vScale = m_pTransform->Get_Scale();

	m_fMaxScale = m_fRadius * XMVectorGetX(vScale) * 0.7f;

	if (XMVectorGetX(vScale) < XMVectorGetY(vScale))
		m_fMaxScale = m_fRadius * XMVectorGetY(vScale) * 0.7f;

	if (XMVectorGetY(vScale) < XMVectorGetZ(vScale))
		m_fMaxScale = m_fRadius * XMVectorGetZ(vScale) * 0.7f;
}

void CStaticObject::Model_Identitiy()
{
	switch (m_iModelIndex)
	{
	//case SM_TESTOBJECT00:
	case SM_Mayship_Whale:
	case SM_Mob_Jans:
	case SM_DK_DemonBody01:
	case SM_Mob_Engine:
	case SM_Mob_Cat:
		m_pModelCom->SetUp_AnimIndex(0);
		break;
	case SM_DK_DemonBody02:
		m_pModelCom->SetUp_AnimIndex(1);
		break;
	case SM_Sn_Robot:
		m_pModelCom->SetUp_AnimIndex(5);
		break;
	case SM_Mob_Swmn:
		m_pModelCom->SetUp_AnimIndex(13);
		break;
	case SM_Sn_RobotAn:
		m_pModelCom->SetUp_AnimIndex(2);
		break;
	default:
		break;
	}
}

void CStaticObject::Model_Identitiy_Update(_double TimeDelta)
{
	switch (m_iModelIndex)
	{
	//case SM_TESTOBJECT00:
	case SM_Mayship_Whale:
	case SM_Sn_Robot:
	case SM_DK_DemonBody01:
	case SM_DK_DemonBody02:
	case SM_Mob_Jans:
	case SM_Mob_Engine:
	case SM_Mob_Swmn:
	case SM_Mob_Cat:
	case SM_Sn_RobotAn:
		m_pModelCom->Update_CombinedTransformationMatrices(TimeDelta);
		break;
	default:
		break;
	}
}

void CStaticObject::Model_Rotation(_double TimeDelta)
{
	_vector		vAxis = XMVectorSet(0.f, 1.f, 0.f, 1.f);
	switch (m_iModelIndex)
	{
	case SM_Mayship_Sky:
	case SM_Ma_Sky:
	case SM_Dam_Sky:
	case SM_DK_Sky:
		m_pTransform->Turn(vAxis, TimeDelta);
		break;
	case SM_Sn_Antenna:
		m_pTransform->Turn(vAxis, TimeDelta * 100.f);
		break;
	default:
		break;
	}
}

HRESULT CStaticObject::SetUp_Components()
{
	/* For.Com_Renderer */
	DEBUG_BREAK(__super::Add_Component(
		LEVEL_STATIC,
		TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"),
		(CComponent**)&m_pRendererCom))

		switch (m_iModelIndex)
		{
		//	애니메이션 모델
		//case SM_TESTOBJECT00:
		case SM_Mayship_Whale:
		case SM_Sn_Robot:
		case SM_DK_DemonBody01:
		case SM_DK_DemonBody02:
		case SM_Mob_Jans:
		case SM_Mob_Engine:
		case SM_Mob_Swmn:
		case SM_Mob_Cat:
		case SM_Sn_RobotAn:
			/* For.Com_Shader */
			DEBUG_BREAK(__super::Add_Component(
				LEVEL_STATIC,
				g_pShaderTag[Shader_AnimModel],
				TEXT("Com_Shader"),
				(CComponent**)&m_pShaderCom))
			break;
		default:
			/* For.Com_Shader */
			DEBUG_BREAK(__super::Add_Component(
				LEVEL_STATIC,
				g_pShaderTag[Shader_NonAnimModel],
				TEXT("Com_Shader"),
				(CComponent**)&m_pShaderCom))
			break;
		}


	return S_OK;
}

HRESULT CStaticObject::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (FAILED(m_pTransform->Bind_OnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;
	RELEASE_INSTANCE(CGameInstance);

	// 모션 블러
	CPipeLine* pPipeLine = GET_INSTANCE(CPipeLine);
	_float4x4 OldWorldMatrix;
	XMStoreFloat4x4(&OldWorldMatrix, XMMatrixTranspose(m_pTransform->Get_OldWorldMatrix()));
	if (FAILED(m_pShaderCom->Set_RawValue("g_OldWorldMatrix", &OldWorldMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_OldViewMatrix", &pPipeLine->Get_OldTransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	RELEASE_INSTANCE(CPipeLine);

	// HDR
	if (FAILED(m_pShaderCom->Set_RawValue("g_bHDR", &g_bHDR, sizeof(_bool))))
		return E_FAIL;

	// 이미시브 & 글로우
	if (FAILED(m_pShaderCom->Set_RawValue("g_bEmissive", &m_bGlow, sizeof(_bool))))
		return E_FAIL;

	return S_OK;
}

CStaticObject* CStaticObject::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CStaticObject* pInstance = new CStaticObject(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CStaticObject");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CStaticObject::Clone(void* pArg)
{
	CStaticObject* pInstance = new CStaticObject(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Clone CStaticObject");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStaticObject::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pSphereCom);
}

//IMGUI========================================================================================================================================================================================
#if defined(USE_IMGUI)

void CStaticObject::Running_Window()
{
	//1. 활성화 상태확인
	if (m_bShow_Window == false)
		return;
	
	//2, 윈도우 기능추가
	static ImGuiWindowFlags flags = 0;
	ImGui::Begin("Static_Map_Object_Window", 0, flags);
	CImGui_Manager* pImguiManager = GET_INSTANCE(CImGui_Manager);

	//변경사항적용
	Edit_Input();

	//3. 윈도우띄우기
	Widget_Transform();
	pImguiManager->Small_Separator();
	RELEASE_INSTANCE(CImGui_Manager);
	ImGui::End();
}



void CStaticObject::Show_Window()
{
	if (pSelectedObject == this)
	{
		Set_Release();
		return;
	}
	else if (pSelectedObject != nullptr)
	{
		dynamic_cast<CStaticObject*>(pSelectedObject)->Set_Release();
	}

	//선택되었을때 셰이더 패스 처리 하기
	pSelectedObject = this;
	m_bShow_Window = true;
	m_iPassIndex = 2;
}

void CStaticObject::Set_Release()
{
	//선택해제되었을때 셰이더 패스 처리 하기
	m_bShow_Window = false;
	pSelectedObject = nullptr;
	m_iPassIndex = 0;
}

void CStaticObject::Edit_Input()
{
	_vector		vRight = m_pTransform->Get_State(CTransform::STATE_RIGHT);
	_vector		vUp = m_pTransform->Get_State(CTransform::STATE_UP);
	_vector		vLook = m_pTransform->Get_State(CTransform::STATE_LOOK);
	_vector		vPos = m_pTransform->Get_State(CTransform::STATE_POSITION);

	//1. 회전
	_vector		vRotation = { XMConvertToRadians(m_vDegree[0]),XMConvertToRadians(m_vDegree[1]) ,XMConvertToRadians(m_vDegree[2]) ,0.f };
	m_pTransform->RotationPYR(vRotation);

	//2. 위치
	_vector		vLocation = { m_vLocation[0],m_vLocation[1] ,m_vLocation[2] ,1.f };
	m_pTransform->Set_State(CTransform::STATE_POSITION, vLocation);

	//3. 스케일
	_vector		vScale = { m_vScale[0],m_vScale[1] ,m_vScale[2] ,1.f };
	m_pTransform->Scaled(vScale);
}

void CStaticObject::Widget_Transform()
{
	CImGui_Manager* pImguiManager = GET_INSTANCE(CImGui_Manager);

	ImGui::DragFloat("Location X", &m_vLocation[0], 0.01f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None);
	ImGui::DragFloat("Location Y", &m_vLocation[1], 0.01f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None);
	ImGui::DragFloat("Location Z", &m_vLocation[2], 0.01f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None);

	//ImGui::InputFloat("Location Y", &m_vLocation[1], 0.5f, 1.0f, "%.3f");
	//ImGui::InputFloat("Location Z", &m_vLocation[2], 0.5f, 1.0f, "%.3f");
	pImguiManager->Small_Separator();
	ImGui::DragFloat("Rotation X", &m_vDegree[0], 0.1f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None);
	ImGui::DragFloat("Rotation Y", &m_vDegree[1], 0.1f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None);
	ImGui::DragFloat("Rotation Z", &m_vDegree[2], 0.1f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None);

	//ImGui::InputFloat("Rotation X", &m_vDegree[0], 0.5f, 1.0f, "%.3f");
	//ImGui::InputFloat("Rotation Y", &m_vDegree[1], 0.5f, 1.0f, "%.3f");
	//ImGui::InputFloat("Rotation Z", &m_vDegree[2], 0.5f, 1.0f, "%.3f");
	pImguiManager->Small_Separator();
	ImGui::DragFloat("Scale X", &m_vScale[0], 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None);
	ImGui::DragFloat("Scale Y", &m_vScale[1], 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None);
	ImGui::DragFloat("Scale Z", &m_vScale[2], 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None);

	pImguiManager->Small_Separator();

	ImGui::DragFloat("Range", &m_fRadius, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None);

	//ImGui::InputFloat("Scale X", &m_vScale[0], 0.1f, 1.0f, "%.3f");
	//ImGui::InputFloat("Scale Y", &m_vScale[1], 0.1f, 1.0f, "%.3f");
	//ImGui::InputFloat("Scale Z", &m_vScale[2], 0.1f, 1.0f, "%.3f");

	pImguiManager->Small_Separator();

	if (ImGui::SmallButton("Delete"))
	{
		m_bDelete = true;
	}

	RELEASE_INSTANCE(CImGui_Manager);

}
#endif
//IMGUI========================================================================================================================================================================================
