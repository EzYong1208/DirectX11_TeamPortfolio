#pragma once

#include "Effect_None_Anim_Effect.h"

BEGIN(Client)

class CEffect_JKO_CheerKira final : public CEffect_None_Anim_Effect
{
public:
	typedef enum tagJKOCheerKiraMaterialIndex
	{
		KIRA_BIG, KIRA_SMALL, KIRA_END
	}MATERIALINDEX;

	typedef struct tagJKOCheerKiraDesc
	{
		tagJKOCheerKiraDesc() = default;
		tagJKOCheerKiraDesc(EFFECTDESC _EffDesc, SUB_EFFECT_DESC _SubEffDesc)
			: EffectDesc(_EffDesc)
			, SubEffectDesc(_SubEffDesc) {}

		EFFECTDESC			EffectDesc;
		SUB_EFFECT_DESC		SubEffectDesc;
		MATERIALINDEX		eIndex;
	}JKOCHEERKIRADESC;

private:
	explicit CEffect_JKO_CheerKira(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_JKO_CheerKira(const CEffect_JKO_CheerKira& rhs);
	virtual ~CEffect_JKO_CheerKira() = default;

public:
	virtual HRESULT					NativeConstruct_Prototype();
	virtual HRESULT					NativeConstruct(void* pArg);
	virtual _int					Tick(_double TimeDelta);
	virtual _int					LateTick(_double TimeDelta);
	virtual HRESULT					Render();

private:
	JKOCHEERKIRADESC				m_JKOCheerKiraDesc;
	_uint							m_iMaterialIndex = 0;

private:
	void							SetMaterialIndex();

public:
	static	CEffect_JKO_CheerKira*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject*			Clone(void* pArg);
	virtual void					Free() override;

};

END