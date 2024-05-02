#include "stdafx.h"
#include "..\Public\Terrain_For_Editer.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "Light.h"
#include "Navigation_For_Editer.h"
#include "Cell_For_Editer.h"
#include "Line.h"
#include "Navigation_Line.h"

#if defined(USE_IMGUI)
#include "ImGui_Manager.h"

_bool MinX_Float3(const _float3 & src, const _float3 & dst)
{
	if (src.x < dst.x)
	{
		return true;
	}
	else if(src.x == dst.x)
	{
		if (src.z > dst.z)
		{
			return true;
		}
		else if(src.z == dst.z)
		{
			if(src.y < dst.y)
			{
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	else
	{
		return false;
	}
}

#endif

CTerrain_For_Editer::CTerrain_For_Editer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CTerrain_For_Editer::CTerrain_For_Editer(const CTerrain_For_Editer& rhs)
	: CGameObject(rhs) {
}

HRESULT CTerrain_For_Editer::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain_For_Editer::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct((CTransform::TRANSFORMDESC*)pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

#if defined(USE_IMGUI)
	//	초기값설정
	m_vAddValues = 5.f;

	m_vDampning[0] = 200.f;
	m_vDampning[1] = 1.f;
	m_vDampning[2] = 0.022f;
	m_vDampning[3] = 0.022f;

	SetUp_Navigation_Window();
#endif
	return S_OK;
}

_int CTerrain_For_Editer::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

#if defined(USE_IMGUI)
	if (m_bIsChanged)
	{
		static_cast<CVIBuffer_Terrain_Dynamic*>(m_pVIBufferCom)->Apply_Data();
		m_bIsChanged = false;
	}
#endif

#if defined(USE_IMGUI)
	Running_Window();
	Running_Navigation_Window(TimeDelta);
#endif

	return _int();
}

_int CTerrain_For_Editer::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_FIRST, this);

	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

HRESULT CTerrain_For_Editer::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render(m_pShaderCom, 0)))
		return E_FAIL;

#ifdef _DEBUG
	m_pRendererCom->Add_Debug(m_pNavigation);
	m_pRendererCom->Add_Debug(m_pNavigation_Line);
	//m_pNavigation->Render(m_pTransform);
#endif

	return S_OK;
}

HRESULT CTerrain_For_Editer::SetUp_Components()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	//	1. 게임인스턴스 준비
	_uint iCurLevel = pGameInstance->Get_CurrentLevelIndex();
	RELEASE_INSTANCE(CGameInstance);

	//	2. 예외처리
	if (iCurLevel == LEVEL_LOGO ||
		iCurLevel == LEVEL_MAIN_MENU)
	{
		MSGBOX("CState_Player_Turn Error : CurLevel is invailed");
		return E_FAIL;
	}

	//	3. 로딩 레벨의 경우
	if (iCurLevel == LEVEL_LOADING)
	{
		iCurLevel = dynamic_cast<CLevel_Loading*>(pGameInstance->Get_LoadingLevel())->Get_NextLevel();
	}

	/* For.Com_Renderer */
	DEBUG_BREAK(__super::Add_Component(
		LEVEL_STATIC, 
		TEXT("Prototype_Component_Renderer"), 
		TEXT("Com_Renderer"), 
		(CComponent**)&m_pRendererCom))

	/* For.Com_Shader */
	DEBUG_BREAK(__super::Add_Component(
		LEVEL_STATIC, 
		g_pShaderTag[Shader_VTXRGB_FOR_TERRAIN],
		TEXT("Com_Shader"), 
		(CComponent**)&m_pShaderCom))

	/* For.Com_VIBuffer */
	DEBUG_BREAK(__super::Add_Component(
		iCurLevel,
		TEXT("Prototype_Component_VIBuffer_Terrain_Dynamic"), 
		TEXT("Com_VIBuffer"), 
		(CComponent**)&m_pVIBufferCom))

	/* For.Com_Texture_D */
	DEBUG_BREAK(__super::Add_Component(
		LEVEL_STATIC,
		TEXT("Prototype_Component_Texture_Tile_D"), 
		TEXT("Com_Texture_D"), 
		(CComponent**)&m_pTextureCom[TEXTURETYPE_DIFFUSE]))

	/* For.Com_Texture_N */
	DEBUG_BREAK(__super::Add_Component(
		LEVEL_STATIC,
		TEXT("Prototype_Component_Texture_Tile_N"),
		TEXT("Com_Texture_N"),
		(CComponent**)&m_pTextureCom[TEXTURETYPE_NORMAL]))


	/* For.Com_Navigation */
	CNavigation::NAVIDESC tagNavi;
	ZeroMemory(&tagNavi, sizeof(CNavigation::NAVIDESC));
	tagNavi.iCurrentIndex = -1;
	DEBUG_BREAK(__super::Add_Component(
		iCurLevel,
		TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"),
		(CComponent**)&m_pNavigation,
		&tagNavi))

	/* For.Com_Navigation_Line */
	CNavigation_Line::NAVIDESC tagNavi_Line;
	ZeroMemory(&tagNavi_Line, sizeof(CNavigation_Line::NAVIDESC));
	tagNavi_Line.iCurrentIndex = -1;
	DEBUG_BREAK(__super::Add_Component(
		iCurLevel,
		TEXT("Prototype_Component_Navigation_Line"),
		TEXT("Com_Navigation_Line"),
		(CComponent**)&m_pNavigation_Line,
		&tagNavi_Line))

	return S_OK;

}

