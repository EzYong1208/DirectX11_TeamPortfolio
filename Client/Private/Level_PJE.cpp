#include "stdafx.h"
#include "Level_PJE.h"
#include "GameInstance.h"
#include "Camera.h"
#include "Static_Light.h"
#include "Shader_Manager.h"

//IMGUI========================================================================================================================================================================================
#if defined(USE_IMGUI)
#include "ImGui_Manager.h"

const char* staticModel[] = {
    "SM_Tree_Japanese_Maple_1",

	"SM_TESTOBJECT00",
	"SM_TESTOBJECT01",

	"SM_Mayship_Aj",
	"SM_Mayship_Ei",
	"SM_Mayship_Ir",
	"SM_Mayship_Iw",
	"SM_Mayship_Kr",
	"SM_Mayship_Scaffold00",
	"SM_Mayship_Scaffold01",
	"SM_Mayship_Scaffold02",
	"SM_Mayship_Scaffold04",
	"SM_Mayship_Scaffold05",
	"SM_Mayship_ScaffoldMarge",
	"SM_Mayship_Rope00",
	"SM_Mayship_Rope01",
	"SM_Mayship_Rope02",
	"SM_Mayship_Rope03",
	"SM_Mayship_Rope04",
	"SM_Mayship_Rope05",
	"SM_Mayship_Whale",
	"SM_Mayship_Sky",

	"SM_Sn_Antenna",
	"SM_Sn_Bastion01",
	"SM_Sn_Bastion02",
	"SM_Sn_ContainerUpperCorner",
	"SM_Sn_ContainerA",
	"SM_Sn_ContainerB",
	"SM_Sn_ContainerC",
	"SM_Sn_Corridor",
	"SM_Sn_CorridorA",
	"SM_Sn_CorridorD",
	"SM_Sn_CorridorE",
	"SM_Sn_CorridorF",
	"SM_Sn_Exit",
	"SM_Sn_CorridorG",
	"SM_Sn_Flag",
	"SM_Sn_FloorB",
	"SM_Sn_Mountain2",
	"SM_Sn_Prop",
	"SM_Sn_Robot",
	"SM_Sn_RockL",
	"SM_Sn_RockM",
	"SM_Sn_RockS",
	"SM_Sn_RockSS",
	"SM_Sn_SnowBike",
	"SM_Sn_SoftWoodA1",
	"SM_Sn_SoftWoodA2",
	"SM_Sn_SoftWoodB1",
	"SM_Sn_SoftWoodB2",
	"SM_Sn_SoftWoodB3",
	"SM_Sn_SoftWoodB4",
	"SM_Sn_Stairs",
	"SM_Sn_ContainerUpper",

	"SM_Ma_Barrel",
	"SM_Ma_Boat",
	"SM_Ma_Bottle",
	"SM_Ma_BridgeA",
	"SM_Ma_BridgeB01",
	"SM_Ma_CarpetB",
	"SM_Ma_CarpetC",
	"SM_Ma_CarpetD",
	"SM_Ma_Chain",
	"SM_Ma_Chair",
	"SM_Ma_ChampagneBottleA",
	"SM_Ma_ChampagneGlass",
	"SM_Ma_ChandelierA",
	"SM_Ma_ChurchA01",
	"SM_Ma_ChurchB",
	"SM_Ma_ChurchB_R01",
	"SM_Ma_ChurchB02",
	"SM_Ma_Clock",
	"SM_Ma_ColanderA",
	"SM_Ma_DeskD",
	"SM_Ma_EaselA",
	"SM_Ma_FarGround",
	"SM_Ma_FarHouse01",
	"SM_Ma_FarHouse02",
	"SM_Ma_FarHouse03",
	"SM_Ma_FarHouse04",
	"SM_Ma_FarHouseBridge",
	"SM_Ma_FarHouseWall",
	"SM_Ma_Fence00",
	"SM_Ma_FenceA",
	"SM_Ma_FenceA01",
	"SM_Ma_FenceA02",
	"SM_Ma_FloatingRing",
	"SM_Ma_FloorA",
	"SM_Ma_FloorB00",
	"SM_Ma_FloorB01",
	"SM_Ma_FloorC02",
	"SM_Ma_FloorD02",
	"SM_Ma_FlowerB",
	"SM_Ma_FlowerE",
	"SM_Ma_HardWood",
	"SM_Ma_HempSackA",
	"SM_Ma_HempSackB",
	"SM_Ma_HempSackC",
	"SM_Ma_HouseFlower",
	"SM_Ma_HouseA",
	"SM_Ma_HouseA01",
	"SM_Ma_HouseA03",
	"SM_Ma_HouseB",
	"SM_Ma_HouseC",
	"SM_Ma_HouseC2F3F",
	"SM_Ma_HouseC03",
	"SM_Ma_HouseD",
	"SM_Ma_HouseD01",
	"SM_Ma_HouseD02",
	"SM_Ma_HouseD03",
	"SM_Ma_HouseD04",
	"SM_Ma_HouseD06",
	"SM_Ma_Irand2D",
	"SM_Ma_LampA",
	"SM_Ma_LeafA",
	"SM_Ma_PictureA",
	"SM_Ma_PictureB",
	"SM_Ma_PictureC",
	"SM_Ma_PictureD",
	"SM_Ma_PictureE",
	"SM_Ma_PictureF",
	"SM_Ma_PillarA",
	"SM_Ma_PlantA",
	"SM_Ma_PotA",
	"SM_Ma_RockL",
	"SM_Ma_RockM",
	"SM_Ma_RockS",
	"SM_Ma_Rope01",
	"SM_Ma_Rose",
	"SM_Ma_RoseB",
	"SM_Ma_RoseC",
	"SM_Ma_RoseD",
	"SM_Ma_ShelfB",
	"SM_Ma_ShelfC",
	"SM_Ma_Ship",
	"SM_Ma_ShipWave",
	"SM_Ma_ShipWaveTale2",
	"SM_Ma_SineA",
	"SM_Ma_StairsA02",
	"SM_Ma_StairsB01",
	"SM_Ma_StairsF",
	"SM_Ma_Statue",
	"SM_Ma_Stepladder",
	"SM_Ma_Table",
	"SM_Ma_Tent02",
	"SM_Ma_Vase",
	"SM_Ma_VaseA",
	"SM_Ma_VaseB",
	"SM_Ma_VaseBreak",
	"SM_Ma_WallAShadow",
	"SM_Ma_WallA00",
	"SM_Ma_WallA01",
	"SM_Ma_SineA02",
	"SM_Ma_Wheel",
	"SM_Ma_WineBottleA",
	"SM_Ma_WineBottleB",
	"SM_Ma_WineCooler",
	"SM_Ma_WoodBoxA",
	"SM_Ma_WoodBoxB",
	"SM_Ma_WoodBoxC",
	"SM_Ma_WoodenPillar",
	"SM_Ma_WordSineA",
	"SM_Ma_WordSineA2",
	"SM_Ma_Sky",
    "SM_Ma_Sineboard",
    "SM_Ma_SineB02b",
    "SM_Ma_SineB07b",
    "SM_Ma_SineB11b",
    "SM_Ma_SineC01",
    "SM_Ma_SineB10b",
    "SM_Ma_SineC07",
    "SM_Ma_FlowerC",

    "SM_Dam_BushCorn",
    "SM_Dam_Sky",
    "SM_Dam_Arcade01",
    "SM_Dam_Arcade03",
    "SM_Dam_Around",
    "SM_Dam_AroundCornerC",
    "SM_Dam_AroundCornerL",
    "SM_Dam_AroundCornerS",
    "SM_Dam_AroundFountain",
    "SM_Dam_CastleFlying",
    "SM_Dam_CastleRoof",
    "SM_Dam_CastleWindowUnder",
    "SM_Dam_CastleWindowUp",
    "SM_Dam_Chain",
    "SM_Dam_Coach",
    "SM_Dam_Coniferous",
    "SM_Dam_DeerBaby",
    "SM_Dam_DeerBody",
    "SM_Dam_DeerPillar",
    "SM_Dam_DeerWater",
    "SM_Dam_DeerWingAll",
    "SM_Dam_DeerWingGlassFlame",
    "SM_Dam_ElectricLight",
    "SM_Dam_FarHouse01",
    "SM_Dam_FarHouse02",
    "SM_Dam_FarHouse03",
    "SM_Dam_FarHouse04",
    "SM_Dam_FenceA",
    "SM_Dam_FenceB",
    "SM_Dam_FenceCornerL",
    "SM_Dam_FenceCornerS",
    "SM_Dam_FenceCupA",
    "SM_Dam_FenceCupB",
    "SM_Dam_FenceDeco",
    "SM_Dam_FenceDecoCurve",
    "SM_Dam_FencePoint",
    "SM_Dam_Flag",
    "SM_Dam_FloorA",
    "SM_Dam_FloorB",
    "SM_Dam_FloorCornerA",
    "SM_Dam_FloorCornerB",
    "SM_Dam_FlowerAmaranth",
    "SM_Dam_FlowerSalvia",
    "SM_Dam_FrontLow",
    "SM_Dam_GardenStep",
    "SM_Dam_Goddess",
    "SM_Dam_Joint01",
    "SM_Dam_Joint02",
    "SM_Dam_Joint05",
    "SM_Dam_Lily",
    "SM_Dam_RopeStand",
    "SM_Dam_Stairs",
    "SM_Dam_StairsCup",
    "SM_Dam_StationA",
    "SM_Dam_StationB",
    "SM_Dam_StationC",
    "SM_Dam_StationD",
    "SM_Dam_StationF",
    "SM_Dam_StationG",
    "SM_Dam_SteepleLow",
    "SM_Dam_SteepleLowPillar",
    "SM_Dam_SteepleLowWall",
    "SM_Dam_SteepleUp",
    "SM_Dam_SupportParts01",
    "SM_Dam_UnderPillar",
    "SM_Dam_UnderPillarDeco",
    "SM_Dam_WingGround",
    "SM_Dam_FlowerRoseMix",

    "SM_DK_Sky",
    "SM_DK_BigWood",
    "SM_DK_BridgeArea0",
    "SM_DK_DemonBody01",
    "SM_DK_DemonBody02",
    "SM_DK_DemonHeadA",
    "SM_DK_DemonHeadB",
    "SM_DK_DemonSickle",
    "SM_DK_Gate01",
    "SM_DK_Gate02",
    "SM_DK_Gate03",
    "SM_DK_Gate04",
    "SM_DK_GateDifferent",
    "SM_DK_GothicBuilding01",
    "SM_DK_GothicBuilding02",
    "SM_DK_GothicBuilding03",
    "SM_DK_GothicBuilding04",
    "SM_DK_GothicBuilding04L",
    "SM_DK_GothicBuildingArea1",
    "SM_DK_GothicWall",
    "SM_DK_GothicWall01",
    "SM_DK_GothicWallBuilding",
    "SM_DK_GothicWallBuilding01",
    "SM_DK_GothicWallBuilding01Far",
    "SM_DK_GothicWallBuildingFar",
    "SM_DK_MainDemon01",
    "SM_DK_MainGate",
    "SM_DK_ScaffoldArea1",
    "SM_DK_TreeDead",

    "SM_Mob_Jans",
    "SM_Mob_Engine",
    "SM_Mob_Swmn",
    "SM_Mob_Cat",

    "SM_Ma_FloorBrickA",
    "SM_Ma_FloorBrickB",
    "SM_Ma_FenceAA",
    "SM_Ma_FenceAB",
    "SM_Ma_Water",
    "SM_Ma_RockWood",
    "SM_Ma_FloorAA",
    "SM_Ma_FloorBB",

    "SM_Dam_FloorPlane",

    "SM_Sn_CorridorC",
    "SM_Sn_RobotAn",
    "SM_Sn_Fence",

    "SM_DK_BridgeShort",
    "SM_DK_GothicBuildingClear",

    "SM_Mob_POTHead",

    "SM_TAG_END"
};

