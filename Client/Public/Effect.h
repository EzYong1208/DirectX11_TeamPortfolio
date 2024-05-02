#pragma once

#include "Actor.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CRenderer;
END

BEGIN(Client)

typedef struct tagSubEffectDesc
{
	tagSubEffectDesc() = default;
	tagSubEffectDesc(_tchar* _pModel_Tag, _tchar* _pTexture_Tag)
		: pModel_Tag(_pModel_Tag)
		, pTexture_Tag(_pTexture_Tag) {}

	_tchar* pModel_Tag = nullptr;
	_tchar* pTexture_Tag = nullptr;
}SUB_EFFECT_DESC;

typedef struct tagEffectDescription
{
	tagEffectDescription() = default;
	tagEffectDescription(_bool _bOnce
		, LERP_OPTION _eLerp_Option
		, _float _fLifetime
		, _float _fSpeed
		, _int _iEffectPass
		, _int _iTexFrameMax = 0
		, _int _iTexNumX = 0
		, _int _iTexNumY = 0
		, _bool _bScaling = false
		, _bool _bGlow = false
		, _float4 _vRGBA = _float4(1.f, 1.f, 1.f, 1.f)
		, _float4 _vGlowRGBA = _float4(1.f, 1.f, 1.f, 1.f)
		, _int _iEffectIndex = 0)
		: bOnce(_bOnce)
		, eLerp_Option(_eLerp_Option)
		, fLifetime(_fLifetime)
		, fSpeed(_fSpeed)
		, iEffectPass(_iEffectPass)
		, iTexFrameMax(_iTexFrameMax)
		, iTexNumX(_iTexNumX)
		, iTexNumY(_iTexNumY)
		, bScaling(_bScaling)
		, bGlow(_bGlow)
		, vRGBA(_vRGBA)
		, vGlowRGBA(_vGlowRGBA)
		, iEffectIndex(_iEffectIndex) {}

	_bool			bDistortion = false;	//발동용
	_bool			bTrriger = false;	//발동용
	_bool			IsDead = false;		//이펙트제거용
	_bool			bOnce = false;		//한번만 수행할것
	_bool			bScaling = false;
	_bool			bGlow = false;
	_float			fLifetime = 0.f;	//대기시간
	_float			fSpeed = 0.f;
	_float4			vRGBA = { 1.f,1.f,1.f,1.f };
	_float4			vGlowRGBA = { 1.f,1.f,1.f,1.f };
	_float			fAlpha = 0.f;
	_int			iTextureIdx = 0;
	_int			iEffectPass = 0;
	_int			iTexFrameMax = 1;
	_int			iTexNumX = 1;
	_int			iTexNumY = 1;
	_int			iEffectIndex = 0;
	_float3			vScaling = { 1.f,1.f,1.f };
	_float3			vScaleOrigin = { 1.f,1.f,1.f };
	LERP_OPTION		eLerp_Option = LERP_NONE;
	_float4x4		matBillboard;
	CActor*			m_pParent = nullptr;
}EFFECTDESC;

#pragma region LJB

// DNF_SKILL
#define EFFECT_SUBDESC_DNF_SKILL1 SUB_EFFECT_DESC(L"Model_Effect_DNF_Skill1", nullptr)
#define EFFECT_DESC_DNF_SKILL1 EFFECTDESC(true, LERP_NONE, 2.f, 4.f, 4, 6, 6, 1, false, false, _float4(1.5f, 1.7f, 5.f, 1.f))

#define EFFECT_SUBDESC_DNF_SKILL2 SUB_EFFECT_DESC(L"Model_Effect_DNF_Skill2", nullptr)
#define EFFECT_DESC_DNF_SKILL2 EFFECTDESC(true, LERP_NONE, 2.f, 4.f, 10, 1, 1, 1, false, false, _float4(1.5f, 1.7f, 5.f, 1.f))

#define EFFECT_SUBDESC_DNF_SKILL4 SUB_EFFECT_DESC(L"Model_Effect_DNF_Skill4", nullptr)
#define EFFECT_DESC_DNF_SKILL4 EFFECTDESC(true, LERP_NONE, 2.f, 4.f, 10, 1, 1, 1, false, false, _float4(5.f, 1.7f, 1.5f, 1.f))

#define EFFECT_SUBDESC_DNF_SKILL4_MC SUB_EFFECT_DESC(L"Model_Effect_DNF_Skill4_MC", nullptr)
#define EFFECT_DESC_DNF_SKILL4_MC EFFECTDESC(true, LERP_NONE, 5.f, 45.f, 4, 1, 1, 1, true, false, _float4(1.2f, 1.2f, 3.f, 1.f))

// CMN_SMOKE
#define EFFECT_SUBDESC_DASHSMOKE SUB_EFFECT_DESC(L"Model_DashSmoke", nullptr)
#define EFFECT_DESC_DASHSMOKE EFFECTDESC(true, LERP_NONE, 5.f, 25.f, 7)

#define EFFECT_SUBDESC_JUMPSMOKE SUB_EFFECT_DESC(L"Model_JumpSmoke", nullptr)
#define EFFECT_DESC_JUMPSMOKE EFFECTDESC(true, LERP_NONE, 5.f, 35.f, 7)

#define EFFECT_SUBDESC_LANDSMOKE SUB_EFFECT_DESC(L"Model_LandSmoke", nullptr)
#define EFFECT_DESC_LANDSMOKE EFFECTDESC(true, LERP_NONE, 5.f, 45.f, 7)

// CMN_AIRDASH
#define EFFECT_SUBDESC_AIRDASH_MAGICCIRCLE SUB_EFFECT_DESC(L"Model_AirDash_MagicCircle", nullptr)
#define EFFECT_DESC_AIRDASH_MAGICCIRCLE EFFECTDESC(true, LERP_NONE, 5.f, 45.f, 4, 1, 1, 1, true, false, _float4(3.35f, 2.35f, 1.f, 1.f))

#define EFFECT_SUBDESC_AIRDASH_MAGICCIRCLE2 SUB_EFFECT_DESC(L"Model_AirDash_MagicCircle2", nullptr)
#define EFFECT_DESC_AIRDASH_MAGICCIRCLE2 EFFECTDESC(true, LERP_NONE, 5.f, 45.f, 8, 1, 1, 1, false, false)

#define EFFECT_SUBDESC_AIRDASH_SMOKE SUB_EFFECT_DESC(L"Model_AirDash_Smoke", nullptr)
#define EFFECT_DESC_AIRDASH_SMOKE EFFECTDESC(true, LERP_NONE, 2.f, 1.f, 8, 8, 4, 2, false, false)

// CMN_HIT1
#define EFFECT_SUBDESC_HIT1 SUB_EFFECT_DESC(L"Model_Hit1", nullptr)
#define EFFECT_DESC_HIT1 EFFECTDESC(true, LERP_NONE, 5.f, 4.f, 19, 4, 2, 2, false, true, _float4(2.5f, 0.9f, 0.5f, 1.f))

#define EFFECT_SUBDESC_CMN_HIT1_BLUE SUB_EFFECT_DESC(L"Model_Effect_CMN_Hit1_Blue", nullptr)
#define EFFECT_DESC_CMN_HIT1_BLUE EFFECTDESC(true, LERP_NONE, 5.f, 4.f, 28, 4, 2, 2, false, true, _float4(0.5f, 2.5f, 0.9f, 1.f))

// CMN_HIT2
#define EFFECT_SUBDESC_HIT2_CENTER SUB_EFFECT_DESC(L"Model_Hit2_Center", nullptr)
#define EFFECT_DESC_HIT2_CENTER EFFECTDESC(true, LERP_NONE, 3.f, 3.5f, 13, 8, 4, 2, false, true, _float4(2.5f, 0.9f, 0.5f, 1.f))

#define EFFECT_SUBDESC_HIT2_SPARK_0 SUB_EFFECT_DESC(L"Model_Hit2_Spark", nullptr)
#define EFFECT_SUBDESC_HIT2_SPARK_1 SUB_EFFECT_DESC(L"Model_Hit2_Spark_50", nullptr)
#define EFFECT_SUBDESC_HIT2_SPARK_2 SUB_EFFECT_DESC(L"Model_Hit2_Spark_100", nullptr)
#define EFFECT_SUBDESC_HIT2_SPARK_3 SUB_EFFECT_DESC(L"Model_Hit2_Spark_150", nullptr)
#define EFFECT_DESC_HIT2_SPARK EFFECTDESC(true, LERP_NONE, 3.f, 3.f, 12, 7, 0, 0, false, true, _float4(3.f, 1.f, 1.f, 1.f))
#define EFFECT_DESC_HIT2_SPARK2 EFFECTDESC(true, LERP_NONE, 3.f, 3.f, 12, 7, 0, 0, false, true, _float4(3.f, 1.f, 1.f, 1.f))

#define EFFECT_SUBDESC_HIT2_SPARK_BLACK_0 SUB_EFFECT_DESC(L"Model_Hit2_Spark_25", nullptr)
#define EFFECT_SUBDESC_HIT2_SPARK_BLACK_1 SUB_EFFECT_DESC(L"Model_Hit2_Spark_75", nullptr)
#define EFFECT_SUBDESC_HIT2_SPARK_BLACK_2 SUB_EFFECT_DESC(L"Model_Hit2_Spark_125", nullptr)
#define EFFECT_SUBDESC_HIT2_SPARK_BLACK_3 SUB_EFFECT_DESC(L"Model_Hit2_Spark_175", nullptr)
#define EFFECT_DESC_HIT2_SPARK_BLACK EFFECTDESC(true, LERP_NONE, 3.f, 3.f, 10, 7, 0, 0, false, false, _float4(0.f, 0.f, 0.f, 1.f))
#define EFFECT_DESC_HIT2_SPARK_BLACK2 EFFECTDESC(true, LERP_NONE, 3.f, 3.f, 10, 7, 0, 0, false, false, _float4(0.f, 0.f, 0.f, 1.f))

// CMN_HIT3
#define EFFECT_SUBDESC_CMN_HIT3_CENTER SUB_EFFECT_DESC(L"Model_Effect_CMN_Hit3_Center", nullptr)
#define EFFECT_DESC_CMN_HIT3_CENTER EFFECTDESC(true, LERP_NONE, 3.f, 2.f, 13, 4, 4, 1, false, true, _float4(2.5f, 0.9f, 0.5f, 1.f))

#define EFFECT_SUBDESC_CMN_HIT3_SPARK SUB_EFFECT_DESC(L"Model_Effect_CMN_Hit3_Spark", nullptr)
#define EFFECT_DESC_CMN_HIT3_SPARK EFFECTDESC(true, LERP_NONE, 3.f, 5.f, 12, 12, 0, 0, false, true, _float4(3.f, 1.f, 1.f, 1.f))

// CMN_WALLBREAK
#define EFFECT_SUBDESC_CMN_WALLBREAK_MC SUB_EFFECT_DESC(L"Model_Effect_CMN_WallBreak_MC", nullptr)
#define EFFECT_DESC_CMN_WALLBREAK_MC EFFECTDESC(true, LERP_NONE, 5.f, 45.f, 4, 1, 1, 1, true, false, _float4(1.2f, 1.2f, 3.f, 1.f))

#define EFFECT_SUBDESC_CMN_WALLBREAK_MC2 SUB_EFFECT_DESC(L"Model_Effect_CMN_WallBreak_MC2", nullptr)
#define EFFECT_DESC_CMN_WALLBREAK_MC2 EFFECTDESC(true, LERP_NONE, 5.f, 45.f, 8, 1, 1, 1, false, false)

#define EFFECT_SUBDESC_CMN_WALLBREAK_MC3 SUB_EFFECT_DESC(L"Model_Effect_CMN_WallBreak_MC3", nullptr)
#define EFFECT_DESC_CMN_WALLBREAK_MC3 EFFECTDESC(true, LERP_NONE, 5.f, 45.f, 8, 1, 1, 1, false, false, _float4(1.5f, 1.5f, 0.3f, 1.f))

#define EFFECT_SUBDESC_WALLBREAK SUB_EFFECT_DESC(L"Model_WallBreak", nullptr)
#define EFFECT_DESC_WALLBREAK EFFECTDESC(true, LERP_NONE, 0.1f, 2.f, 4, 16, 4, 4, false, false, _float4(1.f, 1.f, 1.f, 1.f))

#define EFFECT_SUBDESC_WALLBREAK2 SUB_EFFECT_DESC(L"Model_WallBreak2", nullptr)
#define EFFECT_DESC_WALLBREAK2 EFFECTDESC(true, LERP_NONE, 0.1f, 2.f, 4, 16, 4, 4, false, false, _float4(2.f, 2.f, 2.f, 1.f))

#define EFFECT_SUBDESC_WALLBREAK3 SUB_EFFECT_DESC(L"Model_WallBreak3", nullptr)
#define EFFECT_DESC_WALLBREAK3 EFFECTDESC(true, LERP_NONE, 1.3f, 2.f, 4, 16, 4, 4, false, false, _float4(3.f, 3.f, 3.f, 1.f))

#define EFFECT_SUBDESC_WALLBREAK_BLINK SUB_EFFECT_DESC(L"Model_Effect_WallBreak_Blink", nullptr)
#define EFFECT_DESC_WALLBREAK_BLINK EFFECTDESC(true, LERP_NONE, 0.5f, 2.f, 4, 1, 1, 1, false, false, _float4(1.f, 1.f, 1.f, 1.f))

#define EFFECT_SUBDESC_WALLBREAK_GLASS SUB_EFFECT_DESC(L"Model_Effect_WallBreak_Glass", nullptr)
#define EFFECT_DESC_WALLBREAK_GLASS EFFECTDESC(true, LERP_NONE, 2.5f, 1.f, 7, 1, 1, 1, false, false, _float4(1.f, 1.f, 1.f, 1.f))

// CMN_GUARD
#define EFFECT_SUBDESC_CMN_GUARD SUB_EFFECT_DESC(L"Model_Effect_CMN_Guard", nullptr)
#define EFFECT_DESC_CMN_GUARD EFFECTDESC(true, LERP_NONE, 5.f, 2.5f, 21, 9, 3, 3, false, true, _float4(1.f, 2.f, 3.f, 1.f))

// CMN_BURST
#define EFFECT_SUBDESC_CMN_BURST SUB_EFFECT_DESC(L"Model_Effect_CMN_Burst", nullptr)
#define EFFECT_DESC_CMN_BURST EFFECTDESC(true, LERP_NONE, 2.f, 4.f, 10, 0, 0, 0, false, false, _float4(1.5f, 1.7f, 5.f, 1.f))

#define EFFECT_SUBDESC_CMN_BURST_SPHERE SUB_EFFECT_DESC(L"Model_Effect_CMN_Burst_Sphere", nullptr)
#define EFFECT_DESC_CMN_BURST_SPHERE EFFECTDESC(true, LERP_NONE, 5.f, 2.5f, 23, 0, 0, 0, true, true, _float4(1.f, 1.f, 3.f, 1.f))

// CMN_FIREANIM8
#define EFFECT_SUBDESC_CMN_FIREANIM8 SUB_EFFECT_DESC(L"Model_Effect_CMN_FireAnim8", nullptr)	
#define EFFECT_DESC_CMN_FIREANIM8 EFFECTDESC(true, LERP_NONE, 2.5f, 3.f, 19, 9, 3, 3, false, true, _float4(1.f, 1.f, 1.f, 1.f))

// CMN_SLASH
#define EFFECT_SUBDESC_CMN_SLASH SUB_EFFECT_DESC(L"Model_Effect_CMN_Slash", nullptr)
#define EFFECT_DESC_CMN_SLASH EFFECTDESC(true, LERP_NONE, 3.f, 1.f, 12, 1, 1, 1, false, true, _float4(2.5f, 2.5f, 2.5f, 1.f))

