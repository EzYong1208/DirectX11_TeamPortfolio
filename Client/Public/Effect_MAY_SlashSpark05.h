#pragma once

#include "Effect_BillboardValueFix_Effect.h"

BEGIN(Client)

class CEffect_MAY_SlashSpark05 final : public CEffect_BillboardValueFix_Effect
{
private:
	explicit CEffect_MAY_SlashSpark05(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_MAY_SlashSpark05(const CEffect_MAY_SlashSpark05& rhs);
	virtual ~CEffect_MAY_SlashSpark05() = default;

public:
	virtual HRESULT						NativeConstruct_Prototype();
	virtual HRESULT						NativeConstruct(void* pArg);
	virtual _int						Tick(_double TimeDelta);
	virtual _int						LateTick(_double TimeDelta);
	virtual HRESULT						Render();

public:
	static	CEffect_MAY_SlashSpark05*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject*				Clone(void* pArg);
	virtual void						Free() override;

};

END