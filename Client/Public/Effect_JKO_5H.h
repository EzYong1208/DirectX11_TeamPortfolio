#pragma once

#include "Effect_None_Anim_Effect.h"

BEGIN(Client)

class CEffect_JKO_5H final : public CEffect_None_Anim_Effect
{
public:
	typedef enum tagJKO5HMaterialIndex
	{
		WIND_00, WIND_01, WIND_02, WIND_03, WIND_END
	}MATERIALINDEX;

	typedef struct tagJKO5HDesc
	{
		tagJKO5HDesc() = default;
		tagJKO5HDesc(EFFECTDESC _EffDesc, SUB_EFFECT_DESC _SubEffDesc)
			: EffectDesc(_EffDesc)
			, SubEffectDesc(_SubEffDesc) {}

		EFFECTDESC			EffectDesc;
		SUB_EFFECT_DESC		SubEffectDesc;
		MATERIALINDEX		eIndex;
	}JKO5HDESC;

private:
	explicit CEffect_JKO_5H(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_JKO_5H(const CEffect_JKO_5H& rhs);
	virtual ~CEffect_JKO_5H() = default;

public:
	virtual HRESULT				NativeConstruct_Prototype();
	virtual HRESULT				NativeConstruct(void* pArg);
	virtual _int				Tick(_double TimeDelta);
	virtual _int				LateTick(_double TimeDelta);
	virtual HRESULT				Render();

private:
	JKO5HDESC					m_JKO5HDesc;
	_uint						m_iMaterialIndex = 0;

private:
	void						SetMaterialIndex();

public:
	static	CEffect_JKO_5H*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject*		Clone(void* pArg);
	virtual void				Free() override;

};

END