#pragma once

#include "Effect_None_Anim_Effect.h"

BEGIN(Client)

class CEffect_BKN_Sanju_Slash final : public CEffect_None_Anim_Effect
{
public:
	typedef	enum tagBKNSanjuSlashMaterialIndex
	{
		SLASH_00, SLASH_01, SLASH_02, SLASH_03, SLASH_04, SLASH_06, SLASH_08, SLASH_12, SLASH_END
	}MATERIALINDEX;

	typedef struct tagBKNSanjuSlashDesc
	{
		tagBKNSanjuSlashDesc() = default;
		tagBKNSanjuSlashDesc(EFFECTDESC _EffDesc, SUB_EFFECT_DESC _SubEffDesc)
			: EffectDesc(_EffDesc)
			, SubEffectDesc(_SubEffDesc) {}

		EFFECTDESC			EffectDesc;
		SUB_EFFECT_DESC		SubEffectDesc;
		MATERIALINDEX		eIndex;
	}BKNSANJUSLASHDESC;

private:
	explicit CEffect_BKN_Sanju_Slash(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_BKN_Sanju_Slash(const CEffect_BKN_Sanju_Slash& rhs);
	virtual ~CEffect_BKN_Sanju_Slash() = default;

public:
	virtual HRESULT						NativeConstruct_Prototype();
	virtual HRESULT						NativeConstruct(void* pArg);
	virtual _int						Tick(_double TimeDelta);
	virtual _int						LateTick(_double TimeDelta);
	virtual HRESULT						Render();

private:
	BKNSANJUSLASHDESC					m_BKNSanjuSlashDesc;
	_uint								m_iMaterialIndex = 0;

private:
	void								SetMaterialIndex();

public:
	static	CEffect_BKN_Sanju_Slash*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject*				Clone(void* pArg);
	virtual void						Free() override;

};

END