#define EFFECT_SUBDESC_CMN_SLASH_CENTER SUB_EFFECT_DESC(L"Model_Effect_CMN_Slash_Center", nullptr)
#define EFFECT_DESC_CMN_SLASH_CENTER EFFECTDESC(true, LERP_NONE, 3.f, 2.f, 12, 5, 1, 1, false, true, _float4(2.5f, 2.5f, 2.5f, 1.f))

// KYK_NORMAL
#define EFFECT_SUBDESC_KYK_SWORD_HOR_SLASH SUB_EFFECT_DESC(L"Model_KYK_Sword_Hor_Slash", nullptr)
#define EFFECT_DESC_KYK_SWORD_HOR_SLASH EFFECTDESC(true, LERP_NONE, 5.f, 2.5f, 16, 4, 4, 1, false, true, _float4(0.5f, 0.5f, 1.f, 1.f))

#define EFFECT_SUBDESC_KYK_SWORD_HOR_SLASH_6H SUB_EFFECT_DESC(L"Model_KYK_Sword_Hor_Slash_6H", nullptr)
#define EFFECT_DESC_KYK_SWORD_HOR_SLASH_6H EFFECTDESC(true, LERP_NONE, 5.f, 2.5f, 16, 4, 4, 1, false, true, _float4(0.5f, 0.5f, 1.f, 1.f))

#define EFFECT_SUBDESC_KYK_5D SUB_EFFECT_DESC(L"Model_Effect_KYK_5D", nullptr)
#define EFFECT_DESC_KYK_5D EFFECTDESC(true, LERP_NONE, 5.f, 2.5f, 16, 4, 4, 1, false, true, _float4(0.5f, 0.5f, 1.f, 1.f))

#define EFFECT_SUBDESC_KYK_SWORD_SIT_SLASH SUB_EFFECT_DESC(L"Model_KYK_Sword_Sit_Slash", nullptr)
#define EFFECT_DESC_KYK_SWORD_SIT_SLASH EFFECTDESC(true, LERP_NONE, 5.f, 2.5f, 16, 4, 4, 1, false, true, _float4(0.5f, 0.5f, 1.f, 1.f))

#define EFFECT_SUBDESC_KYK_SWORD_SLASH_NEAR SUB_EFFECT_DESC(L"Model_KYK_Sword_Slash_Near", nullptr)
#define EFFECT_DESC_KYK_SWORD_SLASH_NEAR EFFECTDESC(true, LERP_NONE, 5.f, 2.5f, 16, 4, 4, 1, false, true, _float4(0.5f, 0.5f, 1.f, 1.f))

#define EFFECT_SUBDESC_KYK_SWORD_STAB SUB_EFFECT_DESC(L"Model_KYK_Sword_Stab", nullptr)
#define EFFECT_DESC_KYK_SWORD_STAB EFFECTDESC(true, LERP_NONE, 5.f, 2.5f, 15, 0, 0, 0, false, true, _float4(0.5f, 0.5f, 1.f, 1.f))

#define EFFECT_SUBDESC_KYK_SWORD_UPPER SUB_EFFECT_DESC(L"Model_KYK_Sword_Upper", nullptr)
#define EFFECT_DESC_KYK_SWORD_UPPER EFFECTDESC(true, LERP_NONE, 5.f, 2.5f, 16, 4, 4, 1, false, true, _float4(0.5f, 0.5f, 1.f, 1.f))

#define EFFECT_SUBDESC_KYK_JH SUB_EFFECT_DESC(L"Model_Effect_KYK_jH", nullptr)
#define EFFECT_DESC_KYK_JH EFFECTDESC(true, LERP_NONE, 5.f, 2.5f, 16, 4, 4, 1, false, true, _float4(0.5f, 0.5f, 1.f, 1.f))

#define EFFECT_SUBDESC_KYK_JS SUB_EFFECT_DESC(L"Model_Effect_KYK_jS", nullptr)
#define EFFECT_DESC_KYK_JS EFFECTDESC(true, LERP_NONE, 5.f, 2.5f, 16, 4, 4, 1, false, true, _float4(0.5f, 0.5f, 1.f, 1.f))

// KYK_SPECIAL
#define EFFECT_SUBDESC_KYK_SWORD_STUN_DIPPER SUB_EFFECT_DESC(L"Model_KYK_Sword_Stun_Dipper", nullptr)
#define EFFECT_DESC_KYK_SWORD_STUN_DIPPER EFFECTDESC(true, LERP_NONE, 5.f, 2.5f, 17, 20, 10, 2, false, true, _float4(1.f, 3.f, 3.f, 1.f))

#define EFFECT_SUBDESC_KYK_FOUDREARC SUB_EFFECT_DESC(L"Model_Effect_KYK_FoudreArc", nullptr)
#define EFFECT_DESC_KYK_FOUDREARC EFFECTDESC(true, LERP_NONE, 5.f, 4.f, 40, 8, 8, 1, false, true, _float4(2.f, 1.f, 3.f, 1.f))

#define EFFECT_SUBDESC_KYK_VAPORTHRUST_S SUB_EFFECT_DESC(L"Model_KYK_VaporThrust_S", nullptr)
#define EFFECT_DESC_KYK_VAPORTHRUST_S EFFECTDESC(true, LERP_NONE, 5.f, 2.5f, 17, 20, 10, 2, false, true, _float4(1.f, 3.f, 3.f, 1.f))

#define EFFECT_SUBDESC_KYK_VAPORTHRUST_H SUB_EFFECT_DESC(L"Model_KYK_VaporThrust_H", nullptr)
#define EFFECT_DESC_KYK_VAPORTHRUST_H EFFECTDESC(true, LERP_NONE, 5.f, 2.5f, 17, 20, 10, 2, false, true, _float4(1.f, 3.f, 3.f, 1.f))

#define EFFECT_SUBDESC_KYK_DIREECLAT SUB_EFFECT_DESC(L"Model_KYK_DireEclat", nullptr)
#define EFFECT_DESC_KYK_DIREECLAT EFFECTDESC(true, LERP_NONE, 5.f, 2.5f, 19, 8, 8, 1, false, true, _float4(3.f, 2.f, 1.f, 1.f))

// KYK_OVERDRIVE
#define EFFECT_SUBDESC_KYK_RIDETHELIGHTNING_CSBG SUB_EFFECT_DESC(L"Model_Effect_KYK_RidetheLightning_CSBG", nullptr)
#define EFFECT_DESC_KYK_RIDETHELIGHTNING_CSBG EFFECTDESC(true, LERP_NONE, 3.f, 3.f, 4, 32, 4, 8, false, false, _float4(0.95f, 1.f, 1.f, 1.f))

#define EFFECT_SUBDESC_KYK_RIDETHELIGHTNING_CSSPARK SUB_EFFECT_DESC(L"Model_Effect_KYK_RidetheLightning_CSSpark", nullptr)
#define EFFECT_DESC_KYK_RIDETHELIGHTNING_CSSPARK EFFECTDESC(true, LERP_NONE, 3.f, 2.5f, 26, 7, 1, 1, false, true, _float4(0.5f, 3.f, 3.f, 1.f))

#define EFFECT_SUBDESC_KYK_RIDETHELIGHTNING_CHARGESPARK SUB_EFFECT_DESC(L"Model_Effect_KYK_RidetheLightning_ChargeSpark", nullptr)
#define EFFECT_DESC_KYK_RIDETHELIGHTNING_CHARGESPARK EFFECTDESC(true, LERP_NONE, 3.f, 2.5f, 21, 8, 8, 1, false, true, _float4(3.f, 3.f, 3.f, 1.f))

#define EFFECT_SUBDESC_KYK_RIDETHELIGHTNING_FLOORSPARK SUB_EFFECT_DESC(L"Model_Effect_KYK_RidetheLightning_FloorSpark", nullptr)
#define EFFECT_DESC_KYK_RIDETHELIGHTNING_FLOORSPARK EFFECTDESC(true, LERP_NONE, 3.f, 2.f, 23, 8, 8, 1, false, true, _float4(3.f, 3.f, 3.f, 1.f))

#define EFFECT_SUBDESC_KYK_RIDETHELIGHTNING_MCDOME SUB_EFFECT_DESC(L"Model_Effect_KYK_RidetheLightning_MCDome", nullptr)
#define EFFECT_DESC_KYK_RIDETHELIGHTNING_MCDOME EFFECTDESC(true, LERP_NONE, 3.f, 2.f, 7, 1, 1, 1, false, false, _float4(3.f, 3.f, 3.f, 1.f))

#define EFFECT_SUBDESC_KYK_RIDETHELIGHTNING_CORE SUB_EFFECT_DESC(L"Model_Effect_KYK_RidetheLightning_Core", nullptr)
#define EFFECT_DESC_KYK_RIDETHELIGHTNING_CORE EFFECTDESC(true, LERP_NONE, 3.f, 2.f, 41, 1, 1, 1, false, true, _float4(1.f, 2.f, 3.f, 1.f))

#define EFFECT_SUBDESC_KYK_RIDETHELIGHTNING_SLASH SUB_EFFECT_DESC(L"Model_Effect_KYK_RidetheLightning_Slash", nullptr)
#define EFFECT_DESC_KYK_RIDETHELIGHTNING_SLASH EFFECTDESC(true, LERP_NONE, 3.f, 2.5f, 21, 8, 2, 4, false, true, _float4(1.f, 2.f, 3.f, 1.f))

#define EFFECT_SUBDESC_KYK_RIDETHELIGHTNING_THUNDERBOLT SUB_EFFECT_DESC(L"Model_Effect_KYK_RidetheLightning_ThunderBolt", nullptr)
#define EFFECT_DESC_KYK_RIDETHELIGHTNING_THUNDERBOLT EFFECTDESC(true, LERP_NONE, 3.f, 2.f, 23, 8, 8, 1, false, true, _float4(3.f, 3.f, 3.f, 1.f))

#define EFFECT_SUBDESC_KYK_RIDETHELIGHTNING_SMOKE SUB_EFFECT_DESC(L"Model_DashSmoke", nullptr)
#define EFFECT_DESC_KYK_RIDETHELIGHTNING_SMOKE EFFECTDESC(true, LERP_NONE, 3.f, 25.f, 10, 0, 0, 0, false, false, _float4(0.1f, 0.1f, 0.3f, 1.f))

#define EFFECT_SUBDESC_KYK_SACREDEDGE_CIRCLE SUB_EFFECT_DESC(L"Model_Effect_KYK_SacredEdge_Circle", nullptr)
#define EFFECT_DESC_KYK_SACREDEDGE_CIRCLE EFFECTDESC(true, LERP_NONE, 3.f, 2.f, 35, 9, 3, 3, false, true, _float4(3.f, 3.f, 0.5f, 1.f))

#define EFFECT_SUBDESC_KYK_SACREDEDGE_BULLET SUB_EFFECT_DESC(L"Model_Effect_KYK_SacredEdge_Bullet", nullptr)
#define EFFECT_DESC_KYK_SACREDEDGE_BULLET EFFECTDESC(true, LERP_NONE, 3.f, 2.f, 35, 1, 1, 1, false, true, _float4(3.f, 3.f, 1.f, 1.f))

#define EFFECT_SUBDESC_KYK_SACREDEDGE_SPARK SUB_EFFECT_DESC(L"Model_Effect_KYK_SacredEdge_Spark", nullptr)
#define EFFECT_DESC_KYK_SACREDEDGE_SPARK EFFECTDESC(true, LERP_NONE, 3.f, 2.f, 35, 10, 2, 5, false, true, _float4(2.f, 2.f, 1.f, 1.f))

#define EFFECT_SUBDESC_KYK_SACREDEDGE_MC SUB_EFFECT_DESC(L"Model_Effect_KYK_SacredEdge_MC", nullptr)
#define EFFECT_DESC_KYK_SACREDEDGE_MC EFFECTDESC(true, LERP_NONE, 3.f, 2.f, 35, 1, 1, 1, false, true, _float4(1.f, 1.f, 1.f, 1.f))

#define EFFECT_SUBDESC_KYK_DRAIN_STAND SUB_EFFECT_DESC(L"Model_KYK_Drain_Stand", nullptr)
#define EFFECT_DESC_KYK_DRAIN_STAND EFFECTDESC(true, LERP_NONE, 0.f, 20.f, 11, 16, 2, 8, false, true, _float4(1.f, 1.f, 1.f, 1.f))

// RAM_NORMAL
#define EFFECT_SUBDESC_RAM_5P SUB_EFFECT_DESC(L"Model_Effect_RAM_5P", nullptr)
#define EFFECT_DESC_RAM_5P EFFECTDESC(true, LERP_NONE, 3.f, 3.f, 19, 8, 4, 2, false, true, _float4(3.f, 2.f, 1.f, 1.f))

#define EFFECT_SUBDESC_RAM_FS SUB_EFFECT_DESC(L"Model_Effect_RAM_fS", nullptr)
#define EFFECT_DESC_RAM_FS EFFECTDESC(true, LERP_NONE, 5.f, 2.5f, 16, 8, 2, 4, false, false, _float4(1.f, 1.f, 1.f, 1.f))

#define EFFECT_SUBDESC_RAM_FS_CLAW SUB_EFFECT_DESC(L"Model_Effect_RAM_fS_Claw", nullptr)
#define EFFECT_DESC_RAM_FS_CLAW EFFECTDESC(true, LERP_NONE, 3.f, 3.f, 19, 8, 4, 2, false, true, _float4(3.f, 2.f, 1.f, 1.f))

#define EFFECT_SUBDESC_RAM_5H SUB_EFFECT_DESC(L"Model_Effect_RAM_5H", nullptr)
#define EFFECT_DESC_RAM_5H EFFECTDESC(true, LERP_NONE, 5.f, 2.5f, 16, 8, 2, 4, false, false, _float4(1.f, 1.f, 1.f, 1.f))

#define EFFECT_SUBDESC_RAM_5H_CLAW SUB_EFFECT_DESC(L"Model_Effect_RAM_5H_Claw", nullptr)
#define EFFECT_DESC_RAM_5H_CLAW EFFECTDESC(true, LERP_NONE, 3.f, 3.f, 19, 8, 4, 2, false, true, _float4(3.f, 2.f, 1.f, 1.f))

#define EFFECT_SUBDESC_RAM_5D SUB_EFFECT_DESC(L"Model_Effect_RAM_5D", nullptr)
#define EFFECT_DESC_RAM_5D EFFECTDESC(true, LERP_NONE, 3.f, 3.f, 19, 6, 6, 1, false, true, _float4(3.f, 2.f, 1.f, 1.f))

#define EFFECT_SUBDESC_RAM_5D_KIRA SUB_EFFECT_DESC(L"Model_Effect_RAM_5D_Kira", nullptr)
#define EFFECT_DESC_RAM_5D_KIRA EFFECTDESC(true, LERP_NONE, 3.f, 4.f, 24, 6, 0, 0, false, true, _float4(2.f, 2.f, 2.f, 1.f))

#define EFFECT_SUBDESC_RAM_2P SUB_EFFECT_DESC(L"Model_Effect_RAM_2P", nullptr)
#define EFFECT_DESC_RAM_2P EFFECTDESC(true, LERP_NONE, 3.f, 3.f, 19, 8, 4, 2, false, true, _float4(3.f, 2.f, 1.f, 1.f))

#define EFFECT_SUBDESC_RAM_2S SUB_EFFECT_DESC(L"Model_Effect_RAM_2S", nullptr)
#define EFFECT_DESC_RAM_2S1 EFFECTDESC(true, LERP_NONE, 3.f, 3.f, 19, 8, 4, 2, false, true, _float4(3.f, 2.f, 1.f, 1.f))
#define EFFECT_DESC_RAM_2S2 EFFECTDESC(true, LERP_NONE, 3.f, 3.f, 19, 8, 2, 4, false, true, _float4(1.f, 0.777f, 0.333f, 1.f))

