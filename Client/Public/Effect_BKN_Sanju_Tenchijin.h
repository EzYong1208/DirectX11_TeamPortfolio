#pragma once

#include "Effect_BillboardValueFix_Effect.h"

BEGIN(Client)

class CEffect_BKN_Sanju_Tenchijin final : public CEffect_BillboardValueFix_Effect
{
private:
	explicit CEffect_BKN_Sanju_Tenchijin(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_BKN_Sanju_Tenchijin(const CEffect_BKN_Sanju_Tenchijin& rhs);
	virtual ~CEffect_BKN_Sanju_Tenchijin() = default;

public:
	virtual HRESULT							NativeConstruct_Prototype();
	virtual HRESULT							NativeConstruct(void* pArg);
	virtual _int							Tick(_double TimeDelta);
	virtual _int							LateTick(_double TimeDelta);
	virtual HRESULT							Render();

private:
	_float									m_fAlpha = 0.f;

private:
	void									SetAlphaCase();

public:
	static	CEffect_BKN_Sanju_Tenchijin*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject*					Clone(void* pArg);
	virtual void							Free() override;

};

END