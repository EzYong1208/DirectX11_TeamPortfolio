#pragma once

#include "Effect_None_Anim_Effect.h"

BEGIN(Client)

class CEffect_BGT_RKM_Bubble final : public CEffect_None_Anim_Effect
{
public:
	typedef enum tagBGTRKMBubbleMaterialIndex
	{
		BUBBLE_GO, BUBBLE_BANG, BUBBLE_RETURN, BUBBLE_END
	}MATERIALINDEX;

	typedef enum tagBGTRKMBubbleType
	{
		TYPE_GROUND, TYPE_AIR, TYPE_END
	}BUBBLETYPE;

	typedef struct tagBGTRKMBubbleDesc
	{
		tagBGTRKMBubbleDesc() = default;
		tagBGTRKMBubbleDesc(EFFECTDESC _EffDesc, SUB_EFFECT_DESC _SubEffDesc)
			: EffectDesc(_EffDesc)
			, SubEffectDesc(_SubEffDesc) {}

		EFFECTDESC			EffectDesc;
		SUB_EFFECT_DESC		SubEffectDesc;
		MATERIALINDEX		eIndex;
		BUBBLETYPE			eType;
		_bool				bReverse = false;
	}BGTRKMBUBBLEDESC;

private:
	explicit CEffect_BGT_RKM_Bubble(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_BGT_RKM_Bubble(const CEffect_BGT_RKM_Bubble& rhs);
	virtual ~CEffect_BGT_RKM_Bubble() = default;

public:
	virtual HRESULT						NativeConstruct_Prototype();
	virtual HRESULT						NativeConstruct(void* pArg);
	virtual _int						Tick(_double TimeDelta);
	virtual _int						LateTick(_double TimeDelta);
	virtual HRESULT						Render();

private:
	BGTRKMBUBBLEDESC					m_BGTRKMBubbleDesc;
	_uint								m_iMaterialIndex = 0;
	_float								m_fValueX, m_fValueY = 0.f;

private:
	void								SetMaterialIndex();
	HRESULT								SetPosition();
	HRESULT								SetUp_ConstantTable_BillBoarding();

public:
	static	CEffect_BGT_RKM_Bubble*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject*				Clone(void* pArg);
	virtual void						Free() override;

};

END