HRESULT CTerrain_For_Editer::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransform->Bind_OnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_vCamPosition", &pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	//	Diffuse
	if (FAILED(m_pTextureCom[TEXTURETYPE_DIFFUSE]->SetUp_OnShader(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;
	if (FAILED(m_pTextureCom[TEXTURETYPE_DIFFUSE]->SetUp_OnShader(m_pShaderCom, "g_SourDiffuseR", 1)))
		return E_FAIL;
	if (FAILED(m_pTextureCom[TEXTURETYPE_DIFFUSE]->SetUp_OnShader(m_pShaderCom, "g_SourDiffuseG", 2)))
		return E_FAIL;
	if (FAILED(m_pTextureCom[TEXTURETYPE_DIFFUSE]->SetUp_OnShader(m_pShaderCom, "g_SourDiffuseB", 3)))
		return E_FAIL;

	//	Normal
	if (FAILED(m_pTextureCom[TEXTURETYPE_NORMAL]->SetUp_OnShader(m_pShaderCom, "g_NormalTexture",0)))
		return E_FAIL;
	if (FAILED(m_pTextureCom[TEXTURETYPE_NORMAL]->SetUp_OnShader(m_pShaderCom, "g_SourNormalR",1)))
		return E_FAIL;
	if (FAILED(m_pTextureCom[TEXTURETYPE_NORMAL]->SetUp_OnShader(m_pShaderCom, "g_SourNormalG", 2)))
		return E_FAIL;
	if (FAILED(m_pTextureCom[TEXTURETYPE_NORMAL]->SetUp_OnShader(m_pShaderCom, "g_SourNormalB", 3)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;

}

CTerrain_For_Editer* CTerrain_For_Editer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CTerrain_For_Editer*	pInstance = new CTerrain_For_Editer(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CTerrain_For_Editer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CTerrain_For_Editer::Clone(void* pArg)
{
	CTerrain_For_Editer*	pInstance = new CTerrain_For_Editer(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CTerrain_For_Editer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTerrain_For_Editer::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pNavigation);
	for (_uint i = 0; TEXTURETYPE_END > i; ++i)
		Safe_Release(m_pTextureCom[i]);

	Safe_Release(m_pShaderCom);
}

//IMGUI========================================================================================================================================================================================
#if defined(USE_IMGUI)
static bool closable_group = true;

void CTerrain_For_Editer::Running_Window()
{

	//	1. 활성화 상태확인
	if (m_bShow_Window == false)
		return;

	++m_iFPS;
	if (m_iFPS > 101)
		m_iFPS = 0;
	
	//	2. 윈도우 기능추가
	static ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
	ImGui::Begin("Edit_Terrain_Height", 0, flags);
	CImGui_Manager* pImguiManager = GET_INSTANCE(CImGui_Manager);

	//	3. 높이맵 기능설정
	Set_Range_And_Dampning();
	pImguiManager->Small_Separator();
	Set_Value_Option();
	pImguiManager->Small_Separator();
	Set_Edit_Type_Option();
	pImguiManager->Small_Separator();
	Save_Map();
	pImguiManager->Small_Separator();
	Load_Map();
	ImGui::SameLine();
	ImGui::Checkbox("Edit_Navi", &m_bNaviWindow);
	RELEASE_INSTANCE(CImGui_Manager);
	ImGui::End();

	//	5. 수정사항적용
	Edit_Input();

}

void CTerrain_For_Editer::SetUp_Navigation_Window()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	m_pNavigationCells = m_pNavigation->Get_Cells();
	m_pLines = m_pNavigation_Line->Get_Lines();
	ZeroMemory(m_vPickedAddPos, sizeof(_float3) * 3);
	ZeroMemory(m_bPickedAddPosVailed, sizeof(_bool) * 3);
	RELEASE_INSTANCE(CGameInstance);
}

void CTerrain_For_Editer::Running_Navigation_Window(_double TimeDelta)
{
	if (m_bNaviWindow == false)
		return;

	//	2. 윈도우 기능추가
	
	static ImGuiWindowFlags flags =0;
	ImGui::Begin("Edit_Navigation_Window", 0, flags);

	CImGui_Manager* pImguiManager = GET_INSTANCE(CImGui_Manager);

	Save_Load_Navi();
	pImguiManager->Small_Separator();
	Widget_PickedPosition();
	pImguiManager->Small_Separator();
	Widget_PickedPoint();
	pImguiManager->Small_Separator();
	Widget_Display_CellList();
	Widget_Display_LineList();
	pImguiManager->Small_Separator();
	Widget_Display_Add_Points();
	pImguiManager->Small_Separator();

	RELEASE_INSTANCE(CImGui_Manager);

	ImGui::End();
}

void CTerrain_For_Editer::Widget_Display_CellList()
{
	if (ImGui::TreeNode("Cells"))
	{
		//	셀 트리
		auto iter = m_pNavigationCells->begin();

		//	2. 리스트의 크기만큼 트리구성
		for (int i = 0; iter !=  m_pNavigationCells->end(); i++)
		{
			char	szObjectTag[MAX_PATH] = "";
			sprintf_s(szObjectTag, "Cell");
			strcat_s(szObjectTag, "_%d");
			sprintf_s(szObjectTag, szObjectTag, i);

			ImGui::Text(szObjectTag);
			ImGui::SameLine();

			//	Slice
			sprintf_s(szObjectTag, "Slice_Cell_%d", i);
			if (ImGui::SmallButton(szObjectTag))
			{
				
			}

			ImGui::SameLine();

			//	Delete
			sprintf_s(szObjectTag, "Delete_Cell_%d",i);
			if (ImGui::SmallButton(szObjectTag))
			{
				iter = m_pNavigationCells->erase(iter);
				m_vPickedPoint = nullptr;
			}
			else
				++iter;
		}

		ImGui::TreePop();
	}

	if (ImGui::SmallButton("Compute"))
	{
		//	모든 셀들을 해당 좌표의 터레인의 높이로 맞춥니다.
		for (auto pCells : (*m_pNavigationCells))
		{
			for(_uint i=0; CCell_For_Editer::POINT_END > i; ++i)
			{
				pCells->Set_PointPosition((CCell_For_Editer::POINTS)i, m_pVIBufferCom->Get_Height(pCells->Get_Point((CCell_For_Editer::POINTS)i)) + XMVectorSet(0.f,0.2f,0.f,0.f));
			}
			
		}
	}
}

void CTerrain_For_Editer::Widget_Display_LineList()
{
	if (ImGui::TreeNode("Lines"))
	{
		//	셀 트리
		auto iter = m_pLines->begin();

		//	2. 리스트의 크기만큼 트리구성
		for (int i = 0; iter != m_pLines->end(); i++)
		{
			char	szObjectTag[MAX_PATH] = "";
			sprintf_s(szObjectTag, "Line");
			strcat_s(szObjectTag, "_%d");
			sprintf_s(szObjectTag, szObjectTag, i);

			ImGui::Text(szObjectTag);
			ImGui::SameLine();

			//	Delete
			sprintf_s(szObjectTag, "Delete_Line_%d", i);
			if (ImGui::SmallButton(szObjectTag))
			{
				iter = m_pLines->erase(iter);
				m_vPickedPoint = nullptr;
			}
			else
				++iter;
		}

		ImGui::TreePop();
	}
}

void CTerrain_For_Editer::Widget_PickedPoint()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if((pGameInstance->Key_Down(DIK_P)))
	{
		//	포인트가 있을경우
		_float		fCurDis = 1000.f;
		_float3*	pPickedPoint = nullptr;
		for (auto pCells : (*m_pNavigationCells))
		{
			_float		Dis = 0.f;
			_float3*	ptemp = nullptr;
			if (ptemp = pCells->Check_Picked_Point(Dis))
			{
				if (Dis < fCurDis)
				{
					fCurDis = Dis;
					pPickedPoint = ptemp;
				}
			}
		}

		for (auto pLines : (*m_pLines))
		{
			_float		Dis = 0.f;
			_float3* ptemp = nullptr;
			if (ptemp = pLines->Check_Picked_Point(Dis))
			{
				if (Dis < fCurDis)
				{
					fCurDis = Dis;
					pPickedPoint = ptemp;
				}
			}
		}

		m_vPickedPoint = pPickedPoint;

		//	ctrl 누르고 클릭할경우 애드 포인트에 값설정
		if(pGameInstance->Key_Pressing(DIK_SPACE))
		{
			if(m_vPickedPoint != nullptr)
			{
				//	위에서 부터 채운다
				if (m_bPickedAddPosVailed[0] == false)
				{
					m_vPickedAddPos[0] = (*m_vPickedPoint);
					m_bPickedAddPosVailed[0] = true;
				}
				else if (m_bPickedAddPosVailed[1] == false)
				{
					m_vPickedAddPos[1] = (*m_vPickedPoint);
					m_bPickedAddPosVailed[1] = true;
				}
				else if (m_bPickedAddPosVailed[2] == false)
				{
					m_vPickedAddPos[2] = (*m_vPickedPoint);
					m_bPickedAddPosVailed[2] = true;
				}
				else
					MSGBOX("Add Point Full");

			}
		}
	
	}
	RELEASE_INSTANCE(CGameInstance);
	
	if(m_vPickedPoint == nullptr)
	{
		ImGui::BulletText(" Picked Point : NONE");
	}
	else
	{
		ImGui::BulletText(
			" Picked Point : (%.1f,%.1f,%.1f)",
			m_vPickedPoint->x,
			m_vPickedPoint->y,
			m_vPickedPoint->z);

		ImGui::DragFloat("X : ", &m_vPickedPoint->x, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None);
		ImGui::DragFloat("Y : ", &m_vPickedPoint->y, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None);
		ImGui::DragFloat("Z : ", &m_vPickedPoint->z, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None);

		if (ImGui::SmallButton("Add_Picked_Point"))
		{
			if (m_vPickedPoint != nullptr)
			{
				//	위에서 부터 채운다
				if (m_bPickedAddPosVailed[0] == false)
				{
					m_vPickedAddPos[0] = (*m_vPickedPoint);
					m_bPickedAddPosVailed[0] = true;
				}
				else if (m_bPickedAddPosVailed[1] == false)
				{
					m_vPickedAddPos[1] = (*m_vPickedPoint);
					m_bPickedAddPosVailed[1] = true;
				}
				else if (m_bPickedAddPosVailed[2] == false)
				{
					m_vPickedAddPos[2] = (*m_vPickedPoint);
					m_bPickedAddPosVailed[2] = true;
				}
				else
					MSGBOX("Add Point Full");
			}
		}

	}
}

void CTerrain_For_Editer::Widget_PickedPosition()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if ((pGameInstance->Key_Down(DIK_O)))
	{
		m_pVIBufferCom->Pick_Dynamic(m_pTransform->Get_WorldMatrixInverse_Float4x4(), &m_vPickedPos);

		//	ctrl 누르고 클릭할경우 애드 포인트에 값설정
		if (pGameInstance->Key_Pressing(DIK_SPACE))
		{
			//	위에서 부터 채운다
			if (m_bPickedAddPosVailed[0] == false)
			{
				m_vPickedAddPos[0] = m_vPickedPos;
				m_bPickedAddPosVailed[0] = true;
			}
			else if (m_bPickedAddPosVailed[1] == false)
			{
				m_vPickedAddPos[1] = m_vPickedPos;
				m_bPickedAddPosVailed[1] = true;
			}
			else if (m_bPickedAddPosVailed[2] == false)
			{
				m_vPickedAddPos[2] = m_vPickedPos;
				m_bPickedAddPosVailed[2] = true;
			}
			else
				MSGBOX("Add Point Full");

		}

	}
	RELEASE_INSTANCE(CGameInstance);

	ImGui::BulletText(
		" Picked Pos : (%.1f,%.1f,%.1f)",
		m_vPickedPos.x,
		m_vPickedPos.y,
		m_vPickedPos.z);

	ImGui::SameLine();
	if (ImGui::SmallButton("Add_Picked_Pos"))
	{
		if (m_bPickedAddPosVailed[0] == false)
		{
			m_vPickedAddPos[0] = m_vPickedPos;
			m_bPickedAddPosVailed[0] = true;
		}
		else if (m_bPickedAddPosVailed[1] == false)
		{
			m_vPickedAddPos[1] = m_vPickedPos;
			m_bPickedAddPosVailed[1] = true;
		}
		else if (m_bPickedAddPosVailed[2] == false)
		{
			m_vPickedAddPos[2] = m_vPickedPos;
			m_bPickedAddPosVailed[2] = true;
		}
		else
			MSGBOX("Add Point Full");
	}

}


void CTerrain_For_Editer::Widget_Display_Point()
{
}

void CTerrain_For_Editer::Widget_Display_Add_Points()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//	1번 누르고 클릭하면 첫번째 점

	//PointA==================================================================================================
	if (m_bPickedAddPosVailed[0] == true)
	{
		ImGui::BulletText(
			" Add Pos A : (%.1f,%.1f,%.1f)",
			m_vPickedAddPos[0].x,
			m_vPickedAddPos[0].y,
			m_vPickedAddPos[0].z);
	}
	else
	{
		ImGui::BulletText(
			" Add Pos A : NONE"
		);
	}

	ImGui::SameLine();

	if (ImGui::SmallButton("Delete_A"))
	{
		m_vPickedAddPos[0] = { 0.f,0.f,0.f };
		m_bPickedAddPosVailed[0] = false;
	}

	//PointB==================================================================================================
	if (m_bPickedAddPosVailed[1] == true)
	{
		ImGui::BulletText(
			" Add Pos B : (%.1f,%.1f,%.1f)",
			m_vPickedAddPos[1].x,
			m_vPickedAddPos[1].y,
			m_vPickedAddPos[1].z);
	}
	else
	{
		ImGui::BulletText(
			" Add Pos B : NONE"
		);
	}

	ImGui::SameLine();
	if (ImGui::SmallButton("Delete_B"))
	{
		m_vPickedAddPos[1] = { 0.f,0.f,0.f };
		m_bPickedAddPosVailed[1] = false;
	}

	//PointC==================================================================================================
	if (m_bPickedAddPosVailed[2] == true)
	{
		ImGui::BulletText(
			" Add Pos C : (%.1f,%.1f,%.1f)",
			m_vPickedAddPos[2].x,
			m_vPickedAddPos[2].y,
			m_vPickedAddPos[2].z);
	}
	else
	{
		ImGui::BulletText(
			" Add Pos C : NONE"
		);
	}

	ImGui::SameLine();
	if (ImGui::SmallButton("Delete_C"))
	{
		m_vPickedAddPos[2] = { 0.f,0.f,0.f };
		m_bPickedAddPosVailed[2] = false;
	}

	ImGui::BulletText(
		" Line_Type"
	);
	ImGui::RadioButton("Straight", &m_iLineType, 0); ImGui::SameLine();
	ImGui::RadioButton("Curve", &m_iLineType, 1); 

	//Add_Cell==================================================================================================
	if (ImGui::Button("Add_Cell"))
	{
		//	3점이 모두 차있을경우
		if (m_bPickedAddPosVailed[0] == true &&
			m_bPickedAddPosVailed[1] == true &&
			m_bPickedAddPosVailed[2] == true)
		{

			//	삽입할 점 3개를 잠시 담을 지역변수(위치 등이 고려되어 들어갈것이다)
			_float3 vPoints[3];
			memcpy(vPoints, m_vPickedAddPos, sizeof(_float3) * 3);

			//	정렬 -> 함수포인터(X 값에따라 정렬한다)
			sort(vPoints, vPoints + 3, MinX_Float3);

			//	점들의 위치 판단
			_vector	vDestDir = XMVector3Normalize(XMLoadFloat3(&vPoints[1]) - XMLoadFloat3(&vPoints[0]));
			_vector vSourDir = XMLoadFloat3(&vPoints[2]) - XMLoadFloat3(&vPoints[0]);
			_vector vSourNormal = XMVector3Normalize(XMLoadFloat4(&_float4(XMVectorGetZ(vSourDir) * -1.f, 0.0f, XMVectorGetX(vSourDir), 0.f)));

			//	두번째 점이 아래쪽일경우 순서를 변경한다(점 0 에서 점1로의 방향벡터를 점0->점2의 수선 벡터와 내적값이 0보다 작을 경우 아래로 판단)
			if (0 > XMVectorGetX(XMVector3Dot(vDestDir, vSourNormal)))
			{
				_float3 temp = vPoints[1];
				vPoints[1] = vPoints[2];
				vPoints[2] = temp;
			}

			CCell_For_Editer* pCell = CCell_For_Editer::Create(m_pDevice, m_pDeviceContext, vPoints, (_uint)m_pNavigationCells->size());

			if (nullptr == pCell)
			{
				MSGBOX("Add Cell Failed");
			}

			m_pNavigationCells->push_back(pCell);
			m_pNavigation->SetUp_Neighbor();

			for (_uint i = 0; 3 > i; ++i)
			{
				m_vPickedAddPos[i] = { 0.f,0.f,0.f };
				m_bPickedAddPosVailed[i] = false;
			}

		}
		else
		{
			MSGBOX("Add Cell Failed : Points are not Full");
		}
	}

	//Add_Line==================================================================================================
	if (ImGui::Button("Add_Line"))
	{
		//	3점이 모두 차있을경우
		if (m_bPickedAddPosVailed[0] == true &&
			m_bPickedAddPosVailed[1] == true &&
			m_bPickedAddPosVailed[2] == true)
		{

			//삽입할 점 3개를 잠시 담을 지역변수(위치 등이 고려되어 들어갈것이다)
			_float3 vPoints[3];
			memcpy(vPoints, m_vPickedAddPos, sizeof(_float3) * 3);

			////정렬 -> 함수포인터(X 값에따라 정렬한다)
			//sort(vPoints, vPoints + 3, MinX_Float3);

			////점들의 위치 판단
			//_vector	vDestDir = XMVector3Normalize(XMLoadFloat3(&vPoints[1]) - XMLoadFloat3(&vPoints[0]));
			//_vector vSourDir = XMLoadFloat3(&vPoints[2]) - XMLoadFloat3(&vPoints[0]);
			//_vector vSourNormal = XMVector3Normalize(XMLoadFloat4(&_float4(XMVectorGetZ(vSourDir) * -1.f, 0.0f, XMVectorGetX(vSourDir), 0.f)));

			////두번째 점이 아래쪽일경우 순서를 변경한다(점 0 에서 점1로의 방향벡터를 점0->점2의 수선 벡터와 내적값이 0보다 작을 경우 아래로 판단)
			//if (0 > XMVectorGetX(XMVector3Dot(vDestDir, vSourNormal)))
			//{
			//	_float3 temp = vPoints[1];
			//	vPoints[1] = vPoints[2];
			//	vPoints[2] = temp;
			//}

			CLine* pLine = nullptr;
			switch (m_iLineType)
			{
			case CLine::LINE_TYPE_STRAIGHT:
				pLine = CLine::Create(m_pDevice, m_pDeviceContext, vPoints, (_uint)m_pNavigationCells->size(), m_iLineType);
				break;
			case CLine::LINE_TYPE_CURVE:
				pLine = CLine::Create(m_pDevice, m_pDeviceContext, vPoints, (_uint)m_pNavigationCells->size(), m_iLineType);
				break;

			default:
				break;
			}

			if (nullptr == pLine)
			{
				MSGBOX("Add pLine Failed");
			}

			m_pLines->push_back(pLine);
			m_pNavigation_Line->SetUp_Neighbor();

			for (_uint i = 0; 3 > i; ++i)
			{
				m_vPickedAddPos[i] = { 0.f,0.f,0.f };
				m_bPickedAddPosVailed[i] = false;
			}

		}
		else
		{
			MSGBOX("Add Line Failed : Points are not Full");
		}

	}

	RELEASE_INSTANCE(CGameInstance);
}

void CTerrain_For_Editer::Key_Input_Navi()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if(pGameInstance->Key_Pressing(DIK_LCONTROL))
	{
		if(pGameInstance->Key_Down(DIK_Z))
		{
			m_pNavigationCells->pop_back();
			m_pNavigation->SetUp_Neighbor();
		}
	}
	RELEASE_INSTANCE(CGameInstance);
}

