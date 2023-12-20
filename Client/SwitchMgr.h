#pragma once

enum SWITCH_COUNT
{
	MAX_SWITCH_COUNT = (_int)(SWITCH_TYPE::MAX)
};
enum SCORE_COUNT
{
	MAX_SCORE_COUNT = (_int)(SCORE_TYPE::MAX)
};

class SwitchMgr
{
	DECLARE_SINGLETON(SwitchMgr)

public:
	_bool Get_SwitchState(SWITCH_TYPE eType) { return m_Switches[(_int)(eType)]; }
	void Set_SwitchState(SWITCH_TYPE eType, _bool bSwitch) { m_Switches[(_int)(eType)] = bSwitch; }

	_uint Get_Score(SCORE_TYPE eType) { return m_Scores[(_int)(eType)]; }
	void Add_Score(SCORE_TYPE eType) { m_Scores[(_int)(eType)]++; }
private:
	array<_bool, MAX_SWITCH_COUNT> m_Switches{false};
	array<_uint, MAX_SCORE_COUNT> m_Scores{0};
};

