#pragma once

#include "Effect_BillboardValueFix_Effect.h"

BEGIN(Client)

class CEffect_BGT_KSMH_Sparkline final : public CEffect_BillboardValueFix_Effect
{
private:
	explicit CEffect_BGT_KSMH_Sparkline(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_BGT_KSMH_Sparkline(const CEffect_BGT_KSMH_Sparkline& rhs);
	virtual ~CEffect_BGT_KSMH_Sparkline() = default;

public:
	virtual HRESULT							NativeConstruct_Prototype();
	virtual HRESULT							NativeConstruct(void* pArg);
	virtual _int							Tick(_double TimeDelta);
	virtual _int							LateTick(_double TimeDelta);
	virtual HRESULT							Render();

private:
	_uint									m_iMaterialIndex = 0;
	_uint									m_iAnimIndex = 0;

public:
	static	CEffect_BGT_KSMH_Sparkline*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject*					Clone(void* pArg);
	virtual void							Free() override;

};

END