#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CDeco_Bird final : public CGameObject
{
private:
	CDeco_Bird(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CDeco_Bird(const CDeco_Bird& rhs);
	virtual ~CDeco_Bird() = default;

public:
	virtual HRESULT			NativeConstruct_Prototype();
	virtual HRESULT			NativeConstruct(void* pArg);
	virtual _int			Tick(_double TimeDelta);
	virtual _int			LateTick(_double TimeDelta);
	virtual HRESULT			Render();

private:
	CShader*				m_pShaderCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;

private:
	HRESULT					SetUp_Components();
	HRESULT					SetUp_ConstantTable();

private:
	_uint					m_iCurLevel = 0;
	_float					m_fFrame = 0.f;

	_vector					m_vRightScale, m_vUpScale, m_vLookScale;
	_vector					m_vStartPos;

	_float					m_fRandX, m_fRandY, m_fRandZ = 0;
	_vector					m_vEndPos;

	_bool					m_bEnd = false;

	void					SetEndPos();
	void					Looking();
	void					Movement(_double TimeDelta);

public:
	static	CDeco_Bird*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject*	Clone(void* pArg);
	virtual	void			Free() override;

};

END