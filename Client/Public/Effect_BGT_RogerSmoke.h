#pragma once

#include "Effect_None_Anim_Effect.h"

BEGIN(Client)

class CEffect_BGT_RogerSmoke final : public CEffect_None_Anim_Effect
{
public:
	typedef enum tagBGTRogerSmokeMaterialIndex
	{
		SMOKE_00, SMOKE_01, SMOKE_02, SMOKE_END
	}MATERIALINDEX;

	typedef struct tagBGTRogerSmokeDesc
	{
		tagBGTRogerSmokeDesc() = default;
		tagBGTRogerSmokeDesc(EFFECTDESC _EffDesc, SUB_EFFECT_DESC _SubEffDesc)
			: EffectDesc(_EffDesc)
			, SubEffectDesc(_SubEffDesc) {}

		EFFECTDESC			EffectDesc;
		SUB_EFFECT_DESC		SubEffectDesc;
		MATERIALINDEX		eIndex;
		_float				fValueX, fValueY, fValueZ = 0.f;
	}BGTROGERSMOKEDESC;

private:
	explicit CEffect_BGT_RogerSmoke(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_BGT_RogerSmoke(const CEffect_BGT_RogerSmoke& rhs);
	virtual ~CEffect_BGT_RogerSmoke() = default;

public:
	virtual HRESULT						NativeConstruct_Prototype();
	virtual HRESULT						NativeConstruct(void* pArg);
	virtual _int						Tick(_double TimeDelta);
	virtual _int						LateTick(_double TimeDelta);
	virtual HRESULT						Render();

private:
	BGTROGERSMOKEDESC					m_BGTRogerSmokeDesc;
	_uint								m_iMaterialIndex = 0;

private:
	void								SetMaterialIndex();

public:
	static	CEffect_BGT_RogerSmoke*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject*				Clone(void* pArg);
	virtual void						Free() override;

};

END