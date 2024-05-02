#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Point_Instance;
END

BEGIN(Client)

class CDeco_Snow final : public CGameObject
{
private:
	CDeco_Snow(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CDeco_Snow(const CDeco_Snow& rhs);
	virtual ~CDeco_Snow() = default;

public:
	virtual HRESULT				NativeConstruct_Prototype();
	virtual HRESULT				NativeConstruct(void* pArg);
	virtual _int				Tick(_double TimeDelta);
	virtual _int				LateTick(_double TimeDelta);
	virtual HRESULT				Render();

private:
	CShader*					m_pShaderCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;
	CRenderer*					m_pRendererCom = nullptr;
	CVIBuffer_Point_Instance*	m_pVIBufferCom = nullptr;

private:
	HRESULT						SetUp_Components();
	HRESULT						SetUp_ConstantTable();

private:
	_uint						m_iCurLevel = 0;
	_float						m_fMaxX, m_fMaxY, m_fMaxZ = 0;
	_float						m_fSetX, m_fSetY, m_fSetZ = 0;
	_bool						m_bFirst = true;

public:
	static CDeco_Snow*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject*		Clone(void* pArg);
	virtual void				Free() override;

};

END