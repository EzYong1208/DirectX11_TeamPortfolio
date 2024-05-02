#pragma once

#include "Effect_None_Anim_Effect.h"

BEGIN(Client)

class CEffect_BKN_FS final : public CEffect_None_Anim_Effect
{
private:
	explicit CEffect_BKN_FS(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_BKN_FS(const CEffect_BKN_FS& rhs);
	virtual ~CEffect_BKN_FS() = default;

public:
	virtual HRESULT				NativeConstruct_Prototype();
	virtual HRESULT				NativeConstruct(void* pArg);
	virtual _int				Tick(_double TimeDelta);
	virtual _int				LateTick(_double TimeDelta);
	virtual HRESULT				Render();

public:
	static	CEffect_BKN_FS*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject*		Clone(void* pArg);
	virtual void				Free() override;

};

END