#define EFFECT_SUBDESC_RAM_2S_CLAW SUB_EFFECT_DESC(L"Model_Effect_RAM_2S_Claw", nullptr)
#define EFFECT_DESC_RAM_2S_CLAW EFFECTDESC(true, LERP_NONE, 3.f, 3.f, 19, 6, 6, 1, false, true, _float4(3.f, 2.f, 1.f, 1.f))

#define EFFECT_SUBDESC_RAM_2H SUB_EFFECT_DESC(L"Model_Effect_RAM_2H", nullptr)
#define EFFECT_DESC_RAM_2H EFFECTDESC(true, LERP_NONE, 3.f, 3.f, 19, 8, 2, 4, false, true, _float4(1.f, 0.777f, 0.333f, 1.f))

#define EFFECT_SUBDESC_RAM_2H_CLAW SUB_EFFECT_DESC(L"Model_Effect_RAM_2H_Claw", nullptr)
#define EFFECT_DESC_RAM_2H_CLAW EFFECTDESC(true, LERP_NONE, 3.f, 3.f, 19, 6, 6, 1, false, true, _float4(3.f, 2.f, 1.f, 1.f))

#define EFFECT_SUBDESC_RAM_6H SUB_EFFECT_DESC(L"Model_Effect_RAM_6H", nullptr)
#define EFFECT_DESC_RAM_6H EFFECTDESC(true, LERP_NONE, 3.f, 3.f, 4, 4, 4, 1, false, false, _float4(1.f, 1.f, 1.f, 1.f))

#define EFFECT_SUBDESC_RAM_6H_CLAW SUB_EFFECT_DESC(L"Model_Effect_RAM_6H_Claw", nullptr)
#define EFFECT_DESC_RAM_6H_CLAW EFFECTDESC(true, LERP_NONE, 3.f, 3.f, 19, 8, 4, 2, false, true, _float4(3.f, 2.f, 1.f, 1.f))

#define EFFECT_SUBDESC_RAM_JP SUB_EFFECT_DESC(L"Model_Effect_RAM_jP", nullptr)
#define EFFECT_DESC_RAM_JP EFFECTDESC(true, LERP_NONE, 3.f, 3.f, 19, 8, 4, 2, false, true, _float4(3.f, 2.f, 1.f, 1.f))

#define EFFECT_SUBDESC_RAM_JS SUB_EFFECT_DESC(L"Model_Effect_RAM_jS", nullptr)
#define EFFECT_DESC_RAM_JS EFFECTDESC(true, LERP_NONE, 5.f, 2.5f, 16, 8, 2, 4, false, false, _float4(1.f, 1.f, 1.f, 1.f))

#define EFFECT_SUBDESC_RAM_JS_CLAW SUB_EFFECT_DESC(L"Model_Effect_RAM_jS_Claw", nullptr)
#define EFFECT_DESC_RAM_JS_CLAW EFFECTDESC(true, LERP_NONE, 3.f, 3.f, 19, 8, 4, 2, false, true, _float4(3.f, 2.f, 1.f, 1.f))

#define EFFECT_SUBDESC_RAM_JH SUB_EFFECT_DESC(L"Model_Effect_RAM_jH", nullptr)
#define EFFECT_DESC_RAM_JH EFFECTDESC(true, LERP_NONE, 5.f, 2.5f, 16, 8, 8, 1, false, false, _float4(1.f, 1.f, 1.f, 1.f))

#define EFFECT_SUBDESC_RAM_JH_CLAW SUB_EFFECT_DESC(L"Model_Effect_RAM_jH_Claw", nullptr)
#define EFFECT_DESC_RAM_JH_CLAW EFFECTDESC(true, LERP_NONE, 3.f, 3.f, 19, 9, 3, 3, false, true, _float4(3.f, 2.f, 1.f, 1.f))

#define EFFECT_SUBDESC_RAM_JD SUB_EFFECT_DESC(L"Model_Effect_RAM_jD", nullptr)
#define EFFECT_DESC_RAM_JD EFFECTDESC(true, LERP_NONE, 3.f, 3.f, 19, 4, 4, 1, false, true, _float4(3.f, 2.f, 1.f, 1.f))

// RAM_SPECIAL
#define EFFECT_SUBDESC_RAM_DAURO SUB_EFFECT_DESC(L"Model_Effect_RAM_Dauro", nullptr)
#define EFFECT_DESC_RAM_DAURO EFFECTDESC(true, LERP_NONE, 3.f, 3.f, 24, 8, 0, 0, false, true, _float4(1.f, 1.f, 1.f, 1.f))

#define EFFECT_SUBDESC_RAM_DAURO_BACK SUB_EFFECT_DESC(L"Model_Effect_RAM_Dauro_Back", nullptr)
#define EFFECT_DESC_RAM_DAURO_BACK EFFECTDESC(true, LERP_NONE, 3.f, 3.f, 4, 8, 2, 4, false, false, _float4(1.f, 1.f, 1.f, 1.f))

#define EFFECT_SUBDESC_RAM_DAURO_CIRCLE SUB_EFFECT_DESC(L"Model_Effect_RAM_Dauro_Circle", nullptr)
#define EFFECT_DESC_RAM_DAURO_CIRCLE EFFECTDESC(true, LERP_NONE, 3.f, 3.f, 10, 0, 0, 0, true, false, _float4(0.1f, 0.f, 0.f, 1.f))

#define EFFECT_SUBDESC_RAM_ERARLUMO_1 SUB_EFFECT_DESC(L"Model_Effect_RAM_Erarlumo_1", nullptr)
#define EFFECT_DESC_RAM_ERARLUMO_1 EFFECTDESC(true, LERP_NONE, 3.f, 3.f, 4, 6, 6, 1, false, false, _float4(1.f, 1.f, 1.f, 1.f))

#define EFFECT_SUBDESC_RAM_ERARLUMO_1_CIRCLE SUB_EFFECT_DESC(L"Model_Effect_RAM_Erarlumo_1_Circle", nullptr)
#define EFFECT_DESC_RAM_ERARLUMO_1_CIRCLE EFFECTDESC(true, LERP_NONE, 3.f, 3.f, 10, 0, 0, 0, true, false, _float4(0.f, 0.f, 0.f, 1.f))

#define EFFECT_SUBDESC_RAM_ERARLUMO_2 SUB_EFFECT_DESC(L"Model_Effect_RAM_Erarlumo_2", nullptr)
#define EFFECT_DESC_RAM_ERARLUMO_2 EFFECTDESC(true, LERP_NONE, 3.f, 3.f, 4, 6, 6, 1, false, false, _float4(2.f, 1.8f, 1.2f, 1.f))

#define EFFECT_SUBDESC_RAM_ERARLUMO_3 SUB_EFFECT_DESC(L"Model_Effect_RAM_Erarlumo_3", nullptr)
#define EFFECT_DESC_RAM_ERARLUMO_3 EFFECTDESC(true, LERP_NONE, 3.f, 3.f, 4, 6, 6, 1, false, false, _float4(2.f, 1.8f, 1.2f, 1.f))

#define EFFECT_SUBDESC_RAM_SILDODETRUO SUB_EFFECT_DESC(L"Model_Effect_RAM_SildoDetruo", nullptr)
#define EFFECT_DESC_RAM_SILDODETRUO EFFECTDESC(true, LERP_NONE, 3.f, 4.f, 19, 4, 4, 1, false, true, _float4(3.f, 2.f, 1.f, 1.f))

#define EFFECT_SUBDESC_RAM_BAJONETO SUB_EFFECT_DESC(L"Model_Effect_RAM_Bajoneto", nullptr)
#define EFFECT_DESC_RAM_BAJONETO EFFECTDESC(true, LERP_NONE, 3.f, 2.f, 25, 0, 0, 0, false, true, _float4(2.f, 2.f, 2.f, 1.f))

#define EFFECT_SUBDESC_RAM_BAJONETO_SMOKE SUB_EFFECT_DESC(L"Model_Effect_RAM_Bajoneto_Smoke", nullptr)
#define EFFECT_DESC_RAM_BAJONETO_SMOKE EFFECTDESC(true, LERP_NONE, 3.f, 2.f, 10, 21, 0, 0, false, false, _float4(0.03f, 0.f, 0.f, 1.f))

#define EFFECT_SUBDESC_RAM_BAJONETO_SMOKE_BACK SUB_EFFECT_DESC(L"Model_Effect_RAM_Bajoneto_Smoke", nullptr)
#define EFFECT_DESC_RAM_BAJONETO_SMOKE_BACK EFFECTDESC(true, LERP_NONE, 3.f, 5.f, 10, 21, 0, 0, false, false, _float4(1.f, 0.65f, 0.8f, 1.f))

#define EFFECT_SUBDESC_RAM_BAJONETO_SPARK SUB_EFFECT_DESC(L"Model_Effect_RAM_Bajoneto_Spark", nullptr)
#define EFFECT_DESC_RAM_BAJONETO_SPARK EFFECTDESC(true, LERP_NONE, 3.f, 7.f, 24, 6, 0, 0, false, true, _float4(2.f, 2.f, 2.f, 1.f))

#define EFFECT_SUBDESC_RAM_KIRA SUB_EFFECT_DESC(L"Model_Effect_RAM_Kira", nullptr)
#define EFFECT_SUBDESC_RAM_KIRA_R1 SUB_EFFECT_DESC(L"Model_Effect_RAM_Kira_R1", nullptr)
#define EFFECT_SUBDESC_RAM_KIRA_L1 SUB_EFFECT_DESC(L"Model_Effect_RAM_Kira_L1", nullptr)
#define EFFECT_DESC_RAM_KIRA EFFECTDESC(true, LERP_NONE, 3.f, 4.f, 25, 6, 0, 0, false, true, _float4(2.f, 2.f, 2.f, 1.f))

#define EFFECT_SUBDESC_RAM_SUMMONSWORD SUB_EFFECT_DESC(L"Model_Effect_RAM_SummonSword", nullptr)
#define EFFECT_SUBDESC_RAM_SUMMONSWORD_R1 SUB_EFFECT_DESC(L"Model_Effect_RAM_SummonSword_R1", nullptr)
#define EFFECT_SUBDESC_RAM_SUMMONSWORD_R2 SUB_EFFECT_DESC(L"Model_Effect_RAM_SummonSword_R2", nullptr)
#define EFFECT_SUBDESC_RAM_SUMMONSWORD_R3 SUB_EFFECT_DESC(L"Model_Effect_RAM_SummonSword_R3", nullptr)
#define EFFECT_SUBDESC_RAM_SUMMONSWORD_L1 SUB_EFFECT_DESC(L"Model_Effect_RAM_SummonSword_L1", nullptr)
#define EFFECT_SUBDESC_RAM_SUMMONSWORD_L2 SUB_EFFECT_DESC(L"Model_Effect_RAM_SummonSword_L2", nullptr)
#define EFFECT_SUBDESC_RAM_SUMMONSWORD_L3 SUB_EFFECT_DESC(L"Model_Effect_RAM_SummonSword_L3", nullptr)
#define EFFECT_DESC_RAM_SUMMONSWORD EFFECTDESC(true, LERP_NONE, 3.f, 3.f, 16, 8, 2, 4, false, false, _float4(1.f, 3.f, 1.1f, 1.f))

#define EFFECT_SUBDESC_RAM_SUMMONSWORD_EQUIP SUB_EFFECT_DESC(L"Model_Effect_RAM_SummonSword_Equip", nullptr)
#define EFFECT_SUBDESC_RAM_SUMMONSWORD_EQUIP_R1 SUB_EFFECT_DESC(L"Model_Effect_RAM_SummonSword_Equip_R1", nullptr)
#define EFFECT_SUBDESC_RAM_SUMMONSWORD_EQUIP_R2 SUB_EFFECT_DESC(L"Model_Effect_RAM_SummonSword_Equip_R2", nullptr)
#define EFFECT_SUBDESC_RAM_SUMMONSWORD_EQUIP_L1 SUB_EFFECT_DESC(L"Model_Effect_RAM_SummonSword_Equip_L1", nullptr)
#define EFFECT_SUBDESC_RAM_SUMMONSWORD_EQUIP_L2 SUB_EFFECT_DESC(L"Model_Effect_RAM_SummonSword_Equip_L2", nullptr)
#define EFFECT_DESC_RAM_SUMMONSWORD_EQUIP EFFECTDESC(true, LERP_NONE, 3.f, 2.f, 16, 8, 2, 4, false, false, _float4(1.f, 3.f, 1.1f, 1.f))

#define EFFECT_SUBDESC_RAM_AGRESAORDONO SUB_EFFECT_DESC(L"Model_Effect_RAM_AgresaOrdono", nullptr)
#define EFFECT_DESC_RAM_AGRESAORDONO EFFECTDESC(true, LERP_NONE, 5.f, 2.5f, 28, 8, 2, 4, false, true, _float4(1.f, 3.f, 1.1f, 1.f))

#define EFFECT_SUBDESC_RAM_AGRESAORDONO_CLAW SUB_EFFECT_DESC(L"Model_Effect_RAM_AgresaOrdono_Claw", nullptr)
#define EFFECT_DESC_RAM_AGRESAORDONO_CLAW EFFECTDESC(true, LERP_NONE, 3.f, 3.f, 28/*19*/, 10, 5, 2, false, true, _float4(1.f, 3.f, 1.1f, 1.f)/*_float4(3.f, 2.f, 1.f, 1.f)*/)

#define EFFECT_SUBDESC_RAM_SABROBATO SUB_EFFECT_DESC(L"Model_Effect_RAM_Sabrobato", nullptr)
#define EFFECT_DESC_RAM_SABROBATO EFFECTDESC(true, LERP_NONE, 5.f, 2.5f, 28, 8, 2, 4, false, true, _float4(1.f, 3.f, 1.1f, 1.f))

#define EFFECT_SUBDESC_RAM_SABROBATO_CLAW SUB_EFFECT_DESC(L"Model_Effect_RAM_Sabrobato_Claw", nullptr)
#define EFFECT_DESC_RAM_SABROBATO_CLAW EFFECTDESC(true, LERP_NONE, 3.f, 3.f, 28/*19*/, 10, 5, 2, false, true, _float4(1.f, 3.f, 1.1f, 1.f)/*_float4(3.f, 2.f, 1.f, 1.f)*/)

// RAM_OVERDRIVE
#define EFFECT_SUBDESC_RAM_CALVADOS_BG_BLACK SUB_EFFECT_DESC(L"Model_Effect_RAM_Calvados_BG_Black", nullptr)
#define EFFECT_DESC_RAM_CALVADOS_BG_BLACK EFFECTDESC(true, LERP_NONE, 1.5f, 12.f, 27, 1, 1, 1, false, false, _float4(0.f, 0.03f, 0.f, 1.f))

#define EFFECT_SUBDESC_RAM_CALVADOS_BG_SPACE1 SUB_EFFECT_DESC(L"Model_Effect_RAM_Calvados_BG_Space1", nullptr)
#define EFFECT_SUBDESC_RAM_CALVADOS_BG_SPACE2 SUB_EFFECT_DESC(L"Model_Effect_RAM_Calvados_BG_Space2", nullptr)
#define EFFECT_SUBDESC_RAM_CALVADOS_BG_SPACE3 SUB_EFFECT_DESC(L"Model_Effect_RAM_Calvados_BG_Space3", nullptr)
#define EFFECT_DESC_RAM_CALVADOS_BG_SPACE1 EFFECTDESC(true, LERP_NONE, 1.5f, 3.f, 27, 1, 1, 1, false, false, _float4(2.f, 2.f, 2.f, 1.f))
#define EFFECT_DESC_RAM_CALVADOS_BG_SPACE2 EFFECTDESC(true, LERP_NONE, 1.5f, 3.f, 27, 1, 1, 1, false, false, _float4(2.f, 2.f, 2.f, 1.f))
#define EFFECT_DESC_RAM_CALVADOS_BG_SPACE3 EFFECTDESC(true, LERP_NONE, 1.5f, 3.f, 28, 1, 1, 1, false, false, _float4(2.f, 2.f, 2.f, 1.f))

