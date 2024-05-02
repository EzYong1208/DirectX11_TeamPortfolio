#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Texture.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CNavigation_Line;
class CVIBuffer_Terrain;
END

BEGIN(Client)

#define TERRAIN_NUM_X 360 
#define TERRAIN_NUM_Y 360

class CTerrain_For_Gameplay final : public CGameObject
{
public:
	typedef enum eTextureType
	{
		TEXTURETYPE_DIFFUSE,
		TEXTURETYPE_NORMAL,
		TEXTURETYPE_END
	}TEXTURETYPE;

private:
	CTerrain_For_Gameplay(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CTerrain_For_Gameplay(const CTerrain_For_Gameplay& rhs);
	virtual ~CTerrain_For_Gameplay() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_Shadow();

private:
	CShader*						m_pShaderCom = nullptr;
	CTexture*						m_pTextureCom[TEXTURETYPE_END];
	CRenderer*						m_pRendererCom = nullptr;
	CVIBuffer_Terrain*				m_pVIBufferCom = nullptr;
	CNavigation_Line*				m_pNavigation_Line = nullptr;
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();


public:
	static CTerrain_For_Gameplay* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END