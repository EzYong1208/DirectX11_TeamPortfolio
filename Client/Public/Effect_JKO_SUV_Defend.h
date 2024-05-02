#pragma once

#include "Effect_None_Anim_Effect.h"

BEGIN(Client)

class CEffect_JKO_SUV_Defend final : public CEffect_None_Anim_Effect
{
private:
	explicit CEffect_JKO_SUV_Defend(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_JKO_SUV_Defend(const CEffect_JKO_SUV_Defend& rhs);
	virtual ~CEffect_JKO_SUV_Defend() = default;

public:
	virtual HRESULT						NativeConstruct_Prototype();
	virtual HRESULT						NativeConstruct(void* pArg);
	virtual _int						Tick(_double TimeDelta);
	virtual _int						LateTick(_double TimeDelta);
	virtual HRESULT						Render();

private:
	_float								m_fTimeDelta = 0.f;

public:
	static	CEffect_JKO_SUV_Defend*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject*				Clone(void* pArg);
	virtual void						Free() override;

};

END