#define EFFECT_SUBDESC_RAM_CALVADOS_SMOKE1 SUB_EFFECT_DESC(L"Model_Effect_RAM_Calvados_Smoke1", nullptr)
#define EFFECT_SUBDESC_RAM_CALVADOS_SMOKE2 SUB_EFFECT_DESC(L"Model_Effect_RAM_Calvados_Smoke2", nullptr)
#define EFFECT_DESC_RAM_CALVADOS_SMOKE1 EFFECTDESC(true, LERP_NONE, 1.5f, 12.f, 28, 10, 2, 5, false, true, _float4(1.f, 3.f, 1.1f, 1.f))
#define EFFECT_DESC_RAM_CALVADOS_SMOKE2 EFFECTDESC(true, LERP_NONE, 1.5f, 12.f, 28, 10, 2, 5, false, true, _float4(1.f, 3.f, 1.1f, 1.f))

#define EFFECT_SUBDESC_RAM_CALVADOS_CIRCLE SUB_EFFECT_DESC(L"Model_Effect_RAM_Calvados_Circle", nullptr)
#define EFFECT_DESC_RAM_CALVADOS_CIRCLE EFFECTDESC(true, LERP_NONE, 1.5f, 10.f, 4, 1, 1, 1, false, false, _float4(0.4f, 1.f, 0.45f, 1.f))

#define EFFECT_SUBDESC_RAM_CALVADOS_BEAM SUB_EFFECT_DESC(L"Model_Effect_RAM_Calvados_Beam", nullptr)
#define EFFECT_DESC_RAM_CALVADOS_BEAM1 EFFECTDESC(true, LERP_NONE, 1.5f, 10.f, 28, 1, 1, 1, false, true, _float4(2.f, 2.f, 2.f, 1.f))
#define EFFECT_DESC_RAM_CALVADOS_BEAM2 EFFECTDESC(true, LERP_NONE, 1.5f, 10.f, 10, 1, 1, 1, false, false, _float4(0.f, 0.03f, 0.f, 1.f))
#define EFFECT_DESC_RAM_CALVADOS_BEAM3 EFFECTDESC(true, LERP_NONE, 1.5f, 10.f, 28, 1, 1, 1, false, true, _float4(2.f, 2.f, 2.f, 1.f))
#define EFFECT_DESC_RAM_CALVADOS_BEAM4 EFFECTDESC(true, LERP_NONE, 1.5f, 10.f, 10, 1, 1, 1, false, false, _float4(0.f, 0.03f, 0.f, 1.f))

#define EFFECT_SUBDESC_RAM_CALVADOS_RING SUB_EFFECT_DESC(L"Model_Effect_RAM_Calvados_Ring", nullptr)
#define EFFECT_DESC_RAM_CALVADOS_RING EFFECTDESC(true, LERP_NONE, 1.5f, 15.f, 28, 8, 1, 8, false, false, _float4(3.f, 3.f, 3.f, 1.f))

#define EFFECT_SUBDESC_RAM_MORTOBATO SUB_EFFECT_DESC(L"Model_Effect_RAM_Mortobato", nullptr)
#define EFFECT_DESC_RAM_MORTOBATO EFFECTDESC(true, LERP_NONE, 3.f, 3.f, 28, 12, 6, 2, false, true, _float4(1.f, 3.f, 1.1f, 1.f))

// NAG_NORMAL
#define EFFECT_SUBDESC_NAG_CS SUB_EFFECT_DESC(L"Model_Effect_NAG_cS", nullptr)
#define EFFECT_DESC_NAG_CS EFFECTDESC(true, LERP_NONE, 2.f, 8.5f, 29, 4, 4, 1, false, false, _float4(1.5f, 1.5f, 1.5f, 1.f))
#define EFFECT_DESC_NAG_CS_BLOOD EFFECTDESC(true, LERP_NONE, 2.f, 8.5f, 29, 4, 4, 1, false, false, _float4(1.5f, 0.1f, 0.1f, 1.f))

#define EFFECT_SUBDESC_NAG_FSS SUB_EFFECT_DESC(L"Model_Effect_NAG_fSS", nullptr)
#define EFFECT_DESC_NAG_FSS EFFECTDESC(true, LERP_NONE, 2.f, 12.f, 29, 4, 4, 1, false, false, _float4(1.5f, 1.5f, 1.5f, 1.f))
#define EFFECT_DESC_NAG_FSS_BLOOD EFFECTDESC(true, LERP_NONE, 2.f, 12.f, 29, 4, 4, 1, false, false, _float4(1.5f, 0.1f, 0.1f, 1.f))

#define EFFECT_SUBDESC_NAG_FSSS SUB_EFFECT_DESC(L"Model_Effect_NAG_fSSS", nullptr)
#define EFFECT_DESC_NAG_FSSS EFFECTDESC(true, LERP_NONE, 2.f, 3.f, 29, 4, 4, 1, false, false, _float4(1.5f, 1.5f, 1.5f, 1.f))
#define EFFECT_DESC_NAG_FSSS_BLOOD EFFECTDESC(true, LERP_NONE, 2.f, 3.f, 29, 4, 4, 1, false, false, _float4(1.5f, 0.1f, 0.1f, 1.f))

#define EFFECT_SUBDESC_NAG_5H SUB_EFFECT_DESC(L"Model_Effect_NAG_5H", nullptr)
#define EFFECT_DESC_NAG_5H EFFECTDESC(true, LERP_NONE, 5.f, 5.f, 29, 4, 4, 1, false, false, _float4(1.5f, 1.5f, 1.5f, 1.f))
#define EFFECT_DESC_NAG_5H_BLOOD EFFECTDESC(true, LERP_NONE, 5.f, 5.f, 29, 4, 4, 1, false, false, _float4(1.5f, 0.1f, 0.1f, 1.f))

#define EFFECT_SUBDESC_NAG_5D SUB_EFFECT_DESC(L"Model_Effect_NAG_5D", nullptr)
#define EFFECT_DESC_NAG_5D EFFECTDESC(true, LERP_NONE, 5.f, 4.f, 29, 4, 4, 1, false, false, _float4(1.5f, 1.5f, 1.5f, 1.f))
#define EFFECT_DESC_NAG_5D_BLOOD EFFECTDESC(true, LERP_NONE, 5.f, 4.f, 29, 4, 4, 1, false, false, _float4(1.5f, 0.1f, 0.1f, 1.f))

#define EFFECT_SUBDESC_NAG_2S SUB_EFFECT_DESC(L"Model_Effect_NAG_2S", nullptr)
#define EFFECT_DESC_NAG_2S EFFECTDESC(true, LERP_NONE, 5.f, 4.f, 29, 4, 4, 1, false, false, _float4(1.5f, 1.5f, 1.5f, 1.f))
#define EFFECT_DESC_NAG_2S_BLOOD EFFECTDESC(true, LERP_NONE, 5.f, 4.f, 29, 4, 4, 1, false, false, _float4(1.5f, 0.1f, 0.1f, 1.f))

#define EFFECT_SUBDESC_NAG_2H SUB_EFFECT_DESC(L"Model_Effect_NAG_2H", nullptr)
#define EFFECT_DESC_NAG_2H EFFECTDESC(true, LERP_NONE, 5.f, 2.f, 29, 4, 4, 1, false, false, _float4(1.5f, 1.5f, 1.5f, 1.f))
#define EFFECT_DESC_NAG_2H_BLOOD EFFECTDESC(true, LERP_NONE, 5.f, 2.f, 29, 4, 4, 1, false, false, _float4(1.5f, 0.1f, 0.1f, 1.f))

#define EFFECT_SUBDESC_NAG_6P SUB_EFFECT_DESC(L"Model_Effect_NAG_6P", nullptr)
#define EFFECT_DESC_NAG_6P EFFECTDESC(true, LERP_NONE, 2.f, 3.f, 4, 1, 1, 1, false, false, _float4(1.f, 1.f, 1.f, 1.f))

#define EFFECT_SUBDESC_NAG_6K SUB_EFFECT_DESC(L"Model_Effect_NAG_6K", nullptr)
#define EFFECT_DESC_NAG_6K EFFECTDESC(true, LERP_NONE, 2.f, 3.f, 4, 4, 4, 1, false, false, _float4(1.f, 1.f, 1.f, 1.f))

#define EFFECT_SUBDESC_NAG_6H SUB_EFFECT_DESC(L"Model_Effect_NAG_6H", nullptr)
#define EFFECT_DESC_NAG_6H EFFECTDESC(true, LERP_NONE, 5.f, 2.f, 29, 4, 4, 1, false, false, _float4(1.5f, 1.5f, 1.5f, 1.f))
#define EFFECT_DESC_NAG_6H_BLOOD EFFECTDESC(true, LERP_NONE, 5.f, 2.f, 29, 4, 4, 1, false, false, _float4(1.5f, 0.1f, 0.1f, 1.f))

#define EFFECT_SUBDESC_NAG_JS SUB_EFFECT_DESC(L"Model_Effect_NAG_jS", nullptr)
#define EFFECT_DESC_NAG_JS EFFECTDESC(true, LERP_NONE, 5.f, 3.f, 29, 4, 4, 1, false, false, _float4(1.5f, 1.5f, 1.5f, 1.f))
#define EFFECT_DESC_NAG_JS_BLOOD EFFECTDESC(true, LERP_NONE, 5.f, 3.f, 29, 4, 4, 1, false, false, _float4(1.5f, 0.1f, 0.1f, 1.f))

#define EFFECT_SUBDESC_NAG_JH SUB_EFFECT_DESC(L"Model_Effect_NAG_jH", nullptr)
#define EFFECT_DESC_NAG_JH EFFECTDESC(true, LERP_NONE, 5.f, 3.f, 29, 4, 4, 1, false, false, _float4(1.5f, 1.5f, 1.5f, 1.f))
#define EFFECT_DESC_NAG_JH_BLOOD EFFECTDESC(true, LERP_NONE, 5.f, 3.f, 29, 4, 4, 1, false, false, _float4(1.5f, 0.1f, 0.1f, 1.f))

// NAG_SPECIAL
#define EFFECT_SUBDESC_NAG_KAMURIYUKI SUB_EFFECT_DESC(L"Model_Effect_NAG_Kamuriyuki", nullptr)
#define EFFECT_DESC_NAG_KAMURIYUKI EFFECTDESC(true, LERP_NONE, 5.f, 2.f, 4, 10, 10, 1, false, false, _float4(1.f, 0.1f, 0.1f, 1.f))

#define EFFECT_SUBDESC_NAG_SHIZURIYUKI SUB_EFFECT_DESC(L"Model_Effect_NAG_Shizuriyuki", nullptr)
#define EFFECT_DESC_NAG_SHIZURIYUKI EFFECTDESC(true, LERP_NONE, 5.f, 2.f, 4, 10, 10, 1, false, false, _float4(1.f, 0.1f, 0.1f, 1.f))

#define EFFECT_SUBDESC_NAG_BLOODRAGEACTIVATION SUB_EFFECT_DESC(L"Model_Effect_NAG_BloodRageActivation", nullptr)
#define EFFECT_DESC_NAG_BLOODRAGEACTIVATION EFFECTDESC(true, LERP_NONE, 5.f, 2.f, 4, 10, 2, 5, false, false, _float4(0.75f, 0.1f, 0.1f, 1.f))

// NAG_OVERDRIVE
#define EFFECT_SUBDESC_NAG_WASUREYUKI_START SUB_EFFECT_DESC(L"Model_Effect_NAG_Wasureyuki_Start", nullptr)
#define EFFECT_DESC_NAG_WASUREYUKI_START EFFECTDESC(true, LERP_NONE, 2.f, 2.f, 4, 10, 10, 1, false, false, _float4(1.f, 0.1f, 0.1f, 1.f))

#define EFFECT_SUBDESC_NAG_WASUREYUKI_SLASH SUB_EFFECT_DESC(L"Model_Effect_NAG_Wasureyuki_Slash", nullptr)
#define EFFECT_DESC_NAG_WASUREYUKI_SLASH EFFECTDESC(true, LERP_NONE, 10.f, 5.5f, 4, 10, 10, 1, false, false, _float4(1.f, 0.1f, 0.1f, 1.f))

#define EFFECT_SUBDESC_NAG_WASUREYUKI_CIRCLE SUB_EFFECT_DESC(L"Model_Effect_NAG_Wasureyuki_Circle", nullptr)
#define EFFECT_DESC_NAG_WASUREYUKI_CIRCLE EFFECTDESC(true, LERP_NONE, 2.f, 2.f, 4, 4, 4, 1, false, false, _float4(0.2f, 0.2f, 0.2f, 1.f))

#define EFFECT_SUBDESC_NAG_WASUREYUKI SUB_EFFECT_DESC(L"Model_Effect_NAG_Wasureyuki", nullptr)
#define EFFECT_DESC_NAG_WASUREYUKI EFFECTDESC(true, LERP_NONE, 10.f, 2.f, 10, 1, 1, 1, false, false, _float4(1.f, 1.f, 1.f, 1.f))

#define EFFECT_SUBDESC_NAG_ZANSETSU SUB_EFFECT_DESC(L"Model_Effect_NAG_Zansetsu", nullptr)
#define EFFECT_DESC_NAG_ZANSETSU EFFECTDESC(true, LERP_NONE, 5.f, 3.f, 4, 5, 5, 1, false, false, _float4(1.f, 0.1f, 0.1f, 1.f))

// CHP_NORMAL
#define EFFECT_SUBDESC_CHP_5P SUB_EFFECT_DESC(L"Model_Effect_CHP_5P", nullptr)
#define EFFECT_DESC_CHP_5P EFFECTDESC(true, LERP_NONE, 2.f, 3.f, 4, 1, 1, 1, false, false, _float4(1.f, 1.f, 1.f, 1.f))

#define EFFECT_SUBDESC_CHP_FS SUB_EFFECT_DESC(L"Model_Effect_CHP_fS", nullptr)
#define EFFECT_DESC_CHP_FS EFFECTDESC(true, LERP_NONE, 2.f, 5.f, 4, 10, 5, 2, false, false, _float4(1.5f, 1.5f, 1.5f, 1.f))

#define EFFECT_SUBDESC_CHP_5D_TORNADO SUB_EFFECT_DESC(L"Model_Effect_CHP_5D_Tornado", nullptr)
#define EFFECT_DESC_CHP_5D_TORNADO EFFECTDESC(true, LERP_NONE, 2.f, 2.f, 1, 1, 1, 1, false, false, _float4(1.f, 1.f, 1.f, 1.f))

#define EFFECT_SUBDESC_CHP_5D_FINAL SUB_EFFECT_DESC(L"Model_Effect_CHP_5D_Final", nullptr)
#define EFFECT_DESC_CHP_5D_FINAL EFFECTDESC(true, LERP_NONE, 2.f, 2.f, 4, 12, 4, 3, false, false, _float4(1.f, 1.f, 1.f, 1.f))

#define EFFECT_SUBDESC_CHP_2S SUB_EFFECT_DESC(L"Model_Effect_CHP_2S", nullptr)
#define EFFECT_DESC_CHP_2S EFFECTDESC(true, LERP_NONE, 2.f, 5.f, 4, 10, 5, 2, false, false, _float4(1.5f, 1.5f, 1.5f, 1.f))

#define EFFECT_SUBDESC_CHP_2H SUB_EFFECT_DESC(L"Model_Effect_CHP_2H", nullptr)
#define EFFECT_DESC_CHP_2H EFFECTDESC(true, LERP_NONE, 2.f, 5.f, 4, 10, 5, 2, false, false, _float4(1.5f, 1.5f, 1.5f, 1.f))

