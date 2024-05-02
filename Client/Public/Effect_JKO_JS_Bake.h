#pragma once

#include "Effect_None_Anim_Effect.h"

BEGIN(Client)

class CEffect_JKO_JS_Bake final : public CEffect_None_Anim_Effect
{
private:
	explicit CEffect_JKO_JS_Bake(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_JKO_JS_Bake(const CEffect_JKO_JS_Bake& rhs);
	virtual ~CEffect_JKO_JS_Bake() = default;

public:
	virtual HRESULT					NativeConstruct_Prototype();
	virtual HRESULT					NativeConstruct(void* pArg);
	virtual _int					Tick(_double TimeDelta);
	virtual _int					LateTick(_double TimeDelta);
	virtual HRESULT					Render();

private:
	_uint							m_iMaterialIndex = 0;
	_uint							m_iCount = 0;

private:
	_uint							SetMaterialIndex();

public:
	static	CEffect_JKO_JS_Bake*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject*			Clone(void* pArg);
	virtual void					Free() override;

};

END