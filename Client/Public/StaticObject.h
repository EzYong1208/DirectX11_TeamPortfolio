#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CCollider;
END

class CStaticObject : public CGameObject
{
protected:
	CStaticObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CStaticObject(const CStaticObject& rhs);
	virtual ~CStaticObject() = default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_Shadow();

private:
	void SetUp_Range();

private:
	void Model_Identitiy();
	void Model_Identitiy_Update(_double TimeDelta);
	void Model_Rotation(_double TimeDelta);

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();

protected:
	CShader* m_pShaderCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CModel* m_pModelCom = nullptr;
	CCollider* m_pSphereCom = nullptr;

protected:
	_bool				m_bGlow = false;
	_float				m_fMaxScale = 1.f;
	_float				m_fEmmisivePower = 0.f;
public:
	static CStaticObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

#if defined(USE_IMGUI)
public:
	virtual void Running_Window()override;

public:	//오브젝트툴 도구
	_float					m_vDegree[4];
	_float					m_vLocation[4];
	_float					m_vScale[4];
	_uint					m_bDelete = false;
public:	//오브젝트툴 함수
	virtual void Show_Window()override;
	void Set_Release();
	void Edit_Input();	//툴로 지정한 값들로 상태를 변환합니다.
	void Widget_Transform();
#endif

};

