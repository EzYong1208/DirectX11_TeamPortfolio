#pragma once

#include "Effect_BillboardValueFix_Effect.h"

BEGIN(Client)

class CEffect_GOS_WaterRing final : public CEffect_BillboardValueFix_Effect
{
private:
	explicit CEffect_GOS_WaterRing(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_GOS_WaterRing(const CEffect_GOS_WaterRing& rhs);
	virtual ~CEffect_GOS_WaterRing() = default;

public:
	virtual HRESULT					NativeConstruct_Prototype();
	virtual HRESULT					NativeConstruct(void* pArg);
	virtual _int					Tick(_double TimeDelta);
	virtual _int					LateTick(_double TimeDelta);
	virtual HRESULT					Render();

public:
	static	CEffect_GOS_WaterRing*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject*			Clone(void* pArg);
	virtual void					Free() override;

};

END