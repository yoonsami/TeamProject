#pragma once
#include "FSM.h"
class DragonBall_FSM :
    public FSM
{
public:
    enum class STATE
    {
        Idle,
        Crash,
        NONE,
    };
public:
    virtual void Tick() override;
    virtual void OnCollision(shared_ptr<BaseCollider> pCollider, _float fGap) override {}
    virtual void OnCollisionEnter(shared_ptr<BaseCollider> pCollider, _float fGap) override;
    virtual void OnCollisionExit(shared_ptr<BaseCollider> pCollider, _float fGap) override {}
    virtual void Set_State(_uint iIndex) override { m_eCurState = static_cast<STATE>(iIndex); }
    virtual void Get_Hit(const wstring& skillname, shared_ptr<GameObject> pLookTarget) override {}

protected:
    virtual void State_Tick() override;
    virtual void State_Init() override;
    virtual void AttackCollider_On(const wstring& skillname) override {}
    virtual void AttackCollider_Off() override {}

private:
    void Idle();
    void Idle_Init();
    void Crash();
    void Crash_Init();

private:
    STATE m_eCurState = STATE::Idle;
    STATE m_ePreState = STATE::NONE;


};

