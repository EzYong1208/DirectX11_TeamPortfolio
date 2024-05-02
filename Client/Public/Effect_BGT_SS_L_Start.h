#pragma once

#include "Effect_None_Anim_Effect.h"

BEGIN(Client)

class CEffect_BGT_SS_L_Start final : public CEffect_None_Anim_Effect
{
private:
	explicit CEffect_BGT_SS_L_Start(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_BGT_SS_L_Start(const CEffect_BGT_SS_L_Start& rhs);
	virtual ~CEffect_BGT_SS_L_Start() = default;

public:
	virtual HRESULT						NativeConstruct_Prototype();
	virtual HRESULT						NativeConstruct(void* pArg);
	virtual _int						Tick(_double TimeDelta);
	virtual _int						LateTick(_double TimeDelta);
	virtual HRESULT						Render();

private:
	_uint								m_iMaterialIndex = 0;
	_uint								m_iCount = 0;

private:
	_uint								SetMaterialIndex();

public:
	static	CEffect_BGT_SS_L_Start*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject*				Clone(void* pArg);
	virtual void						Free() override;

};

END