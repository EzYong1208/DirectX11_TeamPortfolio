#pragma once

#include "Effect_None_Anim_Effect.h"

BEGIN(Client)

class CEffect_JKO_SUV_Countdown final : public CEffect_None_Anim_Effect
{
public:
	typedef	enum tagJKOSUVCountdownMaterialIndex
	{
		COUNT_1, COUNT_2, COUNT_3, COUNT_END
	}MATERIALINDEX;

	typedef	struct tagJKOSUVCountdownDesc
	{
		tagJKOSUVCountdownDesc() = default;
		tagJKOSUVCountdownDesc(EFFECTDESC _EffDesc, SUB_EFFECT_DESC _SubEffDesc)
			: EffectDesc(_EffDesc)
			, SubEffectDesc(_SubEffDesc) {}

		EFFECTDESC			EffectDesc;
		SUB_EFFECT_DESC		SubEffectDesc;
		MATERIALINDEX		eIndex;
	}JKOSUVCOUNTDOWNDESC;

private:
	explicit CEffect_JKO_SUV_Countdown(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_JKO_SUV_Countdown(const CEffect_JKO_SUV_Countdown& rhs);
	virtual ~CEffect_JKO_SUV_Countdown() = default;

public:
	virtual HRESULT						NativeConstruct_Prototype();
	virtual HRESULT						NativeConstruct(void* pArg);
	virtual _int						Tick(_double TimeDelta);
	virtual _int						LateTick(_double TimeDelta);
	virtual HRESULT						Render();

private:
	JKOSUVCOUNTDOWNDESC					m_JKOSUVCountdownDesc;
	_uint								m_iMaterialIndex = 0;
	_double								m_fAlpha = 0.f;

private:
	void								SetMaterialIndex();

public:
	static	CEffect_JKO_SUV_Countdown*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject*				Clone(void* pArg);
	virtual void						Free() override;

};

END