#define EFFECT_SUBDESC_CHP_2D SUB_EFFECT_DESC(L"Model_Effect_CHP_2D", nullptr)
#define EFFECT_DESC_CHP_2D EFFECTDESC(true, LERP_NONE, 2.f, 5.f, 4, 10, 5, 2, false, false, _float4(1.5f, 1.5f, 1.5f, 1.f))

#define EFFECT_SUBDESC_CHP_6H SUB_EFFECT_DESC(L"Model_Effect_CHP_6H", nullptr)
#define EFFECT_DESC_CHP_6H EFFECTDESC(true, LERP_NONE, 2.f, 5.f, 4, 10, 5, 2, false, false, _float4(1.5f, 1.5f, 1.5f, 1.f))

// CHP_SPECIAL
#define EFFECT_SUBDESC_CHP_ALPHABLADE_DASH SUB_EFFECT_DESC(L"Model_Effect_CHP_AlphaBlade_Dash", nullptr)
#define EFFECT_DESC_CHP_ALPHABLADE_DASH EFFECTDESC(true, LERP_NONE, 2.f, 4.f, 10, 1, 1, 1, false, false, _float4(1.f, 1.f, 1.f, 1.f))

#define EFFECT_SUBDESC_CHP_ALPHABLADE_LINE SUB_EFFECT_DESC(L"Model_Effect_CHP_AlphaBlade_Line", nullptr)
#define EFFECT_DESC_CHP_ALPHABLADE_LINE EFFECTDESC(true, LERP_NONE, 2.f, 2.f, 17, 4, 2, 2, false, true, _float4(1.f, 1.f, 1.f, 1.f))

#define EFFECT_SUBDESC_CHP_ALPHABLADE_PARTICLE SUB_EFFECT_DESC(nullptr, L"Prototype_Component_Texture_Effect_CMN_at_line03")
#define EFFECT_DESC_CHP_ALPHABLADE_PARTICLE EFFECTDESC(true, LERP_NONE, 2.f, 1.f, 1, 1, 1, 1, false, true, _float4(1.f, 2.f, 3.f, 1.f))

#define EFFECT_SUBDESC_CHP_BETABLADE SUB_EFFECT_DESC(L"Model_Effect_CHP_BetaBlade", nullptr)
#define EFFECT_DESC_CHP_BETABLADE EFFECTDESC(true, LERP_NONE, 3.f, 3.f, 21, 10, 5, 2, false, true, _float4(1.f, 2.f, 3.f, 1.f))

#define EFFECT_SUBDESC_CHP_RESSHOU SUB_EFFECT_DESC(L"Model_Effect_CHP_Resshou", nullptr)
#define EFFECT_DESC_CHP_RESSHOU EFFECTDESC(true, LERP_NONE, 3.f, 3.f, 21, 6, 3, 2, false, true, _float4(1.f, 2.f, 3.f, 1.f))

#define EFFECT_SUBDESC_CHP_ROKUSAI SUB_EFFECT_DESC(L"Model_Effect_CHP_Rokusai", nullptr)
#define EFFECT_DESC_CHP_ROKUSAI EFFECTDESC(true, LERP_NONE, 3.f, 3.f, 21, 10, 5, 2, false, true, _float4(1.f, 2.f, 3.f, 1.f))

#define EFFECT_SUBDESC_CHP_SENSHUU SUB_EFFECT_DESC(L"Model_Effect_CHP_Senshuu", nullptr)
#define EFFECT_DESC_CHP_SENSHUU EFFECTDESC(true, LERP_NONE, 3.f, 4.f, 21, 10, 5, 2, false, true, _float4(1.f, 2.f, 3.f, 1.f))

// CHP_OVERDRIVE
#define EFFECT_SUBDESC_CHP_ZANSEIROUGA_FLASH SUB_EFFECT_DESC(L"Model_Effect_CHP_ZanseiRouga_Flash", nullptr)
#define EFFECT_DESC_CHP_ZANSEIROUGA_FLASH EFFECTDESC(true, LERP_NONE, 2.f, 4.f, 10, 1, 1, 1, false, false, _float4(5.f, 1.7f, 1.5f, 1.f))

#define EFFECT_SUBDESC_CHP_ZANSEIROUGA_XFLASH SUB_EFFECT_DESC(L"Model_Effect_CHP_ZanseiRouga_XFlash", nullptr)
#define EFFECT_DESC_CHP_ZANSEIROUGA_XFLASH EFFECTDESC(true, LERP_NONE, 2.f, 4.f, 10, 1, 1, 1, false, false, _float4(5.f, 1.7f, 1.5f, 1.f))

#define EFFECT_SUBDESC_CHP_ZANSEIROUGA_DASH SUB_EFFECT_DESC(L"Model_Effect_CHP_ZanseiRouga_Dash", nullptr)
#define EFFECT_DESC_CHP_ZANSEIROUGA_DASH EFFECTDESC(true, LERP_NONE, 2.f, 4.f, 4, 8, 4, 2, false, false, _float4(1.f, 1.f, 1.f, 1.f))

#define EFFECT_SUBDESC_CHP_ZANSEIROUGA_LINE SUB_EFFECT_DESC(L"Model_Effect_CHP_ZanseiRouga_Line", nullptr)
#define EFFECT_DESC_CHP_ZANSEIROUGA_LINE EFFECTDESC(true, LERP_NONE, 2.f, 4.f, 21, 8, 8, 1, false, true, _float4(2.f, 2.f, 2.f, 1.f))

#define EFFECT_SUBDESC_CHP_ZANSEIROUGA_OUTLINE SUB_EFFECT_DESC(L"Model_Effect_CHP_ZanseiRouga_OutLine", nullptr)
#define EFFECT_DESC_CHP_ZANSEIROUGA_OUTLINE EFFECTDESC(true, LERP_NONE, 2.f, 3.5f, 21, 5, 5, 1, false, true, _float4(0.f, 0.f, 0.05f, 1.f))

#define EFFECT_SUBDESC_CHP_ZANSEIROUGA_SPARK SUB_EFFECT_DESC(L"Model_Effect_CHP_ZanseiRouga_Spark", nullptr)
#define EFFECT_DESC_CHP_ZANSEIROUGA_SPARK EFFECTDESC(true, LERP_NONE, 2.f, 6.f, 19, 6, 3, 2, false, true, _float4(2.f, 2.f, 2.f, 1.f))

#define EFFECT_SUBDESC_CHP_ZANSEIROUGA_CIRCLE SUB_EFFECT_DESC(L"Model_Effect_CHP_ZanseiRouga_Circle", nullptr)
#define EFFECT_DESC_CHP_ZANSEIROUGA_CIRCLE EFFECTDESC(true, LERP_NONE, 2.f, 1.f, 21, 1, 1, 1, false, false, _float4(1.f, 1.f, 3.f, 1.f))

#define EFFECT_SUBDESC_CHP_ZANSEIROUGA_FLOOR SUB_EFFECT_DESC(L"Model_Effect_CHP_ZanseiRouga_Floor", nullptr)
#define EFFECT_DESC_CHP_ZANSEIROUGA_FLOOR EFFECTDESC(true, LERP_NONE, 2.f, 1.f, 38, 1, 1, 1, false, true, _float4(1.f, 2.f, 3.f, 1.f))

#define EFFECT_SUBDESC_CHP_ZANSEIROUGA_FIRE SUB_EFFECT_DESC(L"Model_Effect_CHP_ZanseiRouga_Fire", nullptr)
#define EFFECT_DESC_CHP_ZANSEIROUGA_FIRE EFFECTDESC(true, LERP_NONE, 2.f, 3.f, 21, 8, 8, 1, false, true, _float4(1.f, 2.f, 3.f, 1.f))

#define EFFECT_SUBDESC_CHP_ZANSEIROUGA_BEAM SUB_EFFECT_DESC(L"Model_Effect_CHP_ZanseiRouga_Beam", nullptr)
#define EFFECT_DESC_CHP_ZANSEIROUGA_BEAM EFFECTDESC(true, LERP_NONE, 2.f, 2.f, 21, 1, 1, 1, false, true, _float4(1.f, 2.f, 3.f, 1.f))

#define EFFECT_SUBDESC_CHP_ZANSEIROUGA_CORE SUB_EFFECT_DESC(L"Model_Effect_CHP_ZanseiRouga_Core", nullptr)
#define EFFECT_DESC_CHP_ZANSEIROUGA_CORE EFFECTDESC(true, LERP_NONE, 2.f, 2.f, 21, 9, 3, 3, false, true, _float4(1.f, 2.f, 3.f, 1.f))

#define EFFECT_SUBDESC_CHP_ZANSEIROUGA_SCREW SUB_EFFECT_DESC(L"Model_Effect_CHP_ZanseiRouga_Screw", nullptr)
#define EFFECT_DESC_CHP_ZANSEIROUGA_SCREW EFFECTDESC(true, LERP_NONE, 2.f, 3.f, 21, 4, 2, 2, false, true, _float4(1.f, 2.f, 3.f, 1.f))

#pragma endregion

#pragma region PJE

//	CHP_RAKKO
#define EFFECT_SUBDESC_CHP_RAKKO SUB_EFFECT_DESC(L"Model_CHP_Rakko", nullptr)
#define EFFECT_DESC_CHP_RAKKO EFFECTDESC(true, LERP_NONE, 5.f, 30.f, 5, 0, 0, 0, false, false)

//	MAY_EFFECT
#define EFFECT_SUBDESC_MAY_ANCHOR_UPPER SUB_EFFECT_DESC(L"Model_MAY_Anchor_Upper", nullptr)		//	S + K
#define EFFECT_DESC_MAY_ANCHOR_UPPER EFFECTDESC(true, LERP_NONE, 0.25f, 3.f, 4, 6, 3, 2, false, false/*, _float4(0.8f, 0.8f, 0.8f, 1.f)*/)

#define EFFECT_SUBDESC_MAY_ANCHOR_SMASH_HORIZONTAL1 SUB_EFFECT_DESC(L"Model_MAY_Anchor_Smash_Horizontal1", nullptr)		//	I
#define EFFECT_DESC_MAY_ANCHOR_SMASH_HORIZONTAL1 EFFECTDESC(true, LERP_NONE, 0.15f, 3.f, 4, 6, 3, 2, false, false/*, _float4(0.8f, 0.8f, 0.8f, 1.f)*/)
#define EFFECT_SUBDESC_MAY_ANCHOR_SMASH_HORIZONTAL2 SUB_EFFECT_DESC(L"Model_MAY_Anchor_Smash_Horizontal2", nullptr)
#define EFFECT_DESC_MAY_ANCHOR_SMASH_HORIZONTAL2 EFFECTDESC(true, LERP_NONE, 0.25f, 3.f, 4, 6, 3, 2, false, false/*, _float4(0.8f, 0.8f, 0.8f, 1.f)*/)

#define EFFECT_SUBDESC_MAY_ANCHOR_SIT_ATTACK_LINE1 SUB_EFFECT_DESC(L"Model_MAY_Anchor_Sit_Attack_Line1", nullptr)		//	S + I
#define EFFECT_DESC_MAY_ANCHOR_SIT_ATTACK_LINE1 EFFECTDESC(true, LERP_NONE, 0.2f, 3.f, 4, 6, 6, 1, false, false, _float4(0.8f, 0.8f, 0.8f, 1.f))
#define EFFECT_SUBDESC_MAY_ANCHOR_SIT_ATTACK_LINE2 SUB_EFFECT_DESC(L"Model_MAY_Anchor_Sit_Attack_Line2", nullptr)
#define EFFECT_DESC_MAY_ANCHOR_SIT_ATTACK_LINE2 EFFECTDESC(true, LERP_NONE, 0.2f, 3.f, 4, 6, 6, 1, false, false, _float4(0.8f, 0.8f, 0.8f, 1.f))
#define EFFECT_SUBDESC_MAY_ANCHOR_SIT_ATTACK1 SUB_EFFECT_DESC(L"Model_MAY_Anchor_Sit_Attack1", nullptr)
#define EFFECT_DESC_MAY_ANCHOR_SIT_ATTACK1 EFFECTDESC(true, LERP_NONE, 0.1f, 0.f, 9, 0, 0, 0, false, false)
#define EFFECT_SUBDESC_MAY_ANCHOR_SIT_ATTACK2 SUB_EFFECT_DESC(L"Model_MAY_Anchor_Sit_Attack2", nullptr)
#define EFFECT_DESC_MAY_ANCHOR_SIT_ATTACK2 EFFECTDESC(true, LERP_NONE, 0.02f, 0.f, 9, 0, 0, 0, false, false)

#define EFFECT_SUBDESC_MAY_ANCHOR_SMASH_NEAR1 SUB_EFFECT_DESC(L"Model_MAY_Anchor_Smash_Near1", nullptr)		//	가까이에서 I
#define EFFECT_SUBDESC_MAY_ANCHOR_SMASH_NEAR2 SUB_EFFECT_DESC(L"Model_MAY_Anchor_Smash_Near2", nullptr)
#define EFFECT_DESC_MAY_ANCHOR_SMASH_NEAR EFFECTDESC(true, LERP_NONE, 0.3f, 2.f, 4, 10, 5, 2, false, false, _float4(0.8f, 0.8f, 0.8f, 1.f))

#define EFFECT_SUBDESC_MAY_JUMP_ANCHOR_UPWARD1 SUB_EFFECT_DESC(L"Model_MAY_Jump_Anchor_Upward1", nullptr)	//	점프 O
#define EFFECT_DESC_MAY_JUMP_ANCHOR_UPWARD1 EFFECTDESC(true, LERP_NONE, 0.15f, 4.5f, 4, 6, 3, 2, false, false, _float4(0.8f, 0.8f, 0.8f, 1.f))
#define EFFECT_SUBDESC_MAY_JUMP_ANCHOR_UPWARD2 SUB_EFFECT_DESC(L"Model_MAY_Jump_Anchor_Upward2", nullptr)
#define EFFECT_DESC_MAY_JUMP_ANCHOR_UPWARD2 EFFECTDESC(true, LERP_NONE, 0.15f, 4.5f, 4, 6, 6, 1, false, false, _float4(0.6f, 0.6f, 0.6f, 1.f))

#define EFFECT_SUBDESC_MAY_JUMP_DOWN_ANCHOR1 SUB_EFFECT_DESC(L"Model_MAY_Jump_Down_Anchor1", nullptr)		//	점프 S + K
#define EFFECT_DESC_MAY_JUMP_DOWN_ANCHOR1 EFFECTDESC(true, LERP_NONE, 0.3f, 4.f, 9, 6, 3, 2, false, false)
#define EFFECT_SUBDESC_MAY_JUMP_DOWN_ANCHOR2 SUB_EFFECT_DESC(L"Model_MAY_Jump_Down_Anchor2", nullptr)
#define EFFECT_DESC_MAY_JUMP_DOWN_ANCHOR2 EFFECTDESC(true, LERP_NONE, 0.3f, 4.5f, 9, 6, 3, 2, false, false)

#define EFFECT_SUBDESC_MAY_ANCHOR_SMASH_CHARGING SUB_EFFECT_DESC(L"Model_MAY_Anchor_Smash_Charging", nullptr)	//	D + K
#define EFFECT_DESC_MAY_ANCHOR_SMASH_CHARGING EFFECTDESC(true, LERP_NONE, 0.2f, 3.5f, 4, 6, 3, 2, false, false/*, _float4(0.8f, 0.8f, 0.8f, 1.f)*/)
 
