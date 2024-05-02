#pragma once

#include "Effect_BillboardValueFix_Effect.h"

BEGIN(Client)

class CEffect_BGT_RogerSpeedline final : public CEffect_BillboardValueFix_Effect
{
private:
	explicit CEffect_BGT_RogerSpeedline(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_BGT_RogerSpeedline(const CEffect_BGT_RogerSpeedline& rhs);
	virtual ~CEffect_BGT_RogerSpeedline() = default;

public:
	virtual HRESULT							NativeConstruct_Prototype();
	virtual HRESULT							NativeConstruct(void* pArg);
	virtual _int							Tick(_double TimeDelta);
	virtual _int							LateTick(_double TimeDelta);
	virtual HRESULT							Render();

public:
	static	CEffect_BGT_RogerSpeedline*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject*					Clone(void* pArg);
	virtual void							Free() override;

};

END