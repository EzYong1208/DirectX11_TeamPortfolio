#pragma once

#include "Effect_None_Anim_Effect.h"

BEGIN(Client)

class CEffect_Twinkle02_06 final : public CEffect_None_Anim_Effect
{
private:
	explicit CEffect_Twinkle02_06(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_Twinkle02_06(const CEffect_Twinkle02_06& rhs);
	virtual ~CEffect_Twinkle02_06() = default;

public:
	virtual HRESULT					NativeConstruct_Prototype();
	virtual HRESULT					NativeConstruct(void* pArg);
	virtual _int					Tick(_double TimeDelta);
	virtual _int					LateTick(_double TimeDelta);
	virtual HRESULT					Render();

public:	//	트리거 발동시
	virtual	void					Trigger_Activate(const _double& _fTimeDelta);

private:
	HRESULT							SetUp_Components();

private:
	_double							m_FlashTimeDelta = 0.f;
	_bool							m_bHit = false;

public:
	void							SetHitted() { m_bHit = true; }

public:
	static	CEffect_Twinkle02_06*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject*			Clone(void* pArg);
	virtual void					Free() override;

};

END