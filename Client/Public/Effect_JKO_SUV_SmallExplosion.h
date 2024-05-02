#pragma once

#include "Effect_None_Anim_Effect.h"

BEGIN(Client)

class CEffect_JKO_SUV_SmallExplosion final : public CEffect_None_Anim_Effect
{
public:
	typedef enum tagJKOSUVSmallExplosionType
	{
		TYPE_00, TYPE_01, TYPE_02, TYPE_03, TYPE_04, TYPE_05, TYPE_06, TYPE_07, TYPE_08, TYPE_09, TYPE_END
	}SMALLEXPLOSIONTYPE;

	typedef	struct tagJKOSUVSmallExplosionDesc
	{
		tagJKOSUVSmallExplosionDesc() = default;
		tagJKOSUVSmallExplosionDesc(EFFECTDESC _EffDesc, SUB_EFFECT_DESC _SubEffDesc)
			: EffectDesc(_EffDesc)
			, SubEffectDesc(_SubEffDesc) {}

		EFFECTDESC				EffectDesc;
		SUB_EFFECT_DESC			SubEffectDesc;
		SMALLEXPLOSIONTYPE		eType;
	}JKOSUVSMALLEXPLOSIONDESC;

private:
	explicit CEffect_JKO_SUV_SmallExplosion(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_JKO_SUV_SmallExplosion(const CEffect_JKO_SUV_SmallExplosion& rhs);
	virtual ~CEffect_JKO_SUV_SmallExplosion() = default;

public:
	virtual HRESULT								NativeConstruct_Prototype();
	virtual HRESULT								NativeConstruct(void* pArg);
	virtual _int								Tick(_double TimeDelta);
	virtual _int								LateTick(_double TimeDelta);
	virtual HRESULT								Render();

private:
	JKOSUVSMALLEXPLOSIONDESC					m_JKOSUVSmallExplosionDesc;
	_bool										m_bGetBig = true;
	_float										m_fSize = 0.4f;
	_double										m_fTimeDelta = 0.f;
	_float										m_fStartX, m_fStartY = 0.f;
	_float										m_fValueX, m_fValueY = 0.f;
	_double										m_fAlpha = 0.f;

private:
	HRESULT										SetBillboardMatrix();
	HRESULT										SetValue();

public:
	static	CEffect_JKO_SUV_SmallExplosion*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject*						Clone(void* pArg);
	virtual void								Free() override;

};

END