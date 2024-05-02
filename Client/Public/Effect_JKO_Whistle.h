#pragma once

#include "Effect_Anim_Effect.h"

BEGIN(Client)

class CEffect_JKO_Whistle : public CEffect_Anim_Effect
{
public:
	typedef	enum tagAnimIndex
	{
		INDEX_432, INDEX_433, INDEX_502, INDEX_END
	}ANIMINDEX;

	typedef struct tagJKOWhistleDesc
	{
		tagJKOWhistleDesc() = default;
		tagJKOWhistleDesc(EFFECTDESC _EffDesc, SUB_EFFECT_DESC _SubEffDesc)
			: EffectDesc(_EffDesc)
			, SubEffectDesc(_SubEffDesc) {}

		EFFECTDESC			EffectDesc;
		SUB_EFFECT_DESC		SubEffectDesc;
		ANIMINDEX			eIndex;
	}JKOWHISTLEDESC;

private:
	explicit CEffect_JKO_Whistle(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_JKO_Whistle(const CEffect_JKO_Whistle& rhs);
	virtual ~CEffect_JKO_Whistle() = default;

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
	JKOWHISTLEDESC					m_JKOWhistleDesc;
	_uint							m_iFrameLockCount = 0;

public:
	static	CEffect_JKO_Whistle*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject*			Clone(void* pArg);
	virtual void					Free() override;

};

END