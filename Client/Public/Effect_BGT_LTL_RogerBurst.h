#pragma once

#include "Effect_None_Anim_Effect.h"

BEGIN(Client)

class CEffect_BGT_LTL_RogerBurst final : public CEffect_None_Anim_Effect
{
private:
	explicit CEffect_BGT_LTL_RogerBurst(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_BGT_LTL_RogerBurst(const CEffect_BGT_LTL_RogerBurst& rhs);
	virtual ~CEffect_BGT_LTL_RogerBurst() = default;

public:
	virtual HRESULT							NativeConstruct_Prototype();
	virtual HRESULT							NativeConstruct(void* pArg);
	virtual _int							Tick(_double TimeDelta);
	virtual _int							LateTick(_double TimeDelta);
	virtual HRESULT							Render();

public:
	static	CEffect_BGT_LTL_RogerBurst*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject*					Clone(void* pArg);
	virtual void							Free() override;

};

END