#pragma once

#include "Effect_Anim_Effect.h"

BEGIN(Client)

class CEffect_MAY_MayDynamic : public CEffect_Anim_Effect
{
private:
	typedef	enum tagAnimIndex
	{
		INDEX_00, INDEX_END
	}ANIMINDEX;

private:
	explicit CEffect_MAY_MayDynamic(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_MAY_MayDynamic(const CEffect_MAY_MayDynamic& rhs);
	virtual ~CEffect_MAY_MayDynamic() = default;

public:
	virtual HRESULT						NativeConstruct_Prototype();
	virtual HRESULT						NativeConstruct(void* pArg);
	virtual _int						Tick(_double TimeDelta);
	virtual _int						LateTick(_double TimeDelta);
	virtual HRESULT						Render();
	virtual HRESULT						Render_OutLine();

private:
	HRESULT								SetUp_ConstantTable_BillBoarding();
	_uint								FrameLock();

private:
	_bool								m_bMove = false;
	_double								m_fCurAcc = 0.f;
	_uint								m_ActorSide = 0;
public:
	static	CEffect_MAY_MayDynamic*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject*				Clone(void* pArg);
	virtual void						Free() override;

};

END