const char* pStaticlayer_Editer[] = {
	"Layer_Terrain",
	"Layer_Map_Static",
	"Layer_Map_Dynamic",
	"Layer_Environment",
	"Layer_Deco",
	"Layer_Place",
	"Layer_Wall",
	"STATIC_END",

	"Layer_Player",
	"Layer_Monster",
	"Layer_Boss",
	"Layer_NPC",
	"Layer_Effect",
	"Layer_Camera",
	"Layer_UI",
	"DYNAMIC_LAYER",
	"LAYER_END"
};
#endif
//IMGUI========================================================================================================================================================================================


CLevel_PJE::CLevel_PJE(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
    : CLevel(pDevice, pDeviceContext)
{
}

HRESULT CLevel_PJE::NativeConstruct()
{
    if (FAILED(__super::NativeConstruct()))
        return E_FAIL;

    if (FAILED(Ready_Layer_Camera(g_pLayerTags[DYNAMIC_LAYER_CAMERA])))
        return E_FAIL;

    DEBUG_BREAK(Ready_Terrain());

    DEBUG_BREAK(Ready_ObjectPrototype());

#if defined(USE_IMGUI)
    if (FAILED(SetUp_Editer_Window()))
        return E_FAIL;
#endif

    return S_OK;
}

_int CLevel_PJE::Tick(_double TimeDelta)
{
	//	레벨생성시 한번만 세팅할 것들 작성
    if (m_bOnceSetUp == false)
    {
        Ready_Lights();

        /* 셰이더 매니저 레벨 초기화 */
        CShader_Manager* pShader_Manager = GET_INSTANCE(CShader_Manager);
        pShader_Manager->NativeConstruct_Level((LEVEL)m_iLevelIdx, MAP_TYPE_TEST);
        RELEASE_INSTANCE(CShader_Manager);

        m_bOnceSetUp = true;
    }

    if (0 > __super::Tick(TimeDelta))
        return -1;

    /* 셰이더 매니저 Tick */
    CShader_Manager* pShader_Manager = GET_INSTANCE(CShader_Manager);
    if (0 > pShader_Manager->Tick(TimeDelta))
        return -1;
    RELEASE_INSTANCE(CShader_Manager);

    //IMGUI========================================================================================================================================================================================
#if defined(USE_IMGUI)
    Running_Editer_Window(TimeDelta);
#endif
    //IMGUI========================================================================================================================================================================================


    return 0;
}

_int CLevel_PJE::LateTick(_double TimeDelta)
{
    if (0 > __super::LateTick(TimeDelta))
        return -1;

    /* 셰이더 매니저 LateTick */
    CShader_Manager* pShader_Manager = GET_INSTANCE(CShader_Manager);
    if (0 > pShader_Manager->LateTick(TimeDelta))
        return -1;
    RELEASE_INSTANCE(CShader_Manager);

    return 0;
}

HRESULT CLevel_PJE::Render()
{
    if (FAILED(__super::Render()))
        return E_FAIL;
    return S_OK;
}

HRESULT CLevel_PJE::Ready_Lights()
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//	1. 전역조명 추가
    LIGHTDESC tagLightDesc;
    ZeroMemory(&tagLightDesc, sizeof(LIGHTDESC));
    XMStoreFloat4(&tagLightDesc.vDirection, XMVector3Normalize(XMVectorSet(-0.5f, -0.5f, 1.f, 0.f)));
    tagLightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
    tagLightDesc.vDiffuse = _float4(255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1.f);
    tagLightDesc.vSpecular = _float4(0.3f, 0.3f, 0.3f, 1.f);
    tagLightDesc.bIsDead = false;
    tagLightDesc.eLightType = LIGHTDESC::TYPE_DIRECTIONAL;
    tagLightDesc.fIntensity = 0.1f;
    tagLightDesc.fMaxIntensity = 0.1f;
    pGameInstance->Add_Light(CLight_Manager::LIGHTCHANNEL_WORLD, CStatic_Light::Create(m_pDevice, m_pDeviceContext, tagLightDesc));

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

HRESULT CLevel_PJE::Ready_ObjectPrototype()
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

HRESULT CLevel_PJE::Ready_Terrain()
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//	맵툴용 터레인 추가
    DEBUG_BREAK(pGameInstance->Add_GameObject(
        LEVEL_PJE, 
        g_pLayerTags[STATIC_LAYER_TERRAIN], 
        TEXT("Prototype_GameObject_Terrain_For_Editer")));

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

HRESULT CLevel_PJE::Ready_Layer_Camera(const _tchar* pLayerTag)
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    //	사본객체를 생성한다
    CCamera::CAMERADESC		CameraDesc;
    ZeroMemory(&CameraDesc, sizeof(CameraDesc));

    CameraDesc.vEye = _float3(0.f, 10.f, -7.f);
    CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
    CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);

    CameraDesc.fFovy = XMConvertToRadians(30.f);
    CameraDesc.fAspect = _float(g_iWinCX) / g_iWinCY;
    CameraDesc.fNear = 0.2f;
    CameraDesc.fFar = 500.f;

    CameraDesc.TransformDesc.fSpeedPerSec = 10.f;
    CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

    if (FAILED(pGameInstance->Add_GameObject(LEVEL_PJE, pLayerTag, TEXT("Prototype_GameObject_Camera_Dynamic"), &CameraDesc)))
        return E_FAIL;

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

