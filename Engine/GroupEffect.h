#pragma once
#include "Component.h"

class GroupEffect final : public Component
{
public:
	typedef struct tagGroupEffect
	{
		const char*			pszTag;
		weak_ptr<Transform> pParentTransform;
	}DESC;

public:
	GroupEffect(shared_ptr<Shader> pShader);
	virtual ~GroupEffect();

public:
	virtual void			Init(void* pArg);
	virtual void			Tick() override;
	virtual void			Final_Tick() override;      

	void					Update_Desc();

	void					Start(){ m_bStart = true; }

private:
	DESC					m_tDesc;
	_bool					m_bStart = { false };
	_float					m_fCurrAge = { 0.f };
	map<string, pair<_bool, _float>>	m_vEffects;	// Tag, Activate, Time

	/* Component */
	shared_ptr<Shader>      m_pShader = { nullptr };

};

