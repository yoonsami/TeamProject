#pragma once

enum SWITCH_COUNT
{
	MAX_SWITCH_COUNT = (_int)(SWITCH_TYPE::MAX)
};

class SwitchMgr
{
	DECLARE_SINGLETON(SwitchMgr)

public:
	_bool Get_SwitchState(SWITCH_TYPE eType) { return m_Switches[(_int)(eType)]; }
	void Set_SwitchState(SWITCH_TYPE eType, _bool bSwitch) { m_Switches[(_int)(eType)] = bSwitch; }
private:
	array<_bool, MAX_SWITCH_COUNT> m_Switches{false};
};