void CTerrain_For_Editer::Save_Load_Navi()
{
	CImGui_Manager* pImguiManager = GET_INSTANCE(CImGui_Manager);
	pImguiManager->Small_Separator();
	ImGui::Text("Save_Navi");

	//	패스입력
	static char NaviFileName[128] = "Input_Navi_FileName";
	ImGui::InputText("File_Name", NaviFileName, IM_ARRAYSIZE(NaviFileName));

#pragma region CELL
	//	세이브 버튼
	static int Navi_Save_clicked = 0;
	if (ImGui::Button("Save_Navi_File"))
		Navi_Save_clicked++;
	if (Navi_Save_clicked & 1)
	{
		//	2. 파일을 저장할 경로를완성한다
		_tchar		szDefaultPath[] = TEXT("../Bin/Save/Navi/");
		_tchar		szNaviPath[MAX_PATH] = TEXT("");
		wcscpy_s(szNaviPath, szDefaultPath);
		wcscat_s(szNaviPath, _bstr_t(NaviFileName));
		wcscat_s(szNaviPath, TEXT("_Navi.dat"));
		Navi_Save_clicked = 0;

		//	3. 네비 데이터 저장
		HANDLE hFile = CreateFile(
			szNaviPath,
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
		DWORD dwBytes = 0;

		for(_uint i=0; m_pNavigationCells->size() > i; ++i)
		{
			//	네비 정보 저장
			_vector ptA =  (*m_pNavigationCells)[i]->Get_Point(CCell_For_Editer::POINT_A);
			_vector ptB = (*m_pNavigationCells)[i]->Get_Point(CCell_For_Editer::POINT_B);
			_vector ptC = (*m_pNavigationCells)[i]->Get_Point(CCell_For_Editer::POINT_C);

			_float3 temp = {0.f,0.f,0.f};

			//	점 A 저장
			XMStoreFloat3(&temp, ptA);
			WriteFile(
				hFile,
				&temp,
				sizeof(_float3),
				&dwBytes,
				nullptr);

			//	점 B 저장
			temp = { 0.f,0.f,0.f };
			XMStoreFloat3(&temp, ptB);
			WriteFile(
				hFile,
				&temp,
				sizeof(_float3),
				&dwBytes,
				nullptr);

			//	점 C 저장
			temp = { 0.f,0.f,0.f };
			XMStoreFloat3(&temp, ptC);
			WriteFile(
				hFile,
				&temp,
				sizeof(_float3),
				&dwBytes,
				nullptr);
		}

		CloseHandle(hFile);
		strcpy_s(NaviFileName, "Input_Navi_FileName");
	}

	//	로드 버튼
	ImGui::SameLine();
	static int Navi_Load_clicked = 0;
	if (ImGui::Button("Load_Navi_File"))
		Navi_Load_clicked++;
	if (Navi_Load_clicked & 1)
	{
		Navi_Load_clicked = 0;
		//	2. 파일을 저장할 경로를완성한다
		_tchar		szDefaultPath[] = TEXT("../Bin/Save/Navi/");
		_tchar		szNaviPath[MAX_PATH] = TEXT("");
		wcscpy_s(szNaviPath, szDefaultPath);
		wcscat_s(szNaviPath, _bstr_t(NaviFileName));
		wcscat_s(szNaviPath, TEXT("_Navi.dat"));

		//	3. 네비 데이터 저장
		HANDLE hFile = CreateFile(
			szNaviPath,
			GENERIC_READ,
			0,
			nullptr,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			nullptr);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			MSGBOX("INVALID_HANDLE_VALUE");
			return;
		}

		DWORD dwBytes = 0;
		_float3			vPoints[3];

		//	기존 셀들 제거

		m_vPickedPoint = nullptr;	//	얕은 복사한 포인트 포인터 nullptr
		for(auto& pCell : (*m_pNavigationCells))
		{
			Safe_Release(pCell);
		}
		m_pNavigationCells->clear();
		m_pNavigationCells->resize(0);

		//	파일 읽어오기
		while (true)
		{
			//	파일에서 포인트 정보들을 받아온다 
			ReadFile(hFile, vPoints, sizeof(_float3) * 3, &dwBytes, nullptr);

			//	읽어온 데이터가 없을 경우 탈출
			if (0 == dwBytes)
				break;

			CCell_For_Editer*		pCell = CCell_For_Editer::Create(m_pDevice, m_pDeviceContext, vPoints, (_uint)m_pNavigationCells->size());

			if (nullptr == pCell)
			{
				MSGBOX("Create Load Cell Failed");
				return;
			}

			m_pNavigationCells->push_back(pCell);
		}
		CloseHandle(hFile);

		//	추가된 셀들 이웃 지정하기
		if (FAILED(m_pNavigation->SetUp_Neighbor()))
			return;
	}
#pragma endregion

#pragma region LINE
	//	세이브 버튼
	if (ImGui::Button("Save_Line_File"))
	{
		//	2. 파일을 저장할 경로를완성한다
		_tchar		szDefaultPath[] = TEXT("../Bin/Save/Line/");
		_tchar		szNaviPath[MAX_PATH] = TEXT("");
		wcscpy_s(szNaviPath, szDefaultPath);
		wcscat_s(szNaviPath, _bstr_t(NaviFileName));
		wcscat_s(szNaviPath, TEXT("_Line.dat"));

		//	3. 네비 데이터 저장
		HANDLE hFile = CreateFile(
			szNaviPath,
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
		DWORD dwBytes = 0;

		for (_uint i = 0; m_pLines->size() > i; ++i)
		{
			//	네비 정보 저장
			_vector ptA = (*m_pLines)[i]->Get_Point(CLine::POINT_START);
			_vector ptB = (*m_pLines)[i]->Get_Point(CLine::POINT_WAYPOINT);
			_vector ptC = (*m_pLines)[i]->Get_Point(CLine::POINT_DEST);

			_float3 temp = { 0.f,0.f,0.f };
			_uint	Type = (*m_pLines)[i]->Get_Type();
			//	점 A 저장
			XMStoreFloat3(&temp, ptA);
			WriteFile(
				hFile,
				&temp,
				sizeof(_float3),
				&dwBytes,
				nullptr);

			//	점 B 저장
			temp = { 0.f,0.f,0.f };
			XMStoreFloat3(&temp, ptB);
			WriteFile(
				hFile,
				&temp,
				sizeof(_float3),
				&dwBytes,
				nullptr);

			//	점 C 저장
			temp = { 0.f,0.f,0.f };
			XMStoreFloat3(&temp, ptC);
			WriteFile(
				hFile,
				&temp,
				sizeof(_float3),
				&dwBytes,
				nullptr);

			//	타입 저장
			WriteFile(
				hFile,
				&Type,
				sizeof(_int),
				&dwBytes,
				nullptr);
		}

		CloseHandle(hFile);
		strcpy_s(NaviFileName, "Input_FileName");
	}

	//	로드 버튼
	ImGui::SameLine();
	if (ImGui::Button("Load_Line_File"))
	{
		//	2. 파일을 저장할 경로를완성한다
		_tchar		szDefaultPath[] = TEXT("../Bin/Save/Line/");
		_tchar		szNaviPath[MAX_PATH] = TEXT("");
		wcscpy_s(szNaviPath, szDefaultPath);
		wcscat_s(szNaviPath, _bstr_t(NaviFileName));
		wcscat_s(szNaviPath, TEXT("_Line.dat"));

		//	3. 네비 데이터 저장
		HANDLE hFile = CreateFile(
			szNaviPath,
			GENERIC_READ,
			0,
			nullptr,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			nullptr);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			MSGBOX("INVALID_HANDLE_VALUE");
			return;
		}

		DWORD dwBytes = 0;
		_float3		vPoints[3];
		_int		Type=0;
		//	기존 셀들 제거

		m_vPickedPoint = nullptr;	//	얕은 복사한 포인트 포인터 nullptr
		for (auto& pLine : (*m_pLines))
		{
			Safe_Release(pLine);
		}
		m_pLines->clear();
		m_pLines->resize(0);

		//	파일 읽어오기
		while (true)
		{
			//	파일에서 포인트 정보들을 받아온다 
			ReadFile(hFile, vPoints, sizeof(_float3) * 3, &dwBytes, nullptr);
			ReadFile(hFile, &Type, sizeof(_int), &dwBytes, nullptr);

			//	읽어온 데이터가 없을 경우 탈출
			if (0 == dwBytes)
				break;

			CLine* pLine = CLine::Create(m_pDevice, m_pDeviceContext, vPoints, (_uint)m_pLines->size(), Type);

			if (nullptr == pLine)
			{
				MSGBOX("Create Load pLine Failed");
				return;
			}

			m_pLines->push_back(pLine);
		}
		CloseHandle(hFile);

		//	추가된 셀들 이웃 지정하기
		if (FAILED(m_pNavigation_Line->SetUp_Neighbor()))
			return;
	}
#pragma endregion

	RELEASE_INSTANCE(CImGui_Manager);
}


