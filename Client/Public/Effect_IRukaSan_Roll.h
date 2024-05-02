#pragma once

#include "Effect_Anim_Effect.h"

BEGIN(Client)

class CEffect_IRukaSan_Roll : public CEffect_Anim_Effect
{
private:
	explicit CEffect_IRukaSan_Roll(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_IRukaSan_Roll(const CEffect_IRukaSan_Roll& rhs);
	virtual ~CEffect_IRukaSan_Roll() = default;

public:
	virtual HRESULT						NativeConstruct_Prototype();
	virtual HRESULT						NativeConstruct(void* pArg);
	virtual _int						Tick(_double TimeDelta);
	virtual _int						LateTick(_double TimeDelta);
	virtual HRESULT						Render();
	virtual HRESULT						Render_OutLine();

private:
	HRESULT								SetUp_Components();
	HRESULT								Create_Effect_Splash();

private:
	_float								m_fTimeDelta = 0.f;
	_bool								m_bCreateOnce = false;

public:
	static	CEffect_IRukaSan_Roll*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject*				Clone(void* pArg);
	virtual void						Free() override;

};

END