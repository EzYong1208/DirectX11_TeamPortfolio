#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

class CMerged_Bird final : public CGameObject
{
private:
	CMerged_Bird(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CMerged_Bird(const CMerged_Bird& rhs);
	virtual ~CMerged_Bird() = default;

public:
	virtual HRESULT			NativeConstruct_Prototype();
	virtual HRESULT			NativeConstruct(void* pArg);

private:
	_uint					m_iCurLevel = 0;

private:
	void					SetUpBird();

public:
	static CMerged_Bird*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual	CGameObject*	Clone(void* pArg);
	virtual void			Free() override;
};

END