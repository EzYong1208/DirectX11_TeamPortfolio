#pragma once

#include "Effect_None_Anim_Effect.h"

BEGIN(Client)

class CEffect_BGT_RKM_FireRing final : public CEffect_None_Anim_Effect
{
private:
	explicit CEffect_BGT_RKM_FireRing(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_BGT_RKM_FireRing(const CEffect_BGT_RKM_FireRing& rhs);
	virtual ~CEffect_BGT_RKM_FireRing() = default;

public:
	virtual HRESULT						NativeConstruct_Prototype();
	virtual HRESULT						NativeConstruct(void* pArg);
	virtual _int						Tick(_double TimeDelta);
	virtual _int						LateTick(_double TimeDelta);
	virtual HRESULT						Render();

private:
	_float								m_fChangeColorTimer = 0.f;
	_bool								m_bRenderSecond = false;
	_uint								m_iCount = 0;

private:
	HRESULT								SetRGBA(_double TimeDelta);
	HRESULT								CreateEffect_FloorFire();

public:
	static	CEffect_BGT_RKM_FireRing*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject*				Clone(void* pArg);
	virtual void						Free() override;

};

END