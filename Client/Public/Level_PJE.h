#pragma once

#include "Client_Defines.h"
#include "Level.h"
#include "GameObject.h"
#include "Layer.h"

BEGIN(Client)

class CLevel_PJE final : public CLevel
{
public:
	explicit CLevel_PJE(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CLevel_PJE() = default;
public:
	virtual HRESULT NativeConstruct();
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Lights();
	HRESULT Ready_ObjectPrototype();
	HRESULT Ready_Terrain();

private:
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);


public:
	static CLevel_PJE* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual void Free() override;

	//IMGUI========================================================================================================================================================================================
#if defined(USE_IMGUI)
private:	//ÇÔ¼ö
	HRESULT SetUp_Editer_Window();
	void Running_Editer_Window(_double TimeDelta);
	HRESULT Add_Static_Object(const _tchar* _pPrototypeTag, _uint _iLayerType, MODEL _eModelIndex = (MODEL)0);
	HRESULT Add_Object(_bool _bIsStatic, OBJECTDESC _tagObj);

private:
	void Widget_Save_Load_Objects();
	void Widget_Add_Object();

private:
	_int iPickLayer = 0;
	_int iPickModel = 0;

private:
	map<const _tchar*, class CGameObject*>* m_pEditer_Prototypes = nullptr;
	map<const _tchar*, class CLayer*>* m_pEditer_Objects = nullptr;
	_bool m_bShow_Editer_Window = true;
#endif
	//IMGUI========================================================================================================================================================================================


};

END