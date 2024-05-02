#pragma once

#include "Effect_Anim_Effect.h"

BEGIN(Client)

class CEffect_IRukaSan_Anim : public CEffect_Anim_Effect
{
private:
	explicit CEffect_IRukaSan_Anim(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_IRukaSan_Anim(const CEffect_IRukaSan_Anim& rhs);
	virtual ~CEffect_IRukaSan_Anim() = default;

public:
	virtual HRESULT					NativeConstruct_Prototype();
	virtual HRESULT					NativeConstruct(void* pArg);
	virtual _int					Tick(_double TimeDelta);
	virtual _int					LateTick(_double TimeDelta);
	virtual HRESULT					Render();
	virtual HRESULT					Render_OutLine();

private:
	HRESULT							SetUp_Components();

private:
	_bool							m_bHit = false;
public:
	void							SetHitted() { m_bHit = true; }
public:
	static	CEffect_IRukaSan_Anim*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject*			Clone(void* pArg);
	virtual void					Free() override;

};

END