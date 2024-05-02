#include "stdafx.h"
#include "Effect_Glow_09.h"
#include "GameInstance.h"

CEffect_Glow_09::CEffect_Glow_09(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
    : CEffect_None_Anim_Effect(pDevice, pDeviceContext)
{
}

CEffect_Glow_09::CEffect_Glow_09(const CEffect_Glow_09& rhs)
    : CEffect_None_Anim_Effect(rhs)
{
}

HRESULT CEffect_Glow_09::NativeConstruct_Prototype()
{
    if (FAILED(__super::NativeConstruct_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CEffect_Glow_09::NativeConstruct(void* pArg)
{
    //	Ʈ������, ��, ���̴�, ������ �߰�
    if (FAILED(__super::NativeConstruct(pArg)))
        return E_FAIL;

    //	Ʈ���� �ؽ�ó �߰�
    if (FAILED(SetUp_Components()))
        return E_FAIL;

    XMStoreFloat3(&m_EffectDesc.vScaleOrigin, m_pTransform->Get_Scale());

    return S_OK;
}

_int CEffect_Glow_09::Tick(_double TimeDelta)
{
    int temp = 0;

    m_fTime += (_float)TimeDelta * m_EffectDesc.fSpeed;
    m_fTimeDelta += TimeDelta;

    switch (m_iSequence)
    {
    case 0:

        if (m_EffectDesc.fLifetime >= 0.f)
            m_EffectDesc.fLifetime -= (_float)TimeDelta;
        else
            temp = ISDEAD;

        break;

    default:
        break;
    }

    m_pTransform->Reset_Momentum();

    return temp;
}

_int CEffect_Glow_09::LateTick(_double TimeDelta)
{
    if (0 > __super::LateTick(TimeDelta))
        return -1;

    //�������� �߰�
    //if (m_EffectDesc.bGlow)
    //	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT_FIRST_GLOW, this);
    m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT_SECOND, this);

    return 0;
}

HRESULT CEffect_Glow_09::Render()
{
    if (FAILED(SetUp_ConstantTable_BillBoarding()))
        return E_FAIL;

    //	���� ���ε�
    if (FAILED(m_pShaderCom->Set_RawValue("g_vRGBA", &m_EffectDesc.vRGBA, sizeof(_float4))))
        return E_FAIL;

    //	�ִ� ������ ����(��������Ʈ) ����
    if (FAILED(m_pShaderCom->Set_RawValue("g_iCX", &m_EffectDesc.iTexNumX, sizeof(int))))
        return E_FAIL;

    //	�ִ� ������ ����(��������Ʈ) ����
    if (FAILED(m_pShaderCom->Set_RawValue("g_iCY", &m_EffectDesc.iTexNumY, sizeof(int))))
        return E_FAIL;

    _float fAlpha = 0.f;
    if (FAILED(m_pShaderCom->Set_RawValue("g_Alpha", &fAlpha, sizeof(_float))))
        return E_FAIL;

    //	�ؽ�ó ������ ���ε�
    _float2 vSeqUV = { 0.f, 0.f };
    if (m_EffectDesc.iTexFrameMax > 1)
    {
        vSeqUV.x = 0;
        vSeqUV.y = 0;
    }
    else
    {
        m_EffectDesc.iTexNumX = 1;
        m_EffectDesc.iTexNumY = 1;
    }

    if (FAILED(m_pShaderCom->Set_RawValue("g_SeqUV", &vSeqUV, sizeof(_float2))))
        return E_FAIL;

    m_pModelCom->Bind_OnShader(m_pShaderCom, 0, aiTextureType_DIFFUSE, "g_DiffuseTexture");
    m_pModelCom->Render(m_pShaderCom, m_EffectDesc.iEffectPass, 0);

    return S_OK;
}

void CEffect_Glow_09::Trigger_Activate(const _double& _fTimeDelta)
{
}

HRESULT CEffect_Glow_09::SetUp_Components()
{
    return S_OK;
}

CEffect_Glow_09* CEffect_Glow_09::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
    CEffect_Glow_09* pInstance = new CEffect_Glow_09(pDevice, pDeviceContext);

    if (FAILED(pInstance->NativeConstruct_Prototype()))
    {
        MSGBOX("Failed to Created CEffect_Glow_09");
        Safe_Release(pInstance);
    }
    return pInstance;
}

CGameObject* CEffect_Glow_09::Clone(void* pArg)
{
    CEffect_Glow_09* pInstance = new CEffect_Glow_09(*this);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        MSGBOX("Failed to Created CEffect_Glow_09");
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CEffect_Glow_09::Free()
{
    __super::Free();
}
