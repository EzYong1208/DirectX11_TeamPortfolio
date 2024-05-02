#pragma once

#include "Effect_Anim_Effect.h"

BEGIN(Client)

class CEffect_IronMaidon : public CEffect_Anim_Effect
{
private:
	typedef	enum tagAnimIndex
	{
		INDEX_00, INDEX_END
	}ANIMINDEX;

private:
	explicit CEffect_IronMaidon(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_IronMaidon(const CEffect_IronMaidon& rhs);
	virtual ~CEffect_IronMaidon() = default;

public:
	virtual HRESULT					NativeConstruct_Prototype();
	virtual HRESULT					NativeConstruct(void* pArg);
	virtual _int					Tick(_double TimeDelta);
	virtual _int					LateTick(_double TimeDelta);
	virtual HRESULT					Render();
	virtual HRESULT					Render_OutLine();

private:
	_uint							FrameLock();

private:
	_uint							m_iFrameLockCount = 0;

public:
	static	CEffect_IronMaidon*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject*			Clone(void* pArg);
	virtual void					Free() override;

};

END