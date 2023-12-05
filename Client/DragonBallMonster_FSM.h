#pragma once
#include "FSM.h"
#include "ForwardMovingSkillScript.h"
#include "FloorSkill_Script.h"

class DragonBallMonster_FSM :
    public FSM
{
public:
    enum class STATE
    {
        Idle,
        Crash,
        NONE
    };
public:
    virtual HRESULT Init() override;
    virtual void Tick() override;
    virtual void OnCollision(shared_ptr<BaseCollider> pCollider, _float fGap) override {}
    virtual void OnCollisionEnter(shared_ptr<BaseCollider> pCollider, _float fGap) override;
    virtual void OnCollisionExit(shared_ptr<BaseCollider> pCollider, _float fGap) override {}
    virtual void Set_State(_uint iIndex) override { m_eCurState = static_cast<STATE>(iIndex); }
    virtual void Get_Hit(const wstring& skillname, _float fDamage, shared_ptr<GameObject> pLookTarget) override;

protected:
    virtual void State_Tick() override;
    virtual void State_Init() override;
    virtual void AttackCollider_On(const wstring& skillname, _float fAttackDamage)override {}
    virtual void AttackCollider_Off() override {}

private:
    void Idle();
    void Idle_Init();
    void Crash();
    void Crash_Init();

    void Summon_CrossFloor(); // +
    void Summon_X_Floor(); // X
    void Summon_Hash_Floor();  // #
    void Summon_Web_Floor();   
    void Summon_HalfCircle_Floor(); // D
    void Summon_Star_Floor(); 

    void Create_Meteor();



    void Set_AttackPattern();
    void Create_ForwardMovingSkillCollider(const _float4& vPos, _float fSkillRange, FORWARDMOVINGSKILLDESC desc, const wstring& SkillType, _float fAttackDamage);
    void Create_FloorSkillCollider(const _float4& vPos, _float3 vSkillScale, FLOORSKILLDESC desc);
    
private:
    STATE m_eCurState = STATE::Idle;
    STATE m_ePreState = STATE::NONE;

    _uint m_iPreAttack = 100;
    _uint m_iCurMeteorCnt = 0;
    _uint m_iLimitMeteorCnt = 0;

    _bool m_bSummonMeteor = false;

    COOLTIMEINFO m_tPatternCoolTime = { 4.f, 0.f };
    COOLTIMEINFO m_tMeteorCoolTime = { 2.f, 0.f };

};

