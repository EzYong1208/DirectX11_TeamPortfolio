#pragma once

#include "Effect_Anim_Effect.h"

BEGIN(Client)

class CEffect_IRukaSan_Ride final : public CEffect_Anim_Effect
{
private:
	explicit CEffect_IRukaSan_Ride(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_IRukaSan_Ride(const CEffect_IRukaSan_Ride& rhs);
	virtual ~CEffect_IRukaSan_Ride() = default;

public:
	virtual HRESULT						NativeConstruct_Prototype();
	virtual HRESULT						NativeConstruct(void* pArg);
	virtual _int						Tick(_double TimeDelta);
	virtual _int						LateTick(_double TimeDelta);
	virtual HRESULT						Render();
	virtual HRESULT						Render_OutLine();

public:	//	트리거 발동시
	virtual	void						Trigger_Activate(const _double& _fTimeDelta);

private:
	HRESULT								SetUp_Components();

private:
	_bool								m_bHit = false;

public:
	void								SetHitted() { m_bHit = true; }

public:
	static	CEffect_IRukaSan_Ride*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject*				Clone(void* pArg);
	virtual void						Free() override;

};

END