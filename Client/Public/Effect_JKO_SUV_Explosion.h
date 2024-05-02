#pragma once

#include "Effect_None_Anim_Effect.h"

BEGIN(Client)

class CEffect_JKO_SUV_Explosion final : public CEffect_None_Anim_Effect
{
private:
	explicit CEffect_JKO_SUV_Explosion(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_JKO_SUV_Explosion(const CEffect_JKO_SUV_Explosion& rhs);
	virtual ~CEffect_JKO_SUV_Explosion() = default;

public:
	virtual HRESULT						NativeConstruct_Prototype();
	virtual HRESULT						NativeConstruct(void* pArg);
	virtual _int						Tick(_double TimeDelta);
	virtual _int						LateTick(_double TimeDelta);
	virtual HRESULT						Render();

private:
	_bool								m_bGetBig = true;
	_bool								m_bCreateEffect = false;
	_uint								m_iCount = 0;
	_float								m_fSize = 0.f;
	_double								m_fTimeDelta = 0.f;

private:
	HRESULT								SetBillboardMatrix();
	HRESULT								CreateEffect_SmallExplosion();

public:
	static	CEffect_JKO_SUV_Explosion*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject*				Clone(void* pArg);
	virtual void						Free() override;

};

END