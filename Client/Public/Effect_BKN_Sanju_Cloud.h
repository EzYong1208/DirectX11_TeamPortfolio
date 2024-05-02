#pragma once

#include "Effect_None_Anim_Effect.h"

BEGIN(Client)

class CEffect_BKN_Sanju_Cloud final : public CEffect_None_Anim_Effect
{
private:
	explicit CEffect_BKN_Sanju_Cloud(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_BKN_Sanju_Cloud(const CEffect_BKN_Sanju_Cloud& rhs);
	virtual ~CEffect_BKN_Sanju_Cloud() = default;

public:
	virtual HRESULT						NativeConstruct_Prototype();
	virtual HRESULT						NativeConstruct(void* pArg);
	virtual _int						Tick(_double TimeDelta);
	virtual _int						LateTick(_double TimeDelta);
	virtual HRESULT						Render();

private:
	_float								m_fAlpha = 0.f;

private:
	void								SetAlphaCase();

public:
	static	CEffect_BKN_Sanju_Cloud*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject*				Clone(void* pArg);
	virtual void						Free() override;

};

END