CLevel_PJE* CLevel_PJE::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
    CLevel_PJE* pInstance = new CLevel_PJE(pDevice, pDeviceContext);

    if (FAILED(pInstance->NativeConstruct()))
    {
        MSGBOX("Failed to Creating CLevel_PJE");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CLevel_PJE::Free()
{
    __super::Free();
}


//IMGUI========================================================================================================================================================================================

#if defined(USE_IMGUI)

HRESULT CLevel_PJE::SetUp_Editer_Window()
{
	//	1. 윈도우창 키기
    m_bShow_Editer_Window = true;

	//	2. 오브젝트 불러오기
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
    m_pEditer_Prototypes = pGameInstance->Get_Prototypes();
    m_pEditer_Objects = pGameInstance->Get_Layers();
    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

void CLevel_PJE::Running_Editer_Window(_double TimeDelta)
{
	//	Baisc 윈도우
    if (m_bShow_Editer_Window)
    {
        CImGui_Manager* pImguiManager = GET_INSTANCE(CImGui_Manager);
		//	윈도우 시작
        ImGui::Begin("Editer Window");
        Widget_Add_Object();
        pImguiManager->Small_Separator();
        Widget_Save_Load_Objects();
        pImguiManager->Small_Separator();

		//	창에 필요한 기능을 다 넣었다면 End 로 닫아줘야함
        ImGui::End();
        RELEASE_INSTANCE(CImGui_Manager);
    }
}

HRESULT CLevel_PJE::Add_Static_Object(const _tchar* _pPrototypeTag, _uint _iLayerType, MODEL _eModelIndex)
{
	//	스태틱 레이어보다 인덱스가 클경우 예외처리
    if (_iLayerType >= STATIC_LAYER_END ||
        _iLayerType < 0)
        return E_FAIL;

    const _tchar* pLayertag = g_pLayerTags[_iLayerType];
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
    //	Static
    OBJECTDESC temp;
    temp.iLayerNumber = _iLayerType;
    temp.iModelNumber = _eModelIndex;
    XMStoreFloat4x4(&temp.matTransformMatrix, XMMatrixIdentity());

    if (FAILED(pGameInstance->Add_GameObject(
        LEVEL_PJE,
        pLayertag,
        _pPrototypeTag,
        &temp)))
        return E_FAIL;

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

HRESULT CLevel_PJE::Add_Object(_bool _bIsStatic, OBJECTDESC _tagObj)
{
    if (_bIsStatic)
    {
        CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

        if (FAILED(pGameInstance->Add_GameObject(
            LEVEL_PJE,
            g_pLayerTags[_tagObj.iLayerNumber],
            TEXT("Prototype_GameObject_StaticObject"),
            &_tagObj)))
            return E_FAIL;
        RELEASE_INSTANCE(CGameInstance);

    }
    else
    {

    }

    return S_OK;
}

void CLevel_PJE::Widget_Save_Load_Objects()
{
    CImGui_Manager* pImguiManager = GET_INSTANCE(CImGui_Manager);
    ImGui::Text("Save_Objects");

    //	패스입력
    static char Inputtxt[128] = "Input_Object_FileName";
    ImGui::InputText("SaveObjectFileName", Inputtxt, IM_ARRAYSIZE(Inputtxt));

    //	세이브 버튼
    if (ImGui::Button("Save_Object_File"))
    {
		//	1. 파일을 저장할 경로를 완성한다
        _tchar		szDefaultPath[] = TEXT("../Bin/Save/");
        _tchar		szHeightFullPath[MAX_PATH] = TEXT("");
        _tchar		szFilterFullPath[MAX_PATH] = TEXT("");

        wcscpy_s(szHeightFullPath, szDefaultPath);
        wcscat_s(szHeightFullPath, _bstr_t(Inputtxt));
        wcscat_s(szHeightFullPath, TEXT(".bin"));

		//	2. 파일개방
        HANDLE hFile = CreateFile(
            szHeightFullPath,
            GENERIC_WRITE,
            0,
            0,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            nullptr);

        if (hFile == INVALID_HANDLE_VALUE)
        {
            MSGBOX("INVALID_HANDLE_VALUE");
            return;
        }

		//	3. 파일 내용 입력
        DWORD dwBytes = 0;
        CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
        map<const _tchar*, class CLayer*>* pLayers = pGameInstance->Get_Layers();

		//	4. 지정한 범위의 레이어 저장
        for (_uint i = STATIC_LAYER_ENVIRONMENT; STATIC_LAYER_END > i; ++i)
        {
            const _tchar* pLayerTag = g_pLayerTags[i];
            CLayer* pLayer = pGameInstance->Find_Layer(LEVEL_PJE, pLayerTag);

			//	5. 선택한 레이어가 존재하지 않을경우 스킵
            if (pLayer == nullptr)
                continue;

            for (auto& pObject : pLayer->Get_Objects())
            {
                OBJECTDESC temp = pObject->Get_Save_Data();
                WriteFile(
                    hFile,
                    &temp,
                    sizeof(OBJECTDESC),
                    &dwBytes,
                    nullptr);
            }
        }

        RELEASE_INSTANCE(CGameInstance);
        CloseHandle(hFile);

        strcpy_s(Inputtxt, "Input_Object_FileName");
    }

	//	로드 버튼
    if (ImGui::Button("Load_Object_File"))
    {
        CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		//	0. 현재 에디터의 오브젝트들을 제거한다
        pGameInstance->Clear_LevelObject_For_Editer(LEVEL_PJE);

		//	1. 파일을 저장할 경로를완성한다
        _tchar		szDefaultPath[] = TEXT("../Bin/Save/");
        _tchar		szHeightFullPath[MAX_PATH] = TEXT("");
        _tchar		szFilterFullPath[MAX_PATH] = TEXT("");

        wcscpy_s(szHeightFullPath, szDefaultPath);
        wcscat_s(szHeightFullPath, _bstr_t(Inputtxt));
        wcscat_s(szHeightFullPath, TEXT(".bin"));

		//	2. 파일개방
        HANDLE hFile = CreateFile(
            szHeightFullPath,
            GENERIC_READ,
            0,
            nullptr,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            0);

        if (hFile == INVALID_HANDLE_VALUE)
        {
            MSGBOX("INVALID_HANDLE_VALUE");
            return;
        }

		//	3. 파일 읽기
        DWORD dwBytes = 0;

        while (true)
        {
            _bool bResult = false;

			//	4. 파일정보를 받을 구조체
            OBJECTDESC temp;
            ZeroMemory(&temp, sizeof(OBJECTDESC));
            bResult = ReadFile(
                hFile,
                &temp,
                sizeof(OBJECTDESC),
                &dwBytes,
                nullptr);

			//	5. 레이어 분류
            //	StaticLayer
            if (temp.iLayerNumber >= STATIC_LAYER_ENVIRONMENT &&
                temp.iLayerNumber < STATIC_LAYER_END)
            {
				//	6. 정적 오브젝트 추가
                if (FAILED(pGameInstance->Add_GameObject(
                    LEVEL_PJE,
                    g_pLayerTags[temp.iLayerNumber],
					TEXT("Prototype_GameObject_StaticObject"),
                    &temp)))
                {
                    MSGBOX("Load_Object_Failed");
                    return;
                }
            }
           

            if (bResult && dwBytes == 0)
                break;
        }


        RELEASE_INSTANCE(CGameInstance);
        CloseHandle(hFile);
        strcpy_s(Inputtxt, "Input_Object_FileName");
    }

    RELEASE_INSTANCE(CImGui_Manager);
}

void CLevel_PJE::Widget_Add_Object()
{
    if (ImGui::Button("Add_Static_Object"))
    {
        ImGui::OpenPopup("AddStaticObject 1");
    }
    if (ImGui::BeginPopupModal("AddStaticObject 1", NULL, ImGuiWindowFlags_MenuBar))
    {
        ImGui::Text("Static Object Desc");
        ImGui::Combo("Layer_List", &iPickLayer, pStaticlayer_Editer, IM_ARRAYSIZE(pStaticlayer_Editer));
        ImGui::Combo("Model_List", &iPickModel, staticModel, IM_ARRAYSIZE(staticModel));

        if (ImGui::Button("Close"))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
    if (pGameInstance->Key_Pressing(DIK_Q) && pGameInstance->Mouse_Down(CInput_Device::MBS_LBUTTON))
    {
        OBJECTDESC temp;
        _bool bIsStatic = true;
        ZeroMemory(&temp, sizeof(OBJECTDESC));
        temp.iLayerNumber = iPickLayer;
        temp.iModelNumber = iPickModel;
        XMStoreFloat4x4(&temp.matTransformMatrix, XMMatrixIdentity());

        //	터레인 가져오기
        CVIBuffer_Terrain_Dynamic* pTerrainBuffer = (CVIBuffer_Terrain_Dynamic*)pGameInstance->Get_Component(
            LEVEL_PJE,
            g_pLayerTags[STATIC_LAYER_TERRAIN],
            L"Com_VIBuffer");

        if (pTerrainBuffer == nullptr)
        {
            MSGBOX("pTerrainBuffer is Nullptr");
            return;
        }

        CTransform* pTerrainTransform = (CTransform*)pGameInstance->Get_Component(
            LEVEL_PJE,
            g_pLayerTags[STATIC_LAYER_TERRAIN],
            L"Com_Transform");

        if (pTerrainTransform == nullptr)
        {
            MSGBOX("pTerrainTransform is Nullptr");
            return;
        }

        _float3 vPickedPos;
        if (pTerrainBuffer->Pick_Dynamic(pTerrainTransform->Get_WorldMatrixInverse_Float4x4(), &vPickedPos))
        {
            *(_float3*)&temp.matTransformMatrix._41 = vPickedPos;
            if (FAILED(Add_Object(bIsStatic, temp)))
                MSGBOX("Map Editer :: Object Add Failed");
        }
    }

    RELEASE_INSTANCE(CGameInstance);
}

#endif
//IMGUI========================================================================================================================================================================================