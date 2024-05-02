#pragma once

#include "Effect_None_Anim_Effect.h"

BEGIN(Client)

class CEffect_JKO_Servant_Shoot final : public CEffect_None_Anim_Effect
{
public:
	typedef	enum tagJKOServantShootMaterialIndex
	{
		SHOOT_GROUND, SHOOT_AIR, SHOOT_END
	}MATERIALINDEX;

	typedef struct tagJKOServantShootDesc
	{
		tagJKOServantShootDesc() = default;
		tagJKOServantShootDesc(EFFECTDESC _EffDesc, SUB_EFFECT_DESC _SubEffDesc)
			: EffectDesc(_EffDesc)
			, SubEffectDesc(_SubEffDesc) {}

		EFFECTDESC			EffectDesc;
		SUB_EFFECT_DESC		SubEffectDesc;
		MATERIALINDEX		eIndex;
	}JKOSERVANTSHOOTDESC;

private:
	explicit CEffect_JKO_Servant_Shoot(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_JKO_Servant_Shoot(const CEffect_JKO_Servant_Shoot& rhs);
	virtual ~CEffect_JKO_Servant_Shoot() = default;

public:
	virtual HRESULT						NativeConstruct_Prototype();
	virtual HRESULT						NativeConstruct(void* pArg);
	virtual _int						Tick(_double TimeDelta);
	virtual _int						LateTick(_double TimeDelta);
	virtual HRESULT						Render();

private:
	JKOSERVANTSHOOTDESC					m_JKOServantShootDesc;
	_uint								m_iMaterialIndex = 0;

private:
	void								SetMaterialIndex();

public:
	static	CEffect_JKO_Servant_Shoot*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject*				Clone(void* pArg);
	virtual void						Free() override;

};

END