#define EFFECT_SUBDESC_MAY_2D SUB_EFFECT_DESC(L"Model_MAY_2D", nullptr)		//	S + O
#define EFFECT_DESC_MAY_2D EFFECTDESC(true, LERP_NONE, 0.1f, 4.f, 22, 6, 3, 2, false, false/*, _float4(0.8f, 0.8f, 0.8f, 1.f)*/)

#define EFFECT_SUBDESC_MAY_6K SUB_EFFECT_DESC(L"Model_MAY_6K", nullptr)		//	D + J
#define EFFECT_DESC_MAY_6K EFFECTDESC(true, LERP_NONE, 0.15f, 4.f, 4, 6, 3, 2, false, false/*, _float4(0.8f, 0.8f, 0.8f, 1.f)*/)

#define EFFECT_SUBDESC_SPLASHCYLINDER1 SUB_EFFECT_DESC(L"Model_Splashcylinder1", nullptr)		//	돌고래 같은거 소환하면 나오는 물
#define EFFECT_SUBDESC_SPLASHCYLINDER2 SUB_EFFECT_DESC(L"Model_Splashcylinder2", nullptr)
#define EFFECT_SUBDESC_SPLASHCYLINDER3 SUB_EFFECT_DESC(L"Model_Splashcylinder3", nullptr)
#define EFFECT_DESC_SPLASHCYLINDER EFFECTDESC(true, LERP_NONE, 0.7f, 1.3f, 22, 12, 4, 3, false, false)
#define EFFECT_DESC_SPLASHCYLINDER3 EFFECTDESC(true, LERP_NONE, 0.5f, 1.5f, 22, 10, 5, 2, false, false)
#define EFFECT_SUBDESC_WATERRING SUB_EFFECT_DESC(L"Model_WaterRing", nullptr)
#define EFFECT_DESC_WATERRING EFFECTDESC(true, LERP_NONE, 0.7f, 1.3f, 4, 12, 4, 3, false, false)

#define EFFECT_SUBDESC_IRUKASAN_ANIM SUB_EFFECT_DESC(L"Model_IRukaSan_Anim", nullptr)			//	돌고래_애니메이션
#define EFFECT_DESC_IRUKASAN_ANIM EFFECTDESC(true, LERP_NONE, 0.7f, 30.f, 5, 0, 0, 0, false, false)
#define EFFECT_SUBDESC_IRUKASAN_RIDE SUB_EFFECT_DESC(L"Model_IRukaSan_Ride", nullptr)			//	돌고래_타기
#define EFFECT_DESC_IRUKASAN_RIDE EFFECTDESC(true, LERP_NONE, 0.15f, 30.f, 9, 0, 0, 0, false, false)
#define EFFECT_SUBDESC_IRUKASAN_RETURN SUB_EFFECT_DESC(L"Model_IRukaSan_Return", nullptr)			//	돌고래_안맞아서 돌아가기
#define EFFECT_DESC_IRUKASAN_RETURN EFFECTDESC(true, LERP_NONE, 1.f, 30.f, 9, 0, 0, 0, false, false)
#define EFFECT_SUBDESC_IRUKASAN_HIT SUB_EFFECT_DESC(L"Model_IRukaSan_Hit", nullptr)			//	돌고래_맞춰서 짜부
#define EFFECT_DESC_IRUKASAN_HIT EFFECTDESC(true, LERP_NONE, 0.13f, 30.f, 9, 0, 0, 0, false, false)
#define EFFECT_SUBDESC_IRUKASAN_ROLL SUB_EFFECT_DESC(L"Model_IRukaSan_Roll", nullptr)			//	돌고래_맞춰서 돌아가기
#define EFFECT_DESC_IRUKASAN_ROLL EFFECTDESC(true, LERP_NONE, 0.75f, 30.f, 9, 0, 0, 0, false, false)

#define EFFECT_SUBDESC_DROP01_05 SUB_EFFECT_DESC(L"Model_Drop01_05", nullptr)		//	물방울
#define EFFECT_DESC_DROP01_05 EFFECTDESC(true, LERP_NONE, 0.15f, 0.f, 4, 1, 1, 1, false, false)
#define EFFECT_SUBDESC_TWINKLE01_03 SUB_EFFECT_DESC(L"Model_Twinkle01_03", nullptr)		//	물방울 반짝이
#define EFFECT_DESC_TWINKLE01_03 EFFECTDESC(true, LERP_NONE, 0.3f, 3.f, 4, 4, 2, 2, false, false)
#define EFFECT_SUBDESC_TWINKLE02_06 SUB_EFFECT_DESC(L"Model_Twinkle02_06", nullptr)		//	물방울 반짝이2
#define EFFECT_DESC_TWINKLE02_06 EFFECTDESC(true, LERP_NONE, 0.4f, 2.f, 4, 8, 4, 2, false, false)
#define EFFECT_SUBDESC_TWINKLE03_04 SUB_EFFECT_DESC(L"Model_Twinkle03_04", nullptr)		//	물방울 반짝이3
#define EFFECT_DESC_TWINKLE03_04 EFFECTDESC(true, LERP_NONE, 0.4f, 2.5f, 4, 4, 2, 2, false, false)
#define EFFECT_SUBDESC_GLOW_09 SUB_EFFECT_DESC(L"Model_Glow_09", nullptr)		//	물방울 반짝이4
#define EFFECT_DESC_GLOW_09 EFFECTDESC(true, LERP_NONE, 0.3f, 0.f, 4, 1, 1, 1, false, false)

#define EFFECT_SUBDESC_MAY_MAYDYNAMIC SUB_EFFECT_DESC(L"Model_MAY_MayDynamic", nullptr)			//	고쇼가와라
#define EFFECT_DESC_MAY_MAYDYNAMIC EFFECTDESC(true, LERP_NONE, 10.f, 30.f, 5, 0, 0, 0, false, false)
#define EFFECT_SUBDESC_ARISUGAWA SUB_EFFECT_DESC(L"Model_Arisugawa", nullptr)			//	매너티
#define EFFECT_DESC_ARISUGAWA EFFECTDESC(true, LERP_NONE, 5.f, 30.f, 5, 0, 0, 0, false, false)

#define EFFECT_SUBDESC_MAY_BURSTHIT0 SUB_EFFECT_DESC(L"Model_MAY_BurstHit0", nullptr)		//	공격 시 나오는 그 전기 같은 거 0
#define EFFECT_SUBDESC_MAY_BURSTHIT1 SUB_EFFECT_DESC(L"Model_MAY_BurstHit1", nullptr)		//	공격 시 나오는 그 전기 같은 거 1
#define EFFECT_SUBDESC_MAY_BURSTHIT2 SUB_EFFECT_DESC(L"Model_MAY_BurstHit2", nullptr)		//	공격 시 나오는 그 전기 같은 거 2
#define EFFECT_DESC_MAY_BURSTHIT EFFECTDESC(true, LERP_NONE, 0.2f, 3.8f, 19, 4, 2, 2, false, true, _float4(3.f, 2.f, 1.f, 1.f))
#define EFFECT_SUBDESC_MAY_SLASHSPARK01 SUB_EFFECT_DESC(L"Model_MAY_SlashSpark01", nullptr)		//	공격 시 나오는 파티클 같은 거 1
#define EFFECT_SUBDESC_MAY_SLASHSPARK02 SUB_EFFECT_DESC(L"Model_MAY_SlashSpark02", nullptr)		//	공격 시 나오는 파티클 같은 거 2
#define EFFECT_SUBDESC_MAY_SLASHSPARK03 SUB_EFFECT_DESC(L"Model_MAY_SlashSpark03", nullptr)		//	공격 시 나오는 파티클 같은 거 2
#define EFFECT_SUBDESC_MAY_SLASHSPARK05 SUB_EFFECT_DESC(L"Model_MAY_SlashSpark05", nullptr)		//	공격 시 나오는 파티클 같은 거 5
#define EFFECT_DESC_MAY_SLASHSPARK EFFECTDESC(true, LERP_NONE, 0.3f, 3.f, 19, 6, 3, 2, false, true, _float4(3.f, 2.f, 1.f, 1.f))

#define EFFECT_SUBDESC_GOS_SPLASH01 SUB_EFFECT_DESC(L"Model_GOS_Splash01", nullptr)		//	고쇼가와라 물1
#define EFFECT_DESC_GOS_SPLASH01 EFFECTDESC(true, LERP_NONE, 0.2f, 3.f, 22, 12, 4, 3, false, false)
#define EFFECT_SUBDESC_GOS_SPLASH02 SUB_EFFECT_DESC(L"Model_GOS_Splash02", nullptr)		//	고쇼가와라 물2
#define EFFECT_DESC_GOS_SPLASH02 EFFECTDESC(true, LERP_NONE, 0.7f, 0.8f, 22, 12, 4, 3, false, false)
#define EFFECT_SUBDESC_GOS_WATERRING SUB_EFFECT_DESC(L"Model_GOS_WaterRing", nullptr)
#define EFFECT_DESC_GOS_WATERRING EFFECTDESC(true, LERP_NONE, 0.8f, 0.f, 22, 0, 0, 0, false, false)

//	BKN_EFFECT
#define EFFECT_SUBDESC_BKN_FS SUB_EFFECT_DESC(L"Model_BKN_FS", nullptr)		//	I
#define EFFECT_DESC_BKN_FS EFFECTDESC(true, LERP_NONE, 0.15f, 5.f, 4, 6, 2, 3, false, false)

#define EFFECT_SUBDESC_BKN_5H01 SUB_EFFECT_DESC(L"Model_BKN_5H01", nullptr)		//	K
#define EFFECT_DESC_BKN_5H01 EFFECTDESC(true, LERP_NONE, 0.08f, 5.f, 4, 6, 2, 3, false, false)
#define EFFECT_SUBDESC_BKN_5H02 SUB_EFFECT_DESC(L"Model_BKN_5H02", nullptr)
#define EFFECT_DESC_BKN_5H02 EFFECTDESC(true, LERP_NONE, 0.16f, 5.5f, 22, 6, 2, 3, false, false)

#define EFFECT_SUBDESC_BKN_6H SUB_EFFECT_DESC(L"Model_BKN_6H", nullptr)		//	D + K
#define EFFECT_DESC_BKN_6H EFFECTDESC(true, LERP_NONE, 0.15f, 6.f, 22, 6, 2, 3, false, false)

#define EFFECT_SUBDESC_BKN_2H01 SUB_EFFECT_DESC(L"Model_BKN_2H01", nullptr)		//	S + K
#define EFFECT_DESC_BKN_2H01 EFFECTDESC(true, LERP_NONE, 0.08f, 5.f, 4, 6, 2, 3, false, false)
#define EFFECT_SUBDESC_BKN_2H02 SUB_EFFECT_DESC(L"Model_BKN_2H02", nullptr)
#define EFFECT_DESC_BKN_2H02 EFFECTDESC(true, LERP_NONE, 0.05f, 10.f, 4, 2, 2, 1, false, false)

#define EFFECT_SUBDESC_BKN_26H_H SUB_EFFECT_DESC(L"Model_BKN_26H_H", nullptr)		//	S + D + K, K
#define EFFECT_DESC_BKN_26H_H EFFECTDESC(true, LERP_NONE, 0.15f, 5.f, 30, 6, 2, 3, false, true, _float4(2.f, 1.f, 1.5f, 1.f))

#define EFFECT_SUBDESC_BKN_SANJU_SLASH SUB_EFFECT_DESC(L"Model_BKN_Sanju_Slash", nullptr)					//	천지인 Slash
#define EFFECT_DESC_BKN_SANJU_SLASH EFFECTDESC(true, LERP_NONE, 0.05f, 5.f, 30, 6, 2, 3, false, true, _float4(1.f, 1.f, 1.f, 1.f))
#define EFFECT_SUBDESC_BKN_SANJU_KIDOU SUB_EFFECT_DESC(L"Model_BKN_Sanju_Kidou", nullptr)					//	천지인 Kidou
#define EFFECT_DESC_BKN_SANJU_KIDOU EFFECTDESC(true, LERP_NONE, 0.24f, 3.4f, 30, 4, 4, 1, false, true, _float4(1.f, 1.f, 1.f, 1.f))
#define EFFECT_SUBDESC_BKN_SANJU_MOON SUB_EFFECT_DESC(L"Model_BKN_Sanju_Moon", nullptr)						//	천지인 Moon
#define EFFECT_DESC_BKN_SANJU_MOON EFFECTDESC(true, LERP_NONE, 1.5f, 3.f, 22, 1, 1, 1, false, false)
#define EFFECT_SUBDESC_BKN_SANJU_CLOUD SUB_EFFECT_DESC(L"Model_BKN_Sanju_Cloud", nullptr)					//	천지인 Cloud
#define EFFECT_DESC_BKN_SANJU_CLOUD EFFECTDESC(true, LERP_NONE, 1.5f, 1.5f, 8, 1, 1, 1, false, false)
#define EFFECT_SUBDESC_BKN_SANJU_SMOKE SUB_EFFECT_DESC(L"Model_BKN_Sanju_Smoke", nullptr)					//	천지인 Smoke
#define EFFECT_DESC_BKN_SANJU_SMOKE EFFECTDESC(true, LERP_NONE, 2.f, 0.43f, 8, 40, 4, 10, false, false)
#define EFFECT_SUBDESC_BKN_SANJU_SMOKECLOUD SUB_EFFECT_DESC(L"Model_BKN_Sanju_SmokeCloud", nullptr)			//	천지인 Smoke Cloud
#define EFFECT_DESC_BKN_SANJU_SMOKECLOUD EFFECTDESC(true, LERP_NONE, 1.2f, 0.8f, 8, 40, 4, 10, false, false)
#define EFFECT_SUBDESC_BKN_SANJU_TENCHIJIN SUB_EFFECT_DESC(L"Model_BKN_Sanju_Tenchijin", nullptr)			//	천지인
#define EFFECT_SUBDESC_BKN_SANJU_TENCHIJINREVERSE SUB_EFFECT_DESC(L"Model_BKN_Sanju_TenchijinReverse", nullptr)
#define EFFECT_DESC_BKN_SANJU_TENCHIJIN EFFECTDESC(true, LERP_NONE, 0.5f, 1.f, 10, 1, 1, 1, false, false)
#define EFFECT_SUBDESC_BKN_SANJU_TENCHIJINANIM SUB_EFFECT_DESC(L"Model_BKN_Sanju_TenchijinAnim", nullptr)	//	천지인 Anim
#define EFFECT_SUBDESC_BKN_SANJU_TENCHIJINANIMREVERSE SUB_EFFECT_DESC(L"Model_BKN_Sanju_TenchijinAnimReverse", nullptr)
#define EFFECT_DESC_BKN_SANJU_TENCHIJINANIM EFFECTDESC(true, LERP_NONE, 0.35f, 2.5f, 4, 4, 2, 2, false, false)