void CTerrain_For_Editer::Edit_Input()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_float4 vValues={0.f,0.f,0.f,0.f};
	
	switch (m_eEditMode)
	{
	case TERRAIN_EDIT_TYPE_HEIGHT:
		vValues.x = m_vAddValues;
		break;
	case TERRAIN_EDIT_TYPE_TEXTURE_01:
		vValues.y = m_vAddValues;
		break;
	case TERRAIN_EDIT_TYPE_TEXTURE_02:
		vValues.z = m_vAddValues;
		break;
	case TERRAIN_EDIT_TYPE_TEXTURE_03:
		vValues.w = m_vAddValues;
		break;
	}

	if (pGameInstance->Key_Down(DIK_NUMPAD9))
		m_bPressing = !m_bPressing;

	//if(pGameInstance->Key_Pressing(DIK_LCONTROL))
	//{
		if (m_bPressing)
		{
			if (pGameInstance->Mouse_Pressing(CInput_Device::MBS_LBUTTON) && (!(m_iFPS%5)))
			{
				_float3 vPickedPos;
				if (m_pVIBufferCom->Pick_Dynamic(m_pTransform->Get_WorldMatrixInverse_Float4x4(), &vPickedPos))
				{
					_float4 vTemp;
					vTemp.x = m_vDampning[0];
					vTemp.y = m_vDampning[1];
					vTemp.z = m_vDampning[2];
					vTemp.w = m_vDampning[3];
					m_bIsChanged = true;
					static_cast<CVIBuffer_Terrain_Dynamic*>(m_pVIBufferCom)->Add_Height(vPickedPos, vTemp, vValues, m_fMaxHeight, m_fMinHeight);
				}
			}

		}
		else
		{
			if (pGameInstance->Key_Down(DIK_LCONTROL))
			{
				_float3 vPickedPos;
				if (m_pVIBufferCom->Pick_Dynamic(m_pTransform->Get_WorldMatrixInverse_Float4x4(), &vPickedPos))
				{
					_float4 vTemp;
					vTemp.x = m_vDampning[0];
					vTemp.y = m_vDampning[1];
					vTemp.z = m_vDampning[2];
					vTemp.w = m_vDampning[3];
					m_bIsChanged = true;

					XMStoreFloat3(&vPickedPos, XMVector3TransformCoord(XMLoadFloat3(&vPickedPos), m_pTransform->Get_WorldMatrixInverse())) ;

					static_cast<CVIBuffer_Terrain_Dynamic*>(m_pVIBufferCom)->Add_Height(vPickedPos, vTemp, vValues, m_fMaxHeight, m_fMinHeight);
				}
			}

		}

	//}



	RELEASE_INSTANCE(CGameInstance);
}

