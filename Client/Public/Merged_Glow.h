#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

class CMerged_Glow final : public CGameObject
{
private:
	CMerged_Glow(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CMerged_Glow(const CMerged_Glow& rhs);
	virtual ~CMerged_Glow() = default;

public:
	virtual HRESULT			NativeConstruct_Prototype();
	virtual HRESULT			NativeConstruct(void* pArg);

private:
	_uint					m_iCurLevel = 0;

private:
	HRESULT					SetUpGlow();

public:
	static CMerged_Glow*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual	CGameObject*	Clone(void* pArg);
	virtual void			Free() override;
};

END