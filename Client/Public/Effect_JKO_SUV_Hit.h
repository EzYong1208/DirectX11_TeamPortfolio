#pragma once

#include "Effect_None_Anim_Effect.h"

BEGIN(Client)

class CEffect_JKO_SUV_Hit final : public CEffect_None_Anim_Effect
{
public:
	typedef enum tagJKOSUVHitType
	{
		HIT_NORMAL, HIT_EXPLOSION, HIT_END
	}HITTYPE;

	typedef struct tagJKOSUVHitDesc
	{
		tagJKOSUVHitDesc() = default;
		tagJKOSUVHitDesc(EFFECTDESC _EffDesc, SUB_EFFECT_DESC _SubEffDesc)
			: EffectDesc(_EffDesc)
			, SubEffectDesc(_SubEffDesc) {}

		EFFECTDESC			EffectDesc;
		SUB_EFFECT_DESC		SubEffectDesc;
		HITTYPE				eType;
	}JKOSUVHITDESC;

private:
	explicit CEffect_JKO_SUV_Hit(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_JKO_SUV_Hit(const CEffect_JKO_SUV_Hit& rhs);
	virtual ~CEffect_JKO_SUV_Hit() = default;

public:
	virtual HRESULT					NativeConstruct_Prototype();
	virtual HRESULT					NativeConstruct(void* pArg);
	virtual _int					Tick(_double TimeDelta);
	virtual _int					LateTick(_double TimeDelta);
	virtual HRESULT					Render();

private:
	JKOSUVHITDESC					m_JKOSUVHitDesc;

private:
	HRESULT							SetBillboardMatrix();

public:
	static	CEffect_JKO_SUV_Hit*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject*			Clone(void* pArg);
	virtual void					Free() override;

};

END