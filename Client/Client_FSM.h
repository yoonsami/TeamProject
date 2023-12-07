#pragma once
#include "FSM.h"
class Client_FSM abstract :
    public FSM
{

protected:
	virtual void AttackCollider_On(const wstring& skillname, _float fAttackDamage) override;
	virtual void AttackCollider_Off() override;
};