#define EFFECT_SUBDESC_BKN_TATAMI_SMOKE SUB_EFFECT_DESC(L"Model_BKN_Tatami_Smoke", nullptr)			//	다다미 연기
#define EFFECT_DESC_BKN_TATAMI_SMOKE EFFECTDESC(true, LERP_NONE, 0.35f, 2.3f, 4, 16, 16, 1, false, false)
#define EFFECT_SUBDESC_BKN_TATAMI_BLAST01 SUB_EFFECT_DESC(L"Model_BKN_Tatami_Blast01", nullptr)		//	다다미 Blast01
#define EFFECT_DESC_BKN_TATAMI_BLAST01 EFFECTDESC(true, LERP_NONE, 0.3f, 2.5f, 30, 5, 1, 5, false, true, _float4(2.f, 1.f, 1.5f, 1.f))
#define EFFECT_SUBDESC_BKN_TATAMI_BLAST02 SUB_EFFECT_DESC(L"Model_BKN_Tatami_Blast02", nullptr)		//	다다미 Blast02
#define EFFECT_DESC_BKN_TATAMI_BLAST02 EFFECTDESC(true, LERP_NONE, 0.3f, 2.5f, 30, 6, 3, 2, false, true, _float4(2.f, 1.f, 1.5f, 1.f))
#define EFFECT_SUBDESC_BKN_TATAMI_WINDRING SUB_EFFECT_DESC(L"Model_BKN_Tatami_Windring", nullptr)		//	다다미 Windring
#define EFFECT_DESC_BKN_TATAMI_WINDRING EFFECTDESC(true, LERP_NONE, 0.3f, 2.5f, 30, 6, 6, 1, false, true, _float4(2.f, 1.f, 1.5f, 1.f))
#define EFFECT_SUBDESC_BKN_TATAMI_LINE SUB_EFFECT_DESC(L"Model_BKN_Tatami_Line", nullptr)			//	다다미 Line
#define EFFECT_DESC_BKN_TATAMI_LINE EFFECTDESC(true, LERP_NONE, 0.2f, 4.f, 4, 3, 1, 3, false, false, _float4(0.f, 0.f, 0.f, 1.f))
#define EFFECT_SUBDESC_BKN_TATAMI_BAKE SUB_EFFECT_DESC(L"Model_BKN_Tatami_Bake", nullptr)			//	다다미 Bake
#define EFFECT_DESC_BKN_TATAMI_BAKE EFFECTDESC(true, LERP_NONE, 0.2f, 4.f, 4, 0, 0, 0, false, false)

#define EFFECT_SUBDESC_BKN_DUSTSMOKE SUB_EFFECT_DESC(L"Model_BKN_Dustsmoke", nullptr)		//	Dustsmoke
#define EFFECT_DESC_BKN_DUSTSMOKE EFFECTDESC(true, LERP_NONE, 0.3f, 3.f, 4, 10, 2, 5, false, false)

#define EFFECT_SUBDESC_BKN_YOUZANSEN_SLASH SUB_EFFECT_DESC(L"Model_BKN_Youzansen_Slash", nullptr)	//	W, S + D + I
#define EFFECT_DESC_BKN_YOUZANSEN_SLASH EFFECTDESC(true, LERP_NONE, 0.5f, 2.f, 30, 16, 2, 8, false, true, _float4(1.f, 1.f, 1.f, 1.f))

//	SHADER PASS DEFINE
#define SHADER_TURQUOISE_EFFECT_SPRITE_ZTEST	34
#define	SHADER_YELLOW_EFFECT_SPRITE	35
#define	SHADER_ORANGE_EFFECT_SPRITE	37
#define SHADER_RED_EFFECT_SRITE_ZTEST	39

//	JKO_EFFECT
#define EFFECT_SUBDESC_JKO_FS SUB_EFFECT_DESC(L"Model_JKO_FS", nullptr)		//	I
#define EFFECT_DESC_JKO_FS EFFECTDESC(true, LERP_NONE, 0.15f, 4.f, 25, 1, 1, 1, false, true)

#define EFFECT_SUBDESC_JKO_5D_SOUL SUB_EFFECT_DESC(L"Model_JKO_5D_Soul", nullptr)		//	O
#define EFFECT_DESC_JKO_5D_SOUL EFFECTDESC(true, LERP_NONE, 0.3f, 3.f, 32, 10, 5, 2, false, true)
#define EFFECT_SUBDESC_JKO_5D_WIND SUB_EFFECT_DESC(L"Model_JKO_5D_Wind", nullptr)
#define EFFECT_DESC_JKO_5D_WIND EFFECTDESC(true, LERP_NONE, 0.3f, 3.f, 33, 4, 1, 4, false, true)
#define EFFECT_SUBDESC_JKO_5D_WINDRING SUB_EFFECT_DESC(L"Model_JKO_5D_Windring", nullptr)
#define EFFECT_DESC_JKO_5D_WINDRING EFFECTDESC(true, LERP_NONE, 0.3f, 3.f, 32, 6, 6, 1, false, true)

#define EFFECT_SUBDESC_JKO_5K SUB_EFFECT_DESC(L"Model_JKO_5K", nullptr)		//	J
#define EFFECT_DESC_JKO_5K EFFECTDESC(true, LERP_NONE, 0.15f, 4.f, 25, 1, 1, 1, false, true)

#define EFFECT_SUBDESC_JKO_5H SUB_EFFECT_DESC(L"Model_JKO_5H", nullptr)		//	K
#define EFFECT_DESC_JKO_5H EFFECTDESC(true, LERP_NONE, 0.18f, 4.f, 22, 6, 6, 1, false, false)

#define EFFECT_SUBDESC_JKO_2S SUB_EFFECT_DESC(L"Model_JKO_2S", nullptr)		//	S + I
#define EFFECT_DESC_JKO_2S EFFECTDESC(true, LERP_NONE, 0.3f, 3.f, 32, 6, 6, 1, false, true)

#define EFFECT_SUBDESC_JKO_2H SUB_EFFECT_DESC(L"Model_JKO_2H", nullptr)		//	S + K
#define EFFECT_DESC_JKO_2H EFFECTDESC(true, LERP_NONE, 0.25f, 3.f, 32, 0, 0, 0, false, true)

#define EFFECT_SUBDESC_JKO_2D SUB_EFFECT_DESC(L"Model_JKO_2D", nullptr)		//	S + O
#define EFFECT_DESC_JKO_2D EFFECTDESC(true, LERP_NONE, 0.5f, 1.5f, SHADER_YELLOW_EFFECT_SPRITE, 6, 6, 1, false, true)

#define EFFECT_SUBDESC_JKO_JS_STRIKE SUB_EFFECT_DESC(L"Model_JKO_JS_Strike", nullptr)		//	W + I
#define EFFECT_DESC_JKO_JS_STRIKE EFFECTDESC(true, LERP_NONE, 0.2f, 3.f, 32, 0, 0, 0, false, true)
#define EFFECT_SUBDESC_JKO_JS_BAKE SUB_EFFECT_DESC(L"Model_JKO_JS_Bake", nullptr)
#define EFFECT_DESC_JKO_JS_BAKE EFFECTDESC(true, LERP_NONE, 0.2f, 3.f, 4, 1, 1, 1, false, false)

#define EFFECT_SUBDESC_JKO_COMMAND SUB_EFFECT_DESC(L"Model_JKO_Command", nullptr)
#define EFFECT_DESC_JKO_COMMAND EFFECTDESC(true, LERP_NONE, 0.2f, 3.f, 4, 1, 1, 1, false, false)
#define EFFECT_DESC_JKO_COMMAND_BUBBLE EFFECTDESC(true, LERP_NONE, 0.2f, 3.5f, 4, 4, 2, 2, false, false)

#define EFFECT_SUBDESC_JKO_SUV_ATTACK SUB_EFFECT_DESC(L"Model_JKO_SUV_Attack", nullptr)
#define EFFECT_DESC_JKO_SUV_ATTACK EFFECTDESC(true, LERP_NONE, 0.2f, 4.5f, 19, 6, 3, 2, false, true)
#define EFFECT_SUBDESC_JKO_SUV_DEFEND SUB_EFFECT_DESC(L"Model_JKO_SUV_Defend", nullptr)
#define EFFECT_DESC_JKO_SUV_DEFEND EFFECTDESC(true, LERP_NONE, 0.5f, 1.f, SHADER_TURQUOISE_EFFECT_SPRITE_ZTEST, 1, 1, 1, false, true)
#define EFFECT_SUBDESC_JKO_SUV_HIT SUB_EFFECT_DESC(L"Model_JKO_SUV_Hit", nullptr)
#define EFFECT_DESC_JKO_SUV_HIT EFFECTDESC(true, LERP_NONE, 0.4f, 2.f, SHADER_YELLOW_EFFECT_SPRITE, 10, 5, 2, false, true)
#define EFFECT_SUBDESC_JKO_SUV_COUNTDOWN SUB_EFFECT_DESC(L"Model_JKO_SUV_Countdown", nullptr)
#define EFFECT_SUBDESC_JKO_SUV_COUNTDOWNREVERSE SUB_EFFECT_DESC(L"Model_JKO_SUV_CountdownReverse", nullptr)
#define EFFECT_DESC_JKO_SUV_COUNTDOWN EFFECTDESC(true, LERP_EseOutElastic, 1.f, 1.f, 4, 1, 1, 1, false, false)

#define EFFECT_SUBDESC_JKO_SUV_THROW_RING SUB_EFFECT_DESC(L"Model_JKO_SUV_Throw_Ring", nullptr)
#define EFFECT_DESC_JKO_SUV_THROW_RING EFFECTDESC(true, LERP_NONE, 0.5f, 1.f, 4, 6, 6, 1, false, false)
#define EFFECT_SUBDESC_JKO_SUV_THROW_BURST SUB_EFFECT_DESC(L"Model_JKO_SUV_Throw_Burst", nullptr)
#define EFFECT_DESC_JKO_SUV_THROW_BURST EFFECTDESC(true, LERP_NONE, 0.4f, 0.1f, SHADER_TURQUOISE_EFFECT_SPRITE_ZTEST, 3, 3, 1, false, true)

#define EFFECT_SUBDESC_JKO_SERVANT_SHOOT SUB_EFFECT_DESC(L"Model_JKO_Servant_Shoot", nullptr)
#define EFFECT_DESC_JKO_SERVANT_SHOOT EFFECTDESC(true, LERP_NONE, 0.3f, 3.f, 19, 6, 6, 1, false, true)

#define EFFECT_SUBDESC_IRONMAIDON SUB_EFFECT_DESC(L"Model_IronMaidon", nullptr)
#define EFFECT_DESC_IRONMAIDON EFFECTDESC(true, LERP_NONE, 5.f, 30.f, 5, 0, 0, 0, false, false)

#define EFFECT_SUBDESC_JKO_FLAG SUB_EFFECT_DESC(L"Model_JKO_Flag", nullptr)
#define EFFECT_DESC_JKO_FLAG EFFECTDESC(true, LERP_NONE, 5.f, 30.f, 5, 0, 0, 0, false, false)

#define EFFECT_SUBDESC_JKO_WHISTLE SUB_EFFECT_DESC(L"Model_JKO_Whistle", nullptr)
#define EFFECT_DESC_JKO_WHISTLE EFFECTDESC(true, LERP_NONE, 5.f, 30.f, 5, 0, 0, 0, false, false)

#define EFFECT_SUBDESC_JKO_CHEERKIRA SUB_EFFECT_DESC(L"Model_JKO_CheerKira", nullptr)
#define EFFECT_DESC_JKO_CHEERKIRA_BIG EFFECTDESC(true, LERP_NONE, 0.1f, 4.f, 17, 4, 2, 2, false, true)
#define EFFECT_DESC_JKO_CHEERKIRA_SMALL EFFECTDESC(true, LERP_NONE, 0.15f, 3.5f, 17, 6, 3, 2, false, true)

#define EFFECT_SUBDESC_JKO_SUV_CHEERKIRA SUB_EFFECT_DESC(L"Model_JKO_SUV_CheerKira", nullptr)
#define EFFECT_DESC_JKO_SUV_CHEERKIRA EFFECTDESC(true, LERP_NONE, 0.2f, 4.f, 17, 9, 3, 3, false, true)

#define EFFECT_SUBDESC_JKO_SUV_EXPLOSION SUB_EFFECT_DESC(L"Model_JKO_SUV_Explosion", nullptr)
#define EFFECT_DESC_JKO_SUV_EXPLOSION EFFECTDESC(true, LERP_NONE, 0.45f, 1.f, SHADER_YELLOW_EFFECT_SPRITE, 1, 1, 1, false, true)

#define EFFECT_SUBDESC_JKO_FED_BURST SUB_EFFECT_DESC(L"Model_JKO_FED_Burst", nullptr)
#define EFFECT_DESC_JKO_FED_BURST EFFECTDESC(true, LERP_NONE, 0.3f, 3.f, SHADER_RED_EFFECT_SRITE_ZTEST, 8, 1, 8, false, true, _float4(2.f, 2.f, 1.f, 1.f))

//	BGT_EFFECT
#define EFFECT_SUBDESC_BGT_5P SUB_EFFECT_DESC(L"Model_BGT_5P", nullptr)
#define EFFECT_DESC_BGT_5P EFFECTDESC(true, LERP_NONE, 0.3f, 4.f, 4, 1, 1, 1, false, false)

#define EFFECT_SUBDESC_BGT_CS SUB_EFFECT_DESC(L"Model_BGT_CS", nullptr)
#define EFFECT_DESC_BGT_CS EFFECTDESC(true, LERP_NONE, 0.4f, 3.5f, 4, 8, 4, 2, false, false)

#define EFFECT_SUBDESC_BGT_FS SUB_EFFECT_DESC(L"Model_BGT_FS", nullptr)
#define EFFECT_DESC_BGT_FS EFFECTDESC(true, LERP_NONE, 0.25f, 3.f, 4, 8, 4, 2, false, false)

#define EFFECT_SUBDESC_BGT_5D SUB_EFFECT_DESC(L"Model_BGT_5D", nullptr)
#define EFFECT_DESC_BGT_5D EFFECTDESC(true, LERP_NONE, 0.4f, 2.f, 4, 8, 4, 2, false, false)

#define EFFECT_SUBDESC_BGT_5H SUB_EFFECT_DESC(L"Model_BGT_5H", nullptr)
#define EFFECT_DESC_BGT_5H EFFECTDESC(true, LERP_NONE, 0.3f, 3.f, 4, 8, 4, 2, false, false)
#define EFFECT_SUBDESC_BGT_5H_LINE SUB_EFFECT_DESC(L"Model_BGT_5H_Line", nullptr)
#define EFFECT_DESC_BGT_5H_LINE EFFECTDESC(true, LERP_NONE, 0.3f, 3.f, 4, 5, 5, 1, false, true)

#define EFFECT_SUBDESC_BGT_2H SUB_EFFECT_DESC(L"Model_BGT_2H", nullptr)
#define EFFECT_DESC_BGT_2H EFFECTDESC(true, LERP_NONE, 0.5f, 3.f, 4, 8, 4, 2, false, false)
#define EFFECT_SUBDESC_BGT_2H_LINE SUB_EFFECT_DESC(L"Model_BGT_2H_Line", nullptr)
#define EFFECT_DESC_BGT_2H_LINE EFFECTDESC(true, LERP_NONE, 0.3f, 3.f, 4, 5, 5, 1, false, true)

#define EFFECT_SUBDESC_BGT_2D SUB_EFFECT_DESC(L"Model_BGT_2D", nullptr)
#define EFFECT_DESC_BGT_2D EFFECTDESC(true, LERP_NONE, 0.25f, 4.f, 4, 8, 4, 2, false, false)
#define EFFECT_SUBDESC_BGT_2D_LINE SUB_EFFECT_DESC(L"Model_BGT_2D_Line", nullptr)
#define EFFECT_DESC_BGT_2D_LINE EFFECTDESC(true, LERP_NONE, 0.25f, 3.f, 4, 5, 5, 1, false, true)

#define EFFECT_SUBDESC_BGT_S SUB_EFFECT_DESC(L"Model_BGT_S", nullptr)
#define EFFECT_DESC_BGT_S EFFECTDESC(true, LERP_NONE, 0.25f, 4.f, 4, 8, 4, 2, false, false)
#define EFFECT_SUBDESC_BGT_S_LINE SUB_EFFECT_DESC(L"Model_BGT_S_Line", nullptr)
#define EFFECT_DESC_BGT_S_LINE EFFECTDESC(true, LERP_NONE, 0.25f, 3.f, 4, 5, 5, 1, false, true)

