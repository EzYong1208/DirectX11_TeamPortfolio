#pragma once

#include "Effect_None_Anim_Effect.h"

BEGIN(Client)

class CEffect_JKO_5D_Soul final : public CEffect_None_Anim_Effect
{
private:
	explicit CEffect_JKO_5D_Soul(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_JKO_5D_Soul(const CEffect_JKO_5D_Soul& rhs);
	virtual ~CEffect_JKO_5D_Soul() = default;

public:
	virtual HRESULT					NativeConstruct_Prototype();
	virtual HRESULT					NativeConstruct(void* pArg);
	virtual _int					Tick(_double TimeDelta);
	virtual _int					LateTick(_double TimeDelta);
	virtual HRESULT					Render();

public:
	static	CEffect_JKO_5D_Soul*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject*			Clone(void* pArg);
	virtual void					Free() override;

};

END