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

class CDeco_Glow final : public CGameObject
{
public:
	typedef	enum tagGlowArea
	{
		AREA_UP, AREA_DOWN, AREA_END
	}GLOWAREA;

	typedef	enum tagGlowMoveDir
	{
		DIR_UP, DIR_DOWN, DIR_END
	}GLOWMOVEDIR;

	typedef	struct tagGlowDesc
	{
		GLOWAREA		eGlowArea;
		GLOWMOVEDIR		eGlowMoveDir;
	}GLOWDESC;

private:
	CDeco_Glow(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CDeco_Glow(const CDeco_Glow& rhs);
	virtual ~CDeco_Glow() = default;

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

	_vector					m_vRightScale, m_vUpScale, m_vLookScale;
	_float					m_fSize = 0.f;
	_bool					m_bGetBig = true;
	_bool					m_bGetSmall = false;
	_double					m_dTimeAcc = 0.f;

	_vector					m_vStartPos;
	_float					m_fRandX, m_fRandY, m_fRandZ, m_fRandTimeAcc = 0;
	_vector					m_vEndPos;
	_float					m_fValue = 0.f;

	_bool					m_bEndPos = false;

	GLOWDESC				m_GlowDesc;

	void					SetStartPos();
	void					SetEndPos();

	void					Looking();
	void					Scaling(_double TimeDelta);

	void					Movement(_double TimeDelta);

public:
	static	CDeco_Glow*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject*	Clone(void* pArg);
	virtual	void			Free() override;

};

END