#define EFFECT_SUBDESC_BGT_JH SUB_EFFECT_DESC(L"Model_BGT_JH", nullptr)
#define EFFECT_DESC_BGT_JH EFFECTDESC(true, LERP_NONE, 0.2f, 4.f, 4, 8, 4, 2, false, false)
#define EFFECT_SUBDESC_BGT_JH_LINE SUB_EFFECT_DESC(L"Model_BGT_JH_Line", nullptr)
#define EFFECT_DESC_BGT_JH_LINE EFFECTDESC(true, LERP_NONE, 0.2f, 3.f, 4, 5, 5, 1, false, true)

#define EFFECT_SUBDESC_BGT_JD SUB_EFFECT_DESC(L"Model_BGT_JD", nullptr)
#define EFFECT_DESC_BGT_JD EFFECTDESC(true, LERP_NONE, 0.3f, 4.f, 4, 1, 1, 1, false, false)

#define EFFECT_SUBDESC_BGT_6P SUB_EFFECT_DESC(L"Model_BGT_6P", nullptr)
#define EFFECT_DESC_BGT_6P EFFECTDESC(true, LERP_NONE, 0.3f, 4.f, 4, 1, 1, 1, false, false)

#define EFFECT_SUBDESC_BGT_2S SUB_EFFECT_DESC(L"Model_BGT_2S", nullptr)
#define EFFECT_DESC_BGT_2S EFFECTDESC(true, LERP_NONE, 0.25f, 3.f, 4, 8, 4, 2, false, false)

#define EFFECT_SUBDESC_BGT_JS SUB_EFFECT_DESC(L"Model_BGT_JS", nullptr)
#define EFFECT_DESC_BGT_JS EFFECTDESC(true, LERP_NONE, 0.2f, 4.f, 4, 8, 4, 2, false, false)

#define EFFECT_SUBDESC_BGT_H SUB_EFFECT_DESC(L"Model_BGT_H", nullptr)
#define EFFECT_DESC_BGT_H EFFECTDESC(true, LERP_NONE, 0.5f, 4.f, 4, 1, 1, 1, false, false)
#define EFFECT_SUBDESC_BGT_H_SUB SUB_EFFECT_DESC(L"Model_BGT_H_Sub", nullptr)
#define EFFECT_DESC_BGT_H_SUB EFFECTDESC(true, LERP_NONE, 0.5f, 4.f, 4, 1, 1, 1, false, false)
#define EFFECT_SUBDESC_BGT_H_LINE SUB_EFFECT_DESC(L"Model_BGT_H_Line", nullptr)
#define EFFECT_DESC_BGT_H_LINE EFFECTDESC(true, LERP_NONE, 0.5f, 3.f, 4, 5, 5, 1, false, true)

#define EFFECT_SUBDESC_BGT_LTL_R_START SUB_EFFECT_DESC(L"Model_BGT_LTL_R_Start", nullptr)
#define EFFECT_DESC_BGT_LTL_R_START EFFECTDESC(true, LERP_NONE, 0.5f, 4.f, SHADER_YELLOW_EFFECT_SPRITE, 1, 1, 1, false, true)
#define EFFECT_SUBDESC_BGT_LTL_R_LOOP SUB_EFFECT_DESC(L"Model_BGT_LTL_R_Loop", nullptr)
#define EFFECT_DESC_BGT_LTL_R_LOOP EFFECTDESC(true, LERP_NONE, 1.f, 4.f, SHADER_YELLOW_EFFECT_SPRITE, 1, 1, 1, false, true)
#define EFFECT_SUBDESC_BGT_LTL_L_START SUB_EFFECT_DESC(L"Model_BGT_LTL_L_Start", nullptr)
#define EFFECT_DESC_BGT_LTL_L_START EFFECTDESC(true, LERP_NONE, 0.5f, 4.f, 17, 1, 1, 1, false, true)
#define EFFECT_SUBDESC_BGT_LTL_L_LOOP SUB_EFFECT_DESC(L"Model_BGT_LTL_L_Loop", nullptr)
#define EFFECT_DESC_BGT_LTL_L_LOOP EFFECTDESC(true, LERP_NONE, 1.f, 4.f, 17, 1, 1, 1, false, true)

#define EFFECT_SUBDESC_BGT_LTL_ROGERPUNCH SUB_EFFECT_DESC(L"Model_BGT_LTL_RogerPunch", nullptr)
#define EFFECT_DESC_BGT_LTL_ROGERPUNCH EFFECTDESC(true, LERP_NONE, 0.5f, 4.f, 4, 1, 1, 1, false, false)
#define EFFECT_SUBDESC_BGT_LTL_ROGERBURST SUB_EFFECT_DESC(L"Model_BGT_LTL_RogerBurst", nullptr)
#define EFFECT_DESC_BGT_LTL_ROGERBURST EFFECTDESC(true, LERP_NONE, 0.3f, 3.f, SHADER_TURQUOISE_EFFECT_SPRITE_ZTEST, 6, 1, 6, false, true)
#define EFFECT_SUBDESC_BGT_ROGERSPEEDLINE SUB_EFFECT_DESC(L"Model_BGT_RogerSpeedline", nullptr)
#define EFFECT_DESC_BGT_ROGERSPEEDLINE EFFECTDESC(true, LERP_NONE, 0.2f, 3.f, 17, 3, 3, 1, false, true)

#define EFFECT_SUBDESC_BGT_ROGERSMOKE SUB_EFFECT_DESC(L"Model_BGT_RogerSmoke", nullptr)
#define EFFECT_DESC_BGT_ROGERSMOKE EFFECTDESC(true, LERP_NONE, 0.3f, 2.8f, 4, 4, 2, 2, false, false)

#define EFFECT_SUBDESC_BGT_SS_R_START SUB_EFFECT_DESC(L"Model_BGT_SS_R_Start", nullptr)		//	D + S + U
#define EFFECT_DESC_BGT_SS_R_START EFFECTDESC(true, LERP_NONE, 0.5f, 4.f, 19, 1, 1, 1, false, true)
#define EFFECT_SUBDESC_BGT_SS_R_LOOP SUB_EFFECT_DESC(L"Model_BGT_SS_R_Loop", nullptr)
#define EFFECT_DESC_BGT_SS_R_LOOP EFFECTDESC(true, LERP_NONE, 0.5f, 4.f, SHADER_ORANGE_EFFECT_SPRITE, 1, 1, 1, false, true)
#define EFFECT_SUBDESC_BGT_SS_R_END SUB_EFFECT_DESC(L"Model_BGT_SS_R_End", nullptr)
#define EFFECT_DESC_BGT_SS_R_END EFFECTDESC(true, LERP_NONE, 0.5f, 4.f, SHADER_ORANGE_EFFECT_SPRITE, 1, 1, 1, false, true)
#define EFFECT_SUBDESC_BGT_SS_L_START SUB_EFFECT_DESC(L"Model_BGT_SS_L_Start", nullptr)
#define EFFECT_DESC_BGT_SS_L_START EFFECTDESC(true, LERP_NONE, 0.5f, 4.f, 19, 1, 1, 1, false, true)
#define EFFECT_SUBDESC_BGT_SS_L_LOOP SUB_EFFECT_DESC(L"Model_BGT_SS_L_Loop", nullptr)
#define EFFECT_DESC_BGT_SS_L_LOOP EFFECTDESC(true, LERP_NONE, 0.5f, 4.f, 19, 1, 1, 1, false, true)
#define EFFECT_SUBDESC_BGT_SS_L_END SUB_EFFECT_DESC(L"Model_BGT_SS_L_End", nullptr)
#define EFFECT_DESC_BGT_SS_L_END EFFECTDESC(true, LERP_NONE, 0.5f, 4.f, 19, 1, 1, 1, false, true)

#define EFFECT_SUBDESC_BGT_RTR_BURST SUB_EFFECT_DESC(L"Model_BGT_RTR_Burst", nullptr)		//	S + D + U
#define EFFECT_DESC_BGT_RTR_BURST EFFECTDESC(true, LERP_NONE, 0.3f, 3.f, 17, 6, 1, 6, false, true)

#define EFFECT_SUBDESC_BGT_RD_AIRBURST SUB_EFFECT_DESC(L"Model_BGT_RD_AirBurst", nullptr)		//	W + S + D + J
#define EFFECT_DESC_BGT_RD_AIRBURST EFFECTDESC(true, LERP_NONE, 0.2f, 3.5f, SHADER_TURQUOISE_EFFECT_SPRITE_ZTEST, 6, 1, 6, false, true)
#define EFFECT_SUBDESC_BGT_RD_BURST SUB_EFFECT_DESC(L"Model_BGT_RD_Burst", nullptr)
#define EFFECT_DESC_BGT_RD_BURST EFFECTDESC(true, LERP_NONE, 0.3f, 3.f, SHADER_TURQUOISE_EFFECT_SPRITE_ZTEST, 6, 1, 6, false, true)

#define EFFECT_SUBDESC_BGT_KSMH_YOYOBAKE SUB_EFFECT_DESC(L"Model_BGT_KSMH_Yoyobake", nullptr)
#define EFFECT_DESC_BGT_KSMH_YOYOBAKE EFFECTDESC(true, LERP_NONE, 0.5f, 1.74f, 22, 1, 1, 1, false, false)
#define EFFECT_SUBDESC_BGT_KSMH_YOYOSPARK01 SUB_EFFECT_DESC(L"Model_BGT_KSMH_YoyoSpark01", nullptr)
#define EFFECT_SUBDESC_BGT_KSMH_YOYOSPARK02 SUB_EFFECT_DESC(L"Model_BGT_KSMH_YoyoSpark02", nullptr)
#define EFFECT_DESC_BGT_KSMH_YOYOSPARK EFFECTDESC(true, LERP_NONE, 1.f, 4.f, 17, 4, 2, 2, false, true)
#define EFFECT_SUBDESC_BGT_KSMH_YOYOLINE01 SUB_EFFECT_DESC(L"Model_BGT_KSMH_YoyoLine01", nullptr)
#define EFFECT_SUBDESC_BGT_KSMH_YOYOLINE02 SUB_EFFECT_DESC(L"Model_BGT_KSMH_YoyoLine02", nullptr)
#define EFFECT_DESC_BGT_KSMH_YOYOLINE EFFECTDESC(true, LERP_NONE, 0.5f, 1.74f, 4, 1, 1, 1, false, false)
#define EFFECT_SUBDESC_BGT_KSMH_YOYOSTRING01 SUB_EFFECT_DESC(L"Model_BGT_KSMH_YoyoString01", nullptr)
#define EFFECT_SUBDESC_BGT_KSMH_YOYOSTRING02 SUB_EFFECT_DESC(L"Model_BGT_KSMH_YoyoString02", nullptr)
#define EFFECT_DESC_BGT_KSMH_YOYOSTRING EFFECTDESC(true, LERP_NONE, 0.5f, 1.74f, 4, 1, 1, 1, false, false)
#define EFFECT_SUBDESC_BGT_KSMH_SPARK01 SUB_EFFECT_DESC(L"Model_BGT_KSMH_Spark01", nullptr)
#define EFFECT_DESC_BGT_KSMH_SPARK01 EFFECTDESC(true, LERP_NONE, 10.f, 3.f, 17, 8, 2, 4, false, true)
#define EFFECT_SUBDESC_BGT_KSMH_SPARK02 SUB_EFFECT_DESC(L"Model_BGT_KSMH_Spark02", nullptr)
#define EFFECT_DESC_BGT_KSMH_SPARK02 EFFECTDESC(true, LERP_NONE, 10.f, 3.f, SHADER_TURQUOISE_EFFECT_SPRITE_ZTEST, 8, 2, 4, false, true)
#define EFFECT_SUBDESC_BGT_KSMH_SPARKLINE01 SUB_EFFECT_DESC(L"Model_BGT_KSMH_SparkLine01", nullptr)
#define EFFECT_DESC_BGT_KSMH_SPARKLINE01 EFFECTDESC(true, LERP_NONE, 10.f, 3.f, 17, 8, 2, 4, false, true)
#define EFFECT_SUBDESC_BGT_KSMH_SPARKLINE02 SUB_EFFECT_DESC(L"Model_BGT_KSMH_SparkLine02", nullptr)
#define EFFECT_DESC_BGT_KSMH_SPARKLINE02 EFFECTDESC(true, LERP_NONE, 10.f, 3.f, SHADER_TURQUOISE_EFFECT_SPRITE_ZTEST, 8, 2, 4, false, true)

#define EFFECT_SUBDESC_BGT_RKM_SMOKE SUB_EFFECT_DESC(L"Model_BGT_RKM_Smoke", nullptr)
#define EFFECT_DESC_BGT_RKM_SMOKE EFFECTDESC(true, LERP_NONE, 0.3f, 3.f, 4, 16, 4, 4, false, false)
#define EFFECT_SUBDESC_BGT_RKM_BUBBLE SUB_EFFECT_DESC(L"Model_BGT_RKM_Bubble", nullptr)
#define EFFECT_DESC_BGT_RKM_BUBBLE EFFECTDESC(true, LERP_NONE, 0.2f, 3.f, 4, 1, 1, 1, false, false)
#define EFFECT_DESC_BGT_RKM_BUBBLE_RETURN EFFECTDESC(true, LERP_NONE, 0.2f, 3.5f, 4, 4, 2, 2, false, false)

#define EFFECT_SUBDESC_BGT_RKM_FIRERING SUB_EFFECT_DESC(L"Model_BGT_RKM_FireRing", nullptr)
#define EFFECT_DESC_BGT_RKM_FIRERING EFFECTDESC(true, LERP_NONE, 6.f, 3.f, 19, 7, 7, 1, false, true, _float4(2.5f, 0.7f, 0.3f, 1.f))
#define EFFECT_SUBDESC_BGT_RKM_FLOORFIRE SUB_EFFECT_DESC(L"Model_BGT_RKM_FloorFire", nullptr)
#define EFFECT_DESC_BGT_RKM_FLOORFIRE EFFECTDESC(true, LERP_NONE, 3.f, 3.f, 19, 7, 1, 7, false, true, _float4(3.f, 1.35f, 0.5f, 1.f))
#define EFFECT_SUBDESC_BGT_RKM_EMBER SUB_EFFECT_DESC(L"Model_BGT_RKM_Ember", nullptr)
#define EFFECT_DESC_BGT_RKM_EMBER EFFECTDESC(true, LERP_NONE, 0.25f, 5.f, 19, 9, 3, 3, false, true, _float4(3.f, 1.35f, 0.5f, 1.f))

#define EFFECT_SUBDESC_BGT_TAS_LINE SUB_EFFECT_DESC(L"Model_BGT_TAS_Line", nullptr)
#define EFFECT_DESC_BGT_TAS_LINE EFFECTDESC(true, LERP_NONE, 0.45f, 1.5f, 19, 6, 6, 1, false, true, _float4(3.f, 1.35f, 0.5f, 1.f))
#define EFFECT_SUBDESC_BGT_TAS_FIRE SUB_EFFECT_DESC(L"Model_BGT_TAS_Fire", nullptr)
#define EFFECT_DESC_BGT_TAS_FIRE EFFECTDESC(true, LERP_NONE, 0.45f, 2.5f, 19, 6, 3, 2, false, true, _float4(3.f, 1.35f, 0.5f, 1.f))

#pragma endregion

class CEffect abstract : public CGameObject
{
protected:
	explicit CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect(const CGameObject& rhs);
	virtual ~CEffect() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* Arg);
	virtual _int Tick(_double fTimeDelta);	//이펙트의 소멸을 담당
	virtual _int LateTick(_double fTimeDelta);
	virtual HRESULT Render();

protected:
	_float Lerp_Option();

protected:
	CRenderer* m_pRendererCom = nullptr;

protected:
	EFFECTDESC		m_EffectDesc;
	_uint			m_iSequence = 0;
	_uint			m_iCurLevel = 0;
	_float			m_fTime = 0.f;

public: // Create 함수는 static 으로 선언되기 때문에 순수가상함수로 정의 안 해둠
	virtual void Free() override;
};

END