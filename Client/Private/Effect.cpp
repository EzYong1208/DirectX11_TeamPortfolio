#include "stdafx.h"
#include "Effect.h"
#include "GameInstance.h"

CEffect::CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CEffect::CEffect(const CGameObject& rhs)
	: CGameObject(rhs)
{
}

HRESULT CEffect::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect::NativeConstruct(void* Arg)
{
	if (FAILED(__super::NativeConstruct(nullptr)))
		return E_FAIL;

	if (Arg != nullptr)
		memcpy(&m_EffectDesc, Arg, sizeof(EFFECTDESC));

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(
		LEVEL_STATIC,
		TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"),
		(CComponent**)&m_pRendererCom)))
		return E_FAIL;

	return S_OK;
}

_int CEffect::Tick(_double fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
		return -1;

	return 0;
}

_int CEffect::LateTick(_double fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	return 0;
}

HRESULT CEffect::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

_float CEffect::Lerp_Option()
{
	switch (m_EffectDesc.eLerp_Option)
	{
	case LERP_EseInElastic:
		return NONG::EseInElastic(m_EffectDesc.fAlpha);

	case LERP_EseOutElastic:
		return NONG::EseOutElastic(m_EffectDesc.fAlpha);

	case LERP_EaseInOutCirc:
		return NONG::EaseInOutCirc(m_EffectDesc.fAlpha);

	case LERP_EaseOutBounce:
		return NONG::EaseOutBounce(m_EffectDesc.fAlpha);

	case LERP_EaseInBack:
		return NONG::EaseInBack(m_EffectDesc.fAlpha);

	case LERP_EaseInOutBack:
		return NONG::EaseInOutBack(m_EffectDesc.fAlpha);

	case LERP_EaseOutExpo:
		return NONG::EaseOutExpo(m_EffectDesc.fAlpha);

	case LERP_EaseOutBack:
		return NONG::EaseOutBack(m_EffectDesc.fAlpha);

	case LERP_EaseInExpo:
		return NONG::EaseInExpo(m_EffectDesc.fAlpha);

	case LERP_EaseOutCirc:
		return NONG::EaseOutCirc(m_EffectDesc.fAlpha);

	case LERP_NONE:
	default:
		return m_EffectDesc.fAlpha;
	}
}

void CEffect::Free()
{
	__super::Free();
	Safe_Release(m_pRendererCom);
}
