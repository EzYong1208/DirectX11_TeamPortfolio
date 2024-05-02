#pragma once

#include "Effect_None_Anim_Effect.h"

BEGIN(Client)

class CEffect_MAY_Jump_Down_Anchor2 final : public CEffect_None_Anim_Effect
{
private:
	explicit CEffect_MAY_Jump_Down_Anchor2(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_MAY_Jump_Down_Anchor2(const CEffect_MAY_Jump_Down_Anchor2& rhs);
	virtual ~CEffect_MAY_Jump_Down_Anchor2() = default;

public:
	virtual HRESULT							NativeConstruct_Prototype();
	virtual HRESULT							NativeConstruct(void* pArg);
	virtual _int							Tick(_double TimeDelta);
	virtual _int							LateTick(_double TimeDelta);
	virtual HRESULT							Render();

public:	//	Ʈ���� �ߵ���
	virtual	void							Trigger_Activate(const _double& _fTimeDelta);

private:
	HRESULT									SetUp_Components();

private:
	_double									m_FlashTimeDelta = 0.f;

public:
	static	CEffect_MAY_Jump_Down_Anchor2*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject*					Clone(void* pArg);
	virtual void							Free() override;

};

END