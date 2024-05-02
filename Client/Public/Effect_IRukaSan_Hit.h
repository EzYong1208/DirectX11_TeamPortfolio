#pragma once

#include "Effect_Anim_Effect.h"

BEGIN(Client)

class CEffect_IRukaSan_Hit : public CEffect_Anim_Effect
{
private:
	explicit CEffect_IRukaSan_Hit(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_IRukaSan_Hit(const CEffect_IRukaSan_Hit& rhs);
	virtual ~CEffect_IRukaSan_Hit() = default;

public:
	virtual HRESULT						NativeConstruct_Prototype();
	virtual HRESULT						NativeConstruct(void* pArg);
	virtual _int						Tick(_double TimeDelta);
	virtual _int						LateTick(_double TimeDelta);
	virtual HRESULT						Render();
	virtual HRESULT						Render_OutLine();

private:
	HRESULT								SetUp_Components();
	HRESULT								Create_Effect_IRukaSan_Roll();

public:
	static	CEffect_IRukaSan_Hit*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject*				Clone(void* pArg);
	virtual void						Free() override;

};

END