#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Texture.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Terrain_Dynamic;
class CNavigation_For_Editer;
class CNavigation_Line;
class CLine;
class CCell_For_Editer;
END

BEGIN(Client)

class CTerrain_For_Editer final : public CGameObject
{
public:
	typedef enum eTextureType
	{
		TEXTURETYPE_DIFFUSE,
		TEXTURETYPE_NORMAL,
		TEXTURETYPE_FILTER,
		TEXTURETYPE_END
	}TEXTURETYPE;

private:
	CTerrain_For_Editer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CTerrain_For_Editer(const CTerrain_For_Editer& rhs);
	virtual ~CTerrain_For_Editer() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

private:
	CShader*						m_pShaderCom = nullptr;
	CTexture*						m_pTextureCom[TEXTURETYPE_END];
	CRenderer*						m_pRendererCom = nullptr;
	CVIBuffer_Terrain_Dynamic*		m_pVIBufferCom = nullptr;
	CNavigation_For_Editer*			m_pNavigation = nullptr;
	CNavigation_Line*				m_pNavigation_Line = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();


public:
	static CTerrain_For_Editer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


#if defined(USE_IMGUI)
public:
	virtual void Running_Window()override;

	//	네비게이션 툴
private:
	void SetUp_Navigation_Window();
	void Running_Navigation_Window(_double TimeDelta);
	void Widget_Display_CellList();
	void Widget_Display_LineList();

	void Widget_PickedPoint();
	void Widget_PickedPosition();
	void Widget_Display_Point();
	void Widget_Display_Add_Points();
	void Key_Input_Navi();
	void Save_Load_Navi();

private:
	vector<CCell_For_Editer*>* m_pNavigationCells = nullptr;
	vector<CLine*>*			   m_pLines = nullptr;
	_bool			m_bNaviWindow = false;
	_float3*		m_vPickedPoint = nullptr;
	_float3			m_vPickedPos = { 0.f,0.f,0.f };
	_float3			m_vPickedAddPos[3];
	_bool			m_bPickedAddPosVailed[3];
	_int			m_iLineType = 0;

public:
	typedef enum eTerrain_Edit_Type
	{
		TERRAIN_EDIT_TYPE_HEIGHT,
		TERRAIN_EDIT_TYPE_TEXTURE_01,
		TERRAIN_EDIT_TYPE_TEXTURE_02,
		TERRAIN_EDIT_TYPE_TEXTURE_03,
		TERRAIN_EDIT_TYPE_END
	}TERRAIN_EDIT_TYPE;

public:	//	맵툴 도구
	TERRAIN_EDIT_TYPE		m_eEdit_Type = TERRAIN_EDIT_TYPE_END;
	_float					m_vDampning[4];
	_float					m_vAddValues;
	_float					m_fMaxHeight =50.f;
	_float					m_fMinHeight = 0.f;
	_bool					m_bPressing = false;
	_uint					m_eEditMode = TERRAIN_EDIT_TYPE_END;
	_uint					m_iFPS = 0;
	_bool					m_bIsChanged = false;

public:	//	맵툴 함수
	void Edit_Input();	//	툴로 지정한 값들로 터레인을 수정합니다.
	void Set_Range_And_Dampning();
	void Set_Value_Option();
	void Set_Edit_Type_Option();
	void Save_Map();
	void Load_Map();
#endif

};

END