#pragma once

#include "Effect.h"

BEGIN(Engine)
class CModel;
class CShader;
END

BEGIN(Client)

typedef struct tagAnimEffectDesc
{
	tagAnimEffectDesc() = default;
	tagAnimEffectDesc(EFFECTDESC _EffDesc, SUB_EFFECT_DESC _SubEffDesc)
		: EffectDesc(_EffDesc)
		, SubEffectDesc(_SubEffDesc) {}

	EFFECTDESC			EffectDesc;
	SUB_EFFECT_DESC		SubEffectDesc;
}ANIMEFFECTDESC;

// CHP
#define EFFECT_CHP_5D_TORNADO ANIMEFFECTDESC(EFFECT_DESC_CHP_5D_TORNADO, EFFECT_SUBDESC_CHP_5D_TORNADO)
#define	EFFECT_CHP_RAKKO	ANIMEFFECTDESC(EFFECT_DESC_CHP_RAKKO, EFFECT_SUBDESC_CHP_RAKKO)

//	MAY_EFFECT
#define	EFFECT_IRUKASAN_ANIM	ANIMEFFECTDESC(EFFECT_DESC_IRUKASAN_ANIM, EFFECT_SUBDESC_IRUKASAN_ANIM)
#define	EFFECT_IRUKASAN_RETURN	ANIMEFFECTDESC(EFFECT_DESC_IRUKASAN_RETURN, EFFECT_SUBDESC_IRUKASAN_RETURN)
#define EFFECT_IRUKASAN_RIDE	ANIMEFFECTDESC(EFFECT_DESC_IRUKASAN_RIDE, EFFECT_SUBDESC_IRUKASAN_RIDE)
#define EFFECT_IRUKASAN_HIT	ANIMEFFECTDESC(EFFECT_DESC_IRUKASAN_HIT, EFFECT_SUBDESC_IRUKASAN_HIT)
#define EFFECT_IRUKASAN_ROLL	ANIMEFFECTDESC(EFFECT_DESC_IRUKASAN_ROLL, EFFECT_SUBDESC_IRUKASAN_ROLL)
#define EFFECT_MAY_MAYDYNAMIC	ANIMEFFECTDESC(EFFECT_DESC_MAY_MAYDYNAMIC, EFFECT_SUBDESC_MAY_MAYDYNAMIC)
#define EFFECT_ARISUGAWA	ANIMEFFECTDESC(EFFECT_DESC_ARISUGAWA, EFFECT_SUBDESC_ARISUGAWA)

//	JKO_EFFECT
#define EFFECT_IRONMAIDON	ANIMEFFECTDESC(EFFECT_DESC_IRONMAIDON, EFFECT_SUBDESC_IRONMAIDON)
#define EFFECT_JKO_FLAG	ANIMEFFECTDESC(EFFECT_DESC_JKO_FLAG, EFFECT_SUBDESC_JKO_FLAG)

class CEffect_Anim_Effect : public CEffect
{
protected:
	CEffect_Anim_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CEffect_Anim_Effect(const CEffect_Anim_Effect& rhs);
	virtual ~CEffect_Anim_Effect() = default;

public:
	virtual HRESULT					NativeConstruct_Prototype();
	virtual HRESULT					NativeConstruct(void* pArg);
	virtual _int					Tick(_double TimeDelta);
	virtual _int					LateTick(_double TimeDelta);
	virtual HRESULT					Render();

public:
	virtual _bool					Check_Ground(_fvector _vPos);

public:	//트리거 발동시
	virtual void					Trigger_Activate(const _double& _fTimeDelta);

protected:
	HRESULT							SetUp_Components();
	HRESULT							SetUp_ConstantTable_BillBoarding();
	HRESULT							SetUp_ConstantTable();

protected:
	CShader*						m_pShaderCom = nullptr;
	CModel*							m_pModelCom = nullptr;

public:
	static CEffect_Anim_Effect*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject*			Clone(void* pArg);
	virtual void					Free() override;
};

END