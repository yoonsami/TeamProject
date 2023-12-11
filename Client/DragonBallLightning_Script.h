#pragma once
#include "MonoBehaviour.h"

class DragonBallLightning_Script : public MonoBehaviour
{
public:
	enum DIR {LEFT, RIGHT};

public:
	DragonBallLightning_Script();
	~DragonBallLightning_Script();

public:
	virtual void Tick() override;
	virtual shared_ptr<MonoBehaviour> Copy_Script()override { return make_shared<DragonBallLightning_Script>(); }

	void		Set_Direction(DIR eDir) { m_eDirection = eDir; }
private:
	void		Add_Effect(const wstring& wstrSkillTag, const _float4x4& matWorldPos);

private:
	DIR			m_eDirection = { LEFT };

	_float		m_fCurrAge = { 0.f };
	_float		m_fCoolTime_CreateEffect = { 0.f };
};

