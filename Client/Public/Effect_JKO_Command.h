#pragma once

#include "Effect_None_Anim_Effect.h"

BEGIN(Client)

class CEffect_JKO_Command final : public CEffect_None_Anim_Effect
{
public:
	typedef enum tagJKOCommandMaterialIndex
	{
		COMMAND_DEFEND, COMMAND_ATTACK, COMMAND_RECOVER, COMMAND_EXPLOSION, 
		COMMAND_DEFENDBUBBLE, COMMAND_ATTACKBUBBLE, COMMAND_RECOVERBUBBLE, COMMAND_EXPLOSIONBUBBLE,
		COMMAND_END
	}MATERIALINDEX;

	typedef enum tagJKOCommandType
	{
		TYPE_GROUND, TYPE_AIR, TYPE_END
	}COMMANDTYPE;

	typedef struct tagJKOCommandDesc
	{
		tagJKOCommandDesc() = default;
		tagJKOCommandDesc(EFFECTDESC _EffDesc, SUB_EFFECT_DESC _SubEffDesc)
			: EffectDesc(_EffDesc)
			, SubEffectDesc(_SubEffDesc) {}

		EFFECTDESC			EffectDesc;
		SUB_EFFECT_DESC		SubEffectDesc;
		MATERIALINDEX		eIndex;
		COMMANDTYPE			eType;
	}JKOCOMMANDDESC;

private:
	explicit CEffect_JKO_Command(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_JKO_Command(const CEffect_JKO_Command& rhs);
	virtual ~CEffect_JKO_Command() = default;

public:
	virtual HRESULT					NativeConstruct_Prototype();
	virtual HRESULT					NativeConstruct(void* pArg);
	virtual _int					Tick(_double TimeDelta);
	virtual _int					LateTick(_double TimeDelta);
	virtual HRESULT					Render();

private:
	JKOCOMMANDDESC					m_JKOCommandDesc;
	_uint							m_iMaterialIndex = 0;

private:
	void							SetMaterialIndex();
	HRESULT							SetPosition();

public:
	static	CEffect_JKO_Command*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject*			Clone(void* pArg);
	virtual void					Free() override;

};

END