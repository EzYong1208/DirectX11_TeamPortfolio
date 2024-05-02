#pragma once

#include "Effect_None_Anim_Effect.h"

BEGIN(Client)

class CEffect_BGT_RKM_FloorFire final : public CEffect_None_Anim_Effect
{
private:
	explicit CEffect_BGT_RKM_FloorFire(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_BGT_RKM_FloorFire(const CEffect_BGT_RKM_FloorFire& rhs);
	virtual ~CEffect_BGT_RKM_FloorFire() = default;

public:
	virtual HRESULT							NativeConstruct_Prototype();
	virtual HRESULT							NativeConstruct(void* pArg);
	virtual _int							Tick(_double TimeDelta);
	virtual _int							LateTick(_double TimeDelta);
	virtual HRESULT							Render();

private:
	_float									m_fTimer = 0.f;
	_uint									m_iCount = 0.f;

private:
	HRESULT									CreateEffect_Ember(_double TimeDelta);

public:
	static	CEffect_BGT_RKM_FloorFire*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject*					Clone(void* pArg);
	virtual void							Free() override;

};

END