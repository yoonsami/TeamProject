#pragma once
#include "FSM.h"
class NPC_FSM abstract:
    public FSM
{
public:

public:
    virtual void InteractWithPlayer() abstract;
    virtual void AttackCollider_On(const wstring& skillname, _float fAttackDamage) override {}
    virtual void AttackCollider_Off() override {}

protected:
    _float3 m_vMinMovingPoint{};
    _float3 m_vMaxMovingPoint{};

    _float m_fStateAcc = 0.f;

    _float m_fInteractRange = 1.f;
    _float m_fMoveTimer = 3.f;
    _float m_fIdleTimer = 5.f; 
};

