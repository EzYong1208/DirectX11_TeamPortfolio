#pragma once

#include "Effect_None_Anim_Effect.h"

BEGIN(Client)

class CEffect_BKN_Tatami_Windring final : public CEffect_None_Anim_Effect
{
private:
	explicit CEffect_BKN_Tatami_Windring(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_BKN_Tatami_Windring(const CEffect_BKN_Tatami_Windring& rhs);
	virtual ~CEffect_BKN_Tatami_Windring() = default;

public:
	virtual HRESULT							NativeConstruct_Prototype();
	virtual HRESULT							NativeConstruct(void* pArg);
	virtual _int							Tick(_double TimeDelta);
	virtual _int							LateTick(_double TimeDelta);
	virtual HRESULT							Render();

private:
	_uint									m_iMaterialIndex = 0;

private:
	void									SetMaterialIndex();

public:
	static	CEffect_BKN_Tatami_Windring*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject*					Clone(void* pArg);
	virtual void							Free() override;

};

END