void CTerrain_For_Editer::Set_Range_And_Dampning()
{
	CImGui_Manager* pImguiManager = GET_INSTANCE(CImGui_Manager);
	//	3. 최대 최소높이 조절
	pImguiManager->Small_Separator();
	ImGui::Text("Range&Dampening");
	ImGui::Text("Set High,Low");
	ImGui::DragFloatRange2(
		"Max,Min",
		&m_fMinHeight,
		&m_fMaxHeight,
		0.25f,
		-255.f,
		255.f,
		"Min: %.1f",
		"Max: %.1f",
		ImGuiSliderFlags_AlwaysClamp);
	ImGui::InputFloat4("Ran,Damp", m_vDampning);
	ImGui::DragFloat("dam1", &m_vDampning[1], 0.005f);
	ImGui::DragFloat("dam2", &m_vDampning[2], 0.05f);
	ImGui::DragFloat("dam3", &m_vDampning[3], 0.025f);

	float(*func)(void*, int) = NONG::Damp;
	ImGui::PlotLines(
		"Lines", 
		func, 
		&m_vDampning, 
		m_vDampning[0],
		0, 
		NULL,
		0.f, 1.f, ImVec2(0.f, 80.f));


	RELEASE_INSTANCE(CImGui_Manager);

}

void CTerrain_For_Editer::Set_Value_Option()
{
	CImGui_Manager* pImguiManager = GET_INSTANCE(CImGui_Manager);

	//	4. 감쇄값,범위 조절
	//ImGui::BulletText("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	pImguiManager->Small_Separator();
	ImGui::Text("Value_Option");
	ImGui::InputFloat("Add_Value", &m_vAddValues, 0.05f, 0, "%.3f");

	RELEASE_INSTANCE(CImGui_Manager);
}

void CTerrain_For_Editer::Set_Edit_Type_Option()
{
	CImGui_Manager* pImguiManager = GET_INSTANCE(CImGui_Manager);

	pImguiManager->Small_Separator();
	ImGui::Text("Edit_Type_Option");

	static int e = 0;
	ImGui::RadioButton("Height", &e, 0); ImGui::SameLine();
	ImGui::RadioButton("Tex1", &e, 1); ImGui::SameLine();
	ImGui::RadioButton("Tex2", &e, 2); ImGui::SameLine();
	ImGui::RadioButton("Tex3", &e, 3);

	m_eEditMode = e;

	RELEASE_INSTANCE(CImGui_Manager);

}

void CTerrain_For_Editer::Save_Map()
{
	CImGui_Manager* pImguiManager = GET_INSTANCE(CImGui_Manager);
	pImguiManager->Small_Separator();
	ImGui::Text("Save");

	//	패스입력
	static char str0[128] = "Input_FileName";
	ImGui::InputText("SaveFileName", str0, IM_ARRAYSIZE(str0));

	//	세이브 버튼
	static int clicked = 0;
	if (ImGui::Button("Save_File"))
		clicked++;
	if (clicked & 1)
	{
		clicked = 0;
		static_cast<CVIBuffer_Terrain_Dynamic*>(m_pVIBufferCom)->Create_Filter_Image(_bstr_t(str0));

		strcpy_s(str0, "Input_FileName");
	}

	RELEASE_INSTANCE(CImGui_Manager);
}

void CTerrain_For_Editer::Load_Map()
{
	CImGui_Manager* pImguiManager = GET_INSTANCE(CImGui_Manager);
	pImguiManager->Small_Separator();
	ImGui::Text("Load");

	//	패스입력
	static char LoadFileName[128] = "Load_FileName";
	ImGui::InputText("LoadFileName", LoadFileName, IM_ARRAYSIZE(LoadFileName));

	//	세이브 버튼
	static int clicked = 0;
	if (ImGui::Button("Load_File"))
		clicked++;
	if (clicked & 1)
	{
		clicked = 0;
		static_cast<CVIBuffer_Terrain_Dynamic*>(m_pVIBufferCom)->Load_Image(_bstr_t(LoadFileName));

		strcpy_s(LoadFileName, "Load_FileName");
	}

	RELEASE_INSTANCE(CImGui_Manager);
}

#endif
//IMGUI========================================================================================================================================================================================
