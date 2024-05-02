#pragma once

#include "Effect_None_Anim_Effect.h"

BEGIN(Client)

class CEffect_BKN_Sanju_Kidou final : public CEffect_None_Anim_Effect
{
public:
	typedef	enum tagBKNSanjuKidouMaterialIndex
	{
		KIDOU_00, KIDOU_01, KIDOU_02, KIDOU_03, KIDOU_04, KIDOU_05, KIDOU_06,
		KIDOU_07, KIDOU_08, KIDOU_09, KIDOU_10, KIDOU_11, KIDOU_END
	}MATERIALINDEX;

	typedef struct tagBKNSanjuKidouDesc
	{
		tagBKNSanjuKidouDesc() = default;
		tagBKNSanjuKidouDesc(EFFECTDESC _EffDesc, SUB_EFFECT_DESC _SubEffDesc)
			: EffectDesc(_EffDesc)
			, SubEffectDesc(_SubEffDesc) {}

		EFFECTDESC			EffectDesc;
		SUB_EFFECT_DESC		SubEffectDesc;
		MATERIALINDEX		eIndex;
	}BKNSANJUKIDOUDESC;

private:
	explicit CEffect_BKN_Sanju_Kidou(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_BKN_Sanju_Kidou(const CEffect_BKN_Sanju_Kidou& rhs);
	virtual ~CEffect_BKN_Sanju_Kidou() = default;

public:
	virtual HRESULT						NativeConstruct_Prototype();
	virtual HRESULT						NativeConstruct(void* pArg);
	virtual _int						Tick(_double TimeDelta);
	virtual _int						LateTick(_double TimeDelta);
	virtual HRESULT						Render();

private:
	BKNSANJUKIDOUDESC					m_BKNSanjuKidouDesc;
	_uint								m_iMaterialIndex = 0;

private:
	void								SetMaterialIndex();

public:
	static	CEffect_BKN_Sanju_Kidou*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject*				Clone(void* pArg);
	virtual void						Free() override;

};

END