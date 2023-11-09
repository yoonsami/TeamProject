#include "pch.h"
#include "MainCameraScript.h"
#include "Dellons_FSM.h"
#include "ModelAnimator.h"
#include "MotionTrailRenderer.h"
#include "Debug_CreateMotionTrail.h"
#include "SphereCollider.h"
#include "AttackColliderInfoScript.h"
#include "Model.h"
#include "DellonsWraith_FSM.h"



Dellons_FSM::Dellons_FSM()
{
}

Dellons_FSM::~Dellons_FSM()
{
}

HRESULT Dellons_FSM::Init()
{
    auto animator = Get_Owner()->Get_Animator();
    if (animator)
    {
        animator->Set_CurrentAnim(L"b_idle", true, 1.f);
        m_eCurState = STATE::b_idle;
    }
    shared_ptr<GameObject> attackCollider = make_shared<GameObject>();
    attackCollider->GetOrAddTransform();
    attackCollider->Add_Component(make_shared<SphereCollider>(1.f));
    attackCollider->Get_Collider()->Set_CollisionGroup(Player_Attack);

    m_pAttackCollider = attackCollider;

    CUR_SCENE->Add_GameObject(m_pAttackCollider.lock());
    m_pAttackCollider.lock()->Get_Collider()->Set_Activate(false);

    m_pAttackCollider.lock()->Add_Component(make_shared<AttackColliderInfoScript>());
    m_pAttackCollider.lock()->Set_Name(L"Player_AttackCollider");

    m_pWeapon = CUR_SCENE->Get_GameObject(L"Weapon_Dellons");

    return S_OK;
}

void Dellons_FSM::Tick()
{
    State_Tick();

    if (!m_pAttackCollider.expired())
    {
        //m_pAttack transform set forward
        m_pAttackCollider.lock()->Get_Transform()->Set_State(Transform_State::POS, Get_Transform()->Get_State(Transform_State::POS) + Get_Transform()->Get_State(Transform_State::LOOK) * 2.f + _float3::Up);
    }
}

void Dellons_FSM::State_Tick()
{
    State_Init();

    switch (m_eCurState)
    {
    case STATE::b_idle:
        b_idle();
        break;
    case STATE::b_run_start:
        b_run_start();
        break;
    case STATE::b_run:
        b_run();
        break;
    case STATE::b_run_end_r:
        b_run_end_r();
        break;
    case STATE::b_run_end_l:
        b_run_end_l();
        break;
    case STATE::b_sprint:
        b_sprint();
        break;
    case STATE::b_walk:
        b_walk();
        break;
    case STATE::die:
        die();
        break;
    case STATE::airborne_start:
        airborne_start();
        break;
    case STATE::airborne_end:
        airborne_end();
        break;
    case STATE::airborne_up:
        airborne_up();
        break;
    case STATE::hit:
        hit();
        break;
    case STATE::knock_start:
        knock_start();
        break;
    case STATE::knock_end:
        knock_end();
        break;
    case STATE::knock_end_loop:
        knock_end_loop();
        break;
    case STATE::knock_end_hit:
        knock_end_hit();
        break;
    case STATE::knock_up:
        knock_up();
        break;
    case STATE::knockdown_start:
        knockdown_start();
        break;
    case STATE::knockdown_end:
        knockdown_end();
        break;
    case STATE::skill_1100:
        skill_1100();
        break;
    case STATE::skill_1200:
        skill_1200();
        break;
    case STATE::skill_1300:
        skill_1300();
        break;
    case STATE::skill_1400:
        skill_1400();
        break;
    case STATE::skill_91100:
        skill_91100();
        break;
    case STATE::skill_93100:
        skill_93100();
        break;
    case STATE::skill_100100:
        skill_100100();
        break;
    case STATE::skill_100200:
        skill_100200();
        break;
    case STATE::skill_200100:
        skill_200100();
        break;
    case STATE::skill_200200:
        skill_200200();
        break;
    case STATE::skill_300100:
        skill_300100();
        break;
    case STATE::skill_400100:
        skill_400100();
        break;
    case STATE::skill_501100:
        skill_501100();
        break;
    }
}

void Dellons_FSM::State_Init()
{
    if (m_eCurState != m_ePreState)
    {
        switch (m_eCurState)
        {
        case STATE::b_idle:
            b_idle_Init();
            break;
        case STATE::b_run_start:
            b_run_start_Init();
            break;
        case STATE::b_run:
            b_run_Init();
            break;
        case STATE::b_run_end_r:
            b_run_end_r_Init();
            break;
        case STATE::b_run_end_l:
            b_run_end_l_Init();
            break;
        case STATE::b_sprint:
            b_sprint_Init();
            break;
        case STATE::b_walk:
            b_walk_Init();
            break;
        case STATE::die:
            die_Init();
            break;
        case STATE::airborne_start:
            airborne_start_Init();
            break;
        case STATE::airborne_end:
            airborne_end_Init();
            break;
        case STATE::airborne_up:
            airborne_up_Init();
            break;
        case STATE::hit:
            hit_Init();
            break;
        case STATE::knock_start:
            knock_start_Init();
            break;
        case STATE::knock_end:
            knock_end_Init();
            break;
        case STATE::knock_end_loop:
            knock_end_loop_Init();
            break;
        case STATE::knock_end_hit:
            knock_end_hit_Init();
            break;
        case STATE::knock_up:
            knock_up_Init();
            break;
        case STATE::knockdown_start:
            knockdown_start_Init();
            break;
        case STATE::knockdown_end:
            knockdown_end_Init();
            break;
        case STATE::skill_1100:
            skill_1100_Init();
            break;
        case STATE::skill_1200:
            skill_1200_Init();
            break;
        case STATE::skill_1300:
            skill_1300_Init();
            break;
        case STATE::skill_1400:
            skill_1400_Init();
            break;
        case STATE::skill_91100:
            skill_91100_Init();
            break;
        case STATE::skill_93100:
            skill_93100_Init();
            break;
        case STATE::skill_100100:
            skill_100100_Init();
            break;
        case STATE::skill_100200:
            skill_100200_Init();
            break;
        case STATE::skill_200100:
            skill_200100_Init();
            break;
        case STATE::skill_200200:
            skill_200200_Init();
            break;
        case STATE::skill_300100:
            skill_300100_Init();
            break;
        case STATE::skill_400100:
            skill_400100_Init();
            break;
        case STATE::skill_501100:
            skill_501100_Init();
            break;
        }
        m_ePreState = m_eCurState;
    }
}

void Dellons_FSM::OnCollision(shared_ptr<BaseCollider> pCollider, _float fGap)
{
}

void Dellons_FSM::OnCollisionEnter(shared_ptr<BaseCollider> pCollider, _float fGap)
{
    if (pCollider->Get_Owner() == nullptr)
        return;

    wstring strSkillName = pCollider->Get_Owner()->Get_Script<AttackColliderInfoScript>()->Get_SkillName();

    if (!m_bInvincible)
    {
        Get_Hit(strSkillName, pCollider);
    }
}

void Dellons_FSM::OnCollisionExit(shared_ptr<BaseCollider> pCollider, _float fGap)
{
}

void Dellons_FSM::Get_Hit(const wstring& skillname, shared_ptr<BaseCollider> pOppositeCollider)
{
    if (skillname == NORMAL_ATTACK || skillname == KNOCKBACK_ATTACK || skillname == KNOCKDOWN_ATTACK || skillname == AIRBORNE_ATTACK)
    {
        _float3 vAttackerPos = pOppositeCollider->Get_Owner()->Get_Script<AttackColliderInfoScript>()->Get_ColliderOwner()->Get_Transform()->Get_State(Transform_State::POS).xyz();
        m_vHitDir = vAttackerPos - Get_Transform()->Get_State(Transform_State::POS);
        m_vHitDir.y = 0.f;
        m_vHitDir.Normalize();
    }
    else if (skillname == NORMAL_SKILL || skillname == KNOCKBACK_SKILL || skillname == KNOCKDOWN_SKILL || skillname == AIRBORNE_SKILL)
    {
        m_vHitDir = pOppositeCollider->Get_CenterPos() - Get_Transform()->Get_State(Transform_State::POS);
        m_vHitDir.y = 0.f;
        m_vHitDir.Normalize();
    }

    if (skillname == NORMAL_ATTACK || skillname == NORMAL_SKILL)
    {
        if (!m_bSuperArmor)
        {
            if (m_eCurState == STATE::hit)
                Reset_Frame();
            else if (m_eCurState == STATE::knock_end_hit)
                Reset_Frame();
            else if (m_eCurState == STATE::knock_end_loop)
                m_eCurState = STATE::knock_end_hit;
            else
                m_eCurState = STATE::hit;
        }
    }
    else if (skillname == KNOCKBACK_ATTACK || skillname == KNOCKBACK_SKILL)
    {
        if (!m_bSuperArmor)
        {
            if (m_eCurState == STATE::knock_end_hit)
                Reset_Frame();
            else if (m_eCurState == STATE::knock_end_loop)
                m_eCurState = STATE::knock_end_hit;
            else
                m_eCurState = STATE::knock_start;
        }
    }
    else if (skillname == KNOCKDOWN_ATTACK || skillname == KNOCKDOWN_SKILL)
    {
        if (!m_bSuperArmor)
        {
            if (m_eCurState == STATE::knock_end_hit)
                Reset_Frame();
            else if (m_eCurState == STATE::knock_end_loop)
                m_eCurState = STATE::knock_end_hit;
            else
                m_eCurState = STATE::knockdown_start;
        }
    }
    else if (skillname == AIRBORNE_ATTACK || skillname == AIRBORNE_SKILL)
    {
        if (!m_bSuperArmor)
        {
            if (m_eCurState == STATE::knock_end_hit)
                Reset_Frame();
            else if (m_eCurState == STATE::knock_end_loop)
                m_eCurState = STATE::knock_end_hit;
            else
                m_eCurState = STATE::airborne_start;
        }
    }
}

void Dellons_FSM::AttackCollider_On(const wstring& skillname)
{
    if (!m_pAttackCollider.expired())
    {
        m_pAttackCollider.lock()->Get_Collider()->Set_Activate(true);
        m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_SkillName(skillname);
    }
}

void Dellons_FSM::AttackCollider_Off()
{
    if (!m_pAttackCollider.expired())
    {
        m_pAttackCollider.lock()->Get_Collider()->Set_Activate(false);
        m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_SkillName(L"");
    }
}

void Dellons_FSM::Set_State(_uint iIndex)
{
}

void Dellons_FSM::b_idle()
{
    EvadeCoolCheck();

    _float3 vInputVector = Get_InputDirVector();

    if (KEYPUSH(KEY_TYPE::W) || KEYPUSH(KEY_TYPE::S) ||
        KEYPUSH(KEY_TYPE::A) || KEYPUSH(KEY_TYPE::D))
        m_eCurState = STATE::b_run_start;

    if (KEYPUSH(KEY_TYPE::LBUTTON))
        m_eCurState = STATE::skill_1100;
    else if (KEYPUSH(KEY_TYPE::KEY_1))
        m_eCurState = STATE::skill_100100;
    else if (KEYPUSH(KEY_TYPE::KEY_2))
        m_eCurState = STATE::skill_200100;
    else if (KEYPUSH(KEY_TYPE::KEY_3))
        m_eCurState = STATE::skill_300100;
    else if (KEYPUSH(KEY_TYPE::KEY_4))
        m_eCurState = STATE::skill_400100;
    else if (KEYPUSH(KEY_TYPE::KEY_5))
        m_eCurState = STATE::skill_501100;
    else if (KEYPUSH(KEY_TYPE::SPACE))
    {
        if (!m_bEvadeCoolCheck)
        {
            if (vInputVector == _float3(0.f))
                m_eCurState = STATE::skill_93100;
            else
                m_eCurState = STATE::skill_91100;
        }
    }
}

void Dellons_FSM::b_idle_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    if (m_ePreState != STATE::skill_400100)
        animator->Set_NextTweenAnim(L"b_idle", 0.1f, true, 1.f);
    else
        animator->Set_NextTweenAnim(L"b_idle", 0.3f, true, 1.f);

    Get_Transform()->Set_Speed(m_fRunSpeed);
    m_tRunEndDelay.fAccTime = 0.f;

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Dellons_FSM::b_run_start()
{
    EvadeCoolCheck();

    Get_Transform()->Go_Straight();

    _float3 vInputVector = Get_InputDirVector();

    if (vInputVector == _float3(0.f))
    {
        m_tRunEndDelay.fAccTime += fDT;

        if (m_tRunEndDelay.fAccTime >= m_tRunEndDelay.fCoolTime)
            m_eCurState = STATE::b_idle;
    }
    else
    {
        if (Is_AnimFinished())
            m_eCurState = STATE::b_run;

        Soft_Turn_ToInputDir(vInputVector, XM_PI * 5.f);

        if (KEYPUSH(KEY_TYPE::LBUTTON))
            m_eCurState = STATE::skill_1100;
        else if (KEYPUSH(KEY_TYPE::KEY_1))
            m_eCurState = STATE::skill_100100;
        else if (KEYPUSH(KEY_TYPE::KEY_2))
            m_eCurState = STATE::skill_200100;
        else if (KEYPUSH(KEY_TYPE::KEY_3))
            m_eCurState = STATE::skill_300100;
        else if (KEYPUSH(KEY_TYPE::KEY_4))
            m_eCurState = STATE::skill_400100;
        else if (KEYPUSH(KEY_TYPE::KEY_5))
            m_eCurState = STATE::skill_501100;
        else if (KEYPUSH(KEY_TYPE::SPACE))
        {
            if (!m_bEvadeCoolCheck)
                m_eCurState = STATE::skill_91100;
        }
    }
}

void Dellons_FSM::b_run_start_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"b_run_start", 0.1f, false, 1.5f);

    Get_Transform()->Set_Speed(m_fRunSpeed);
    m_tRunEndDelay.fAccTime = 0.f;

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Dellons_FSM::b_run()
{
    EvadeCoolCheck();

    Get_Transform()->Go_Straight();

    _float3 vInputVector = Get_InputDirVector();

    if (vInputVector == _float3(0.f))
    {
        m_tRunEndDelay.fAccTime += fDT;

        if (m_tRunEndDelay.fAccTime >= m_tRunEndDelay.fCoolTime)
        {
            if (Get_CurFrame() % 2 == 0)
                m_eCurState = STATE::b_run_end_r;
            else
                m_eCurState = STATE::b_run_end_l;
        }
    }
    else
        Soft_Turn_ToInputDir(vInputVector, XM_PI * 5.f);

    if (KEYPUSH(KEY_TYPE::LSHIFT))
    {
        if ((Get_CurFrame() == 1))
            m_eCurState = STATE::b_sprint;

    }

    if (KEYPUSH(KEY_TYPE::LBUTTON))
        m_eCurState = STATE::skill_1100;
    else if (KEYPUSH(KEY_TYPE::KEY_1))
        m_eCurState = STATE::skill_100100;
    else if (KEYPUSH(KEY_TYPE::KEY_2))
        m_eCurState = STATE::skill_200100;
    else if (KEYPUSH(KEY_TYPE::KEY_3))
        m_eCurState = STATE::skill_300100;
    else if (KEYPUSH(KEY_TYPE::KEY_4))
        m_eCurState = STATE::skill_400100;
    else if (KEYPUSH(KEY_TYPE::KEY_5))
        m_eCurState = STATE::skill_501100;
    else if (KEYPUSH(KEY_TYPE::SPACE))
    {
        if (!m_bEvadeCoolCheck)
            m_eCurState = STATE::skill_91100;
    }
}

void Dellons_FSM::b_run_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"b_run", 0.2f, true, 1.f);

    Get_Transform()->Set_Speed(m_fRunSpeed);

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Dellons_FSM::b_run_end_r()
{
    EvadeCoolCheck();

    _float3 vInputVector = Get_InputDirVector();

    if (vInputVector != _float3(0.f))
        Soft_Turn_ToInputDir(vInputVector, XM_PI * 5.f);

    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;

    if (KEYPUSH(KEY_TYPE::LBUTTON))
        m_eCurState = STATE::skill_1100;
    else if (KEYPUSH(KEY_TYPE::KEY_1))
        m_eCurState = STATE::skill_100100;
    else if (KEYPUSH(KEY_TYPE::KEY_2))
        m_eCurState = STATE::skill_200100;
    else if (KEYPUSH(KEY_TYPE::KEY_3))
        m_eCurState = STATE::skill_300100;
    else if (KEYPUSH(KEY_TYPE::KEY_4))
        m_eCurState = STATE::skill_400100;
    else if (KEYPUSH(KEY_TYPE::KEY_5))
        m_eCurState = STATE::skill_501100;
    else if (KEYPUSH(KEY_TYPE::SPACE))
    {
        if (!m_bEvadeCoolCheck)
            m_eCurState = STATE::skill_93100;
    }

}

void Dellons_FSM::b_run_end_r_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"b_run_end_r", 0.1f, false, 1.5f);

    Get_Transform()->Set_Speed(m_fRunSpeed);
    m_tRunEndDelay.fAccTime = 0.f;

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Dellons_FSM::b_run_end_l()
{
    EvadeCoolCheck();

    _float3 vInputVector = Get_InputDirVector();

    if (vInputVector != _float3(0.f))
        Soft_Turn_ToInputDir(vInputVector, XM_PI * 5.f);

    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;

    if (KEYPUSH(KEY_TYPE::LBUTTON))
        m_eCurState = STATE::skill_1100;
    else if (KEYPUSH(KEY_TYPE::KEY_1))
        m_eCurState = STATE::skill_100100;
    else if (KEYPUSH(KEY_TYPE::KEY_2))
        m_eCurState = STATE::skill_200100;
    else if (KEYPUSH(KEY_TYPE::KEY_3))
        m_eCurState = STATE::skill_300100;
    else if (KEYPUSH(KEY_TYPE::KEY_4))
        m_eCurState = STATE::skill_400100;
    else if (KEYPUSH(KEY_TYPE::KEY_5))
        m_eCurState = STATE::skill_501100;
    else if (KEYPUSH(KEY_TYPE::SPACE))
    {
        if (!m_bEvadeCoolCheck)
            m_eCurState = STATE::skill_93100;
    }
}

void Dellons_FSM::b_run_end_l_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"b_run_end_l", 0.1f, false, 1.5f);

    Get_Transform()->Set_Speed(m_fRunSpeed);
    m_tRunEndDelay.fAccTime = 0.f;

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Dellons_FSM::b_sprint()
{
    EvadeCoolCheck();

    Get_Transform()->Go_Straight();

    _float3 vInputVector = Get_InputDirVector();

    if (vInputVector == _float3(0.f))
    {
        m_tRunEndDelay.fAccTime += fDT;

        if (m_tRunEndDelay.fAccTime >= m_tRunEndDelay.fCoolTime)
        {
            if (Get_CurFrame() % 2 == 0)
                m_eCurState = STATE::b_run_end_r;
            else
                m_eCurState = STATE::b_run_end_l;
        }
    }
    else
        Soft_Turn_ToInputDir(vInputVector, XM_PI * 5.f);

    if (!KEYPUSH(KEY_TYPE::LSHIFT))
    {
        if (Get_CurFrame() < 1 || Get_CurFrame() > 13)
            m_eCurState = STATE::b_run;
    }

    if (KEYPUSH(KEY_TYPE::LBUTTON))
        m_eCurState = STATE::skill_1100;
    else if (KEYPUSH(KEY_TYPE::KEY_1))
        m_eCurState = STATE::skill_100100;
    else if (KEYPUSH(KEY_TYPE::KEY_2))
        m_eCurState = STATE::skill_200100;
    else if (KEYPUSH(KEY_TYPE::KEY_3))
        m_eCurState = STATE::skill_300100;
    else if (KEYPUSH(KEY_TYPE::KEY_4))
        m_eCurState = STATE::skill_400100;
    else if (KEYPUSH(KEY_TYPE::KEY_5))
        m_eCurState = STATE::skill_501100;
    else if (KEYPUSH(KEY_TYPE::SPACE))
    {
        if (!m_bEvadeCoolCheck)
            m_eCurState = STATE::skill_91100;
    }
}

void Dellons_FSM::b_sprint_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"b_sprint", 0.2f, true, 1.f);

    Get_Transform()->Set_Speed(m_fSprintSpeed);

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Dellons_FSM::b_walk()
{
}

void Dellons_FSM::b_walk_Init()
{
}

void Dellons_FSM::die()
{
}

void Dellons_FSM::die_Init()
{
}

void Dellons_FSM::airborne_start()
{
    Soft_Turn_ToInputDir(m_vHitDir, XM_PI * 5.f);

    if (Is_AnimFinished())
        m_eCurState = STATE::airborne_end;
}

void Dellons_FSM::airborne_start_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"airborne_start", 0.2f, false, 1.f);

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = true;
}

void Dellons_FSM::airborne_end()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::airborne_up;
}

void Dellons_FSM::airborne_end_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"airborne_end", 0.2f, false, 1.f);

    m_bInvincible = false;
    m_bSuperArmor = true;
}

void Dellons_FSM::airborne_up()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Dellons_FSM::airborne_up_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_Up", 0.1f, false, 1.f);

    m_bInvincible = false;
    m_bSuperArmor = true;
}

void Dellons_FSM::hit()
{
    Soft_Turn_ToInputDir(m_vHitDir, XM_PI * 5.f);

    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Dellons_FSM::hit_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"hit", 0.1f, false, 1.f);

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Dellons_FSM::knock_start()
{
    Soft_Turn_ToInputDir(m_vHitDir, XM_PI * 5.f);

    Get_Transform()->Go_Backward();

    if (Is_AnimFinished())
        m_eCurState = STATE::knock_end;
}

void Dellons_FSM::knock_start_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_start", 0.1f, false, 1.f);

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = true;

    Get_Transform()->Set_Speed(m_fKnockBackSpeed * 0.8f);
}

void Dellons_FSM::knock_end()
{
    if (Get_CurFrame() < 16)
        Get_Transform()->Go_Backward();

    if (Is_AnimFinished())
        m_eCurState = STATE::knock_end_loop;
}

void Dellons_FSM::knock_end_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_end", 0.2f, false, 1.f);

    m_bInvincible = false;
    m_bSuperArmor = true;

    Get_Transform()->Set_Speed(m_fKnockBackSpeed * 0.5f);
}

void Dellons_FSM::knock_end_loop()
{
    m_tKnockDownEndCoolTime.fAccTime += fDT;

    if (Get_CurFrame() > Get_FinalFrame() / 2)
        m_eCurState = STATE::knock_up;
}

void Dellons_FSM::knock_end_loop_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_end_loop", 0.2f, false, 1.f);

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Dellons_FSM::knock_end_hit()
{
    m_tKnockDownEndCoolTime.fAccTime += fDT;

    if (Is_AnimFinished())
    {
        if (m_tKnockDownEndCoolTime.fAccTime >= m_tKnockDownEndCoolTime.fCoolTime)
            m_eCurState = STATE::knock_up;
        else
            m_eCurState = STATE::knock_end_loop;
    }
}

void Dellons_FSM::knock_end_hit_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_end_hit", 0.2f, false, 1.f);

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Dellons_FSM::knock_up()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Dellons_FSM::knock_up_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_Up", 0.2f, false, 1.f);

    m_bInvincible = false;
    m_bSuperArmor = true;

    m_tKnockDownEndCoolTime.fAccTime = 0.f;

    Get_Transform()->Set_Speed(m_fRunSpeed);
}

void Dellons_FSM::knockdown_start()
{
    Soft_Turn_ToInputDir(m_vHitDir, XM_PI * 5.f);

    Get_Transform()->Go_Backward();

    if (Is_AnimFinished())
        m_eCurState = STATE::knockdown_end;
}

void Dellons_FSM::knockdown_start_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knockdown_start", 0.1f, false, 2.f);

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = true;

    Get_Transform()->Set_Speed(m_fKnockDownSpeed);
}

void Dellons_FSM::knockdown_end()
{
    if (Get_CurFrame() < 16)
        Get_Transform()->Go_Backward();

    if (Is_AnimFinished())
        m_eCurState = STATE::knock_up;
}

void Dellons_FSM::knockdown_end_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_CurrentAnim(L"knockdown_end", false, 1.5f);

    m_bInvincible = false;
    m_bSuperArmor = true;

    Get_Transform()->Set_Speed(m_fKnockDownSpeed * 0.5f);
}

void Dellons_FSM::skill_1100()
{
    EvadeCoolCheck();

    if (Get_CurFrame() == 9)
        AttackCollider_On(NORMAL_ATTACK);
    else if (Get_CurFrame() == 19)
        AttackCollider_Off();


    _float3 vInputVector = Get_InputDirVector();

    if (m_vInputTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vInputTurnVector, XM_PI * 5.f);

    if (_float(Get_CurFrame()) / _float(Get_FinalFrame()) >= 0.25f)
        m_bCanCombo = true;

    if (m_bCanCombo)
    {
        if (KEYTAP(KEY_TYPE::LBUTTON))
            m_eCurState = STATE::skill_1200;
    }

    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;

    if (KEYPUSH(KEY_TYPE::KEY_1))
        m_eCurState = STATE::skill_100100;
    else if (KEYPUSH(KEY_TYPE::KEY_2))
        m_eCurState = STATE::skill_200100;
    else if (KEYPUSH(KEY_TYPE::KEY_3))
        m_eCurState = STATE::skill_300100;
    else if (KEYPUSH(KEY_TYPE::KEY_4))
        m_eCurState = STATE::skill_400100;
    else if (KEYPUSH(KEY_TYPE::KEY_5))
        m_eCurState = STATE::skill_501100;
    else if (KEYPUSH(KEY_TYPE::SPACE))
    {
        if (!m_bEvadeCoolCheck)
        {
            if (vInputVector != _float3(0.f))
                m_eCurState = STATE::skill_91100;
            else
                m_eCurState = STATE::skill_93100;
        }
    }
}

void Dellons_FSM::skill_1100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_1100", 0.15f, false, 1.5f);

    m_bCanCombo = false;

    m_vInputTurnVector = _float3(0.f);
    m_vInputTurnVector = Get_InputDirVector();

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Dellons_FSM::skill_1200()
{
    EvadeCoolCheck();

    if (Get_CurFrame() == 8)
        AttackCollider_On(NORMAL_ATTACK);
    else if (Get_CurFrame() == 18)
        AttackCollider_Off();

    _float3 vInputVector = Get_InputDirVector();

    if (m_vInputTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vInputTurnVector, XM_PI * 5.f);


    if (_float(Get_CurFrame()) / _float(Get_FinalFrame()) >= 0.25f)
        m_bCanCombo = true;

    if (m_bCanCombo)
    {
        if (KEYTAP(KEY_TYPE::LBUTTON))
            m_eCurState = STATE::skill_1300;
    }

    if (Is_AnimFinished())
    {
        m_bCanCombo = false;
        m_eCurState = STATE::b_idle;
    }

    if (KEYPUSH(KEY_TYPE::KEY_1))
        m_eCurState = STATE::skill_100100;
    else if (KEYPUSH(KEY_TYPE::KEY_2))
        m_eCurState = STATE::skill_200100;
    else if (KEYPUSH(KEY_TYPE::KEY_3))
        m_eCurState = STATE::skill_300100;
    else if (KEYPUSH(KEY_TYPE::KEY_4))
        m_eCurState = STATE::skill_400100;
    else if (KEYPUSH(KEY_TYPE::KEY_5))
        m_eCurState = STATE::skill_501100;
    else if (KEYPUSH(KEY_TYPE::SPACE))
    {
        if (!m_bEvadeCoolCheck)
        {
            if (vInputVector != _float3(0.f))
                m_eCurState = STATE::skill_91100;
            else
                m_eCurState = STATE::skill_93100;
        }
    }
}

void Dellons_FSM::skill_1200_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_1200", 0.15f, false, 1.5f);

    m_bCanCombo = false;

    m_vInputTurnVector = _float3(0.f);
    m_vInputTurnVector = Get_InputDirVector();

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Dellons_FSM::skill_1300()
{
    EvadeCoolCheck();

    if (Get_CurFrame() == 8)
        AttackCollider_On(NORMAL_ATTACK);
    else if (Get_CurFrame() == 33)
        AttackCollider_Off();

    _float3 vInputVector = Get_InputDirVector();

    if (m_vInputTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vInputTurnVector, XM_PI * 5.f);


    if (_float(Get_CurFrame()) / _float(Get_FinalFrame()) >= 0.25f)
        m_bCanCombo = true;

    if (m_bCanCombo)
    {
        if (KEYTAP(KEY_TYPE::LBUTTON))
            m_eCurState = STATE::skill_1400;
    }

    if (Is_AnimFinished())
    {
        m_bCanCombo = false;
        m_eCurState = STATE::b_idle;
    }

    if (KEYPUSH(KEY_TYPE::KEY_1))
        m_eCurState = STATE::skill_100100;
    else if (KEYPUSH(KEY_TYPE::KEY_2))
        m_eCurState = STATE::skill_200100;
    else if (KEYPUSH(KEY_TYPE::KEY_3))
        m_eCurState = STATE::skill_300100;
    else if (KEYPUSH(KEY_TYPE::KEY_4))
        m_eCurState = STATE::skill_400100;
    else if (KEYPUSH(KEY_TYPE::KEY_5))
        m_eCurState = STATE::skill_501100;
    else if (KEYPUSH(KEY_TYPE::SPACE))
    {
        if (!m_bEvadeCoolCheck)
        {
            if (vInputVector != _float3(0.f))
                m_eCurState = STATE::skill_91100;
            else
                m_eCurState = STATE::skill_93100;
        }
    }
}

void Dellons_FSM::skill_1300_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_1300", 0.15f, false, 1.5f);

    m_bCanCombo = false;

    m_vInputTurnVector = _float3(0.f);
    m_vInputTurnVector = Get_InputDirVector();

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Dellons_FSM::skill_1400()
{
    EvadeCoolCheck();

    if (Get_CurFrame() == 8)
        AttackCollider_On(NORMAL_ATTACK);
    else if (Get_CurFrame() == 14)
        AttackCollider_Off();
    else if (Get_CurFrame() == 16)
        AttackCollider_On(NORMAL_ATTACK);
    else if (Get_CurFrame() == 24)
        AttackCollider_Off();


    _float3 vInputVector = Get_InputDirVector();

    if (m_vInputTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vInputTurnVector, XM_PI * 5.f);

    if (Is_AnimFinished())
    {
        m_bCanCombo = false;
        m_eCurState = STATE::b_idle;
    }

    if (KEYPUSH(KEY_TYPE::KEY_1))
        m_eCurState = STATE::skill_100100;
    else if (KEYPUSH(KEY_TYPE::KEY_2))
        m_eCurState = STATE::skill_200100;
    else if (KEYPUSH(KEY_TYPE::KEY_3))
        m_eCurState = STATE::skill_300100;
    else if (KEYPUSH(KEY_TYPE::KEY_4))
        m_eCurState = STATE::skill_400100;
    else if (KEYPUSH(KEY_TYPE::KEY_5))
        m_eCurState = STATE::skill_501100;
    else if (KEYPUSH(KEY_TYPE::SPACE))
    {
        if (!m_bEvadeCoolCheck)
        {
            if (vInputVector != _float3(0.f))
                m_eCurState = STATE::skill_91100;
            else
                m_eCurState = STATE::skill_93100;
        }
    }
}

void Dellons_FSM::skill_1400_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_1400", 0.15f, false, 1.5f);

    m_bCanCombo = false;

    m_vInputTurnVector = _float3(0.f);
    m_vInputTurnVector = Get_InputDirVector();

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Dellons_FSM::skill_91100()
{
    m_bEvadeCoolCheck = true;

    _float3 vInputVector = Get_InputDirVector();

    if (m_vInputTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vInputTurnVector, XM_PI * 5.f);

    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;

    if (Get_CurFrame() >= 27)
    {
        if (vInputVector != _float3(0.f))
            m_eCurState = STATE::b_run;
    }
}

void Dellons_FSM::skill_91100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_91100", 0.15f, false, m_fEvade_AnimationSpeed);

    m_bCanCombo = false;

    m_vInputTurnVector = _float3(0.f);
    m_vInputTurnVector = Get_InputDirVector();

    AttackCollider_Off();

    m_bInvincible = true;
    m_bSuperArmor = false;
}

void Dellons_FSM::skill_93100()
{
    m_bEvadeCoolCheck = true;

    _float3 vInputVector = Get_InputDirVector();

    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;

    if (Get_CurFrame() >= 18)
    {
        if (vInputVector != _float3(0.f))
            m_eCurState = STATE::b_run;
    }
}

void Dellons_FSM::skill_93100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_93100", 0.15f, false, m_fEvade_AnimationSpeed);

    m_bCanCombo = false;

    AttackCollider_Off();

    m_bInvincible = true;
    m_bSuperArmor = false;
}

void Dellons_FSM::skill_100100()
{
    EvadeCoolCheck();

    if (Get_CurFrame() == 12)
    {
        if (!m_bSkillCreate)
        {
            FORWARDMOVINGSKILLDESC desc;
            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
            desc.fMoveSpeed = 0.f;
            desc.fLifeTime = 0.5f;
            desc.fLimitDistance = 0.f;

            _float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) + 
                                Get_Transform()->Get_State(Transform_State::LOOK) * 2.f +
                                _float3::Up;

            Create_ForwardMovingSkillCollider(vSkillPos, 1.5f, desc, KNOCKBACK_ATTACK);

            m_bSkillCreate = true;
        }
    }
    else
        m_bSkillCreate = false;
    

    _float3 vInputVector = Get_InputDirVector();

    if (m_vInputTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vInputTurnVector, XM_PI * 5.f);

    if (Get_CurFrame() >= 19)
        m_bCanCombo = true;

    if (m_bCanCombo)
    {
        if (KEYTAP(KEY_TYPE::KEY_1))
            m_eCurState = STATE::skill_100200;
    }

    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;

    if (KEYPUSH(KEY_TYPE::SPACE))
    {
        if (!m_bEvadeCoolCheck)
        {
            if (vInputVector != _float3(0.f))
                m_eCurState = STATE::skill_91100;
            else
                m_eCurState = STATE::skill_93100;
        }
    }
}

void Dellons_FSM::skill_100100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_100100", 0.15f, false, m_fSkillAttack_AnimationSpeed);

    m_bCanCombo = false;

    m_vInputTurnVector = _float3(0.f);
    m_vInputTurnVector = Get_InputDirVector();

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = true;
}

void Dellons_FSM::skill_100200()
{
    EvadeCoolCheck();

    if (Get_CurFrame() == 15)
    {
        if (!m_bSkillCreate)
        {
            FORWARDMOVINGSKILLDESC desc;
            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
            desc.fMoveSpeed = 0.f;
            desc.fLifeTime = 0.5f;
            desc.fLimitDistance = 0.f;

            _float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
                Get_Transform()->Get_State(Transform_State::LOOK) * 2.f +
                _float3::Up;

            Create_ForwardMovingSkillCollider(vSkillPos, 1.5f, desc, AIRBORNE_ATTACK);

            m_bSkillCreate = true;
        }
    }
    else
        m_bSkillCreate = false;


    _float3 vInputVector = Get_InputDirVector();

    if (m_vInputTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vInputTurnVector, XM_PI * 5.f);

    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;

    if (KEYPUSH(KEY_TYPE::SPACE))
    {
        if (!m_bEvadeCoolCheck)
        {
            if (vInputVector != _float3(0.f))
                m_eCurState = STATE::skill_91100;
            else
                m_eCurState = STATE::skill_93100;
        }
    }
}

void Dellons_FSM::skill_100200_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_100200", 0.15f, false, m_fSkillAttack_AnimationSpeed);

    m_bCanCombo = false;

    m_vInputTurnVector = _float3(0.f);
    m_vInputTurnVector = Get_InputDirVector();

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = true;
}

void Dellons_FSM::skill_200100()
{
    EvadeCoolCheck();

    if (Get_CurFrame() == 7)
        AttackCollider_On(KNOCKBACK_ATTACK);
    else if (Get_CurFrame() == 12)
        AttackCollider_Off();

    _float3 vInputVector = Get_InputDirVector();

    if (m_vInputTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vInputTurnVector, XM_PI * 5.f);

    if (Get_CurFrame() >= 16)
        m_bCanCombo = true;

    if (m_bCanCombo)
    {
        if (KEYTAP(KEY_TYPE::KEY_2))
            m_eCurState = STATE::skill_200200;
    }

    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;

    if (KEYPUSH(KEY_TYPE::SPACE))
    {
        if (!m_bEvadeCoolCheck)
        {
            if (vInputVector != _float3(0.f))
                m_eCurState = STATE::skill_91100;
            else
                m_eCurState = STATE::skill_93100;
        }
    }
}

void Dellons_FSM::skill_200100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_200100", 0.15f, false, m_fSkillAttack_AnimationSpeed);

    m_bCanCombo = false;

    m_vInputTurnVector = _float3(0.f);
    m_vInputTurnVector = Get_InputDirVector();

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = true;
}

void Dellons_FSM::skill_200200()
{
    EvadeCoolCheck();

    if (Get_CurFrame() == 7)
    {
        if (!m_bSkillCreate)
        {
            FORWARDMOVINGSKILLDESC desc;
            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
            desc.fMoveSpeed = 0.f;
            desc.fLifeTime = 1.f;
            desc.fLimitDistance = 0.f;

            _float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) + Get_Transform()->Get_State(Transform_State::LOOK) * 3.f + _float3::Up;
            Create_ForwardMovingSkillCollider(vSkillPos, 2.f, desc, KNOCKBACK_SKILL);

            m_bSkillCreate = true;
        }
    }
    else
        m_bSkillCreate = false;

    _float3 vInputVector = Get_InputDirVector();

    if (m_vInputTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vInputTurnVector, XM_PI * 5.f);

    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;

    if (KEYPUSH(KEY_TYPE::SPACE))
    {
        if (!m_bEvadeCoolCheck)
        {
            if (vInputVector != _float3(0.f))
                m_eCurState = STATE::skill_91100;
            else
                m_eCurState = STATE::skill_93100;
        }
    }
}

void Dellons_FSM::skill_200200_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_200200", 0.15f, false, m_fSkillAttack_AnimationSpeed);

    m_bCanCombo = false;

    m_vInputTurnVector = _float3(0.f);
    m_vInputTurnVector = Get_InputDirVector();

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = true;
}

void Dellons_FSM::skill_300100()
{
    EvadeCoolCheck();

    if (Get_CurFrame() == 27)
    {
        if (!m_bSkillCreate)
        {
            Summon_Wraith();

            Set_WraithState((_uint)DellonsWraith_FSM::STATE::FX_DellonsWraith_skill_30010);

            m_bSkillCreate = true;
        }
    }
    else
        m_bSkillCreate = false;

    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Dellons_FSM::skill_300100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_300100", 0.15f, false, 1.f);

    m_bCanCombo = false;

    m_vInputTurnVector = _float3(0.f);
    m_vInputTurnVector = Get_InputDirVector();

    AttackCollider_Off();

    m_bInvincible = true;
    m_bSuperArmor = true;


}

void Dellons_FSM::skill_400100()
{
    EvadeCoolCheck();

    if (Get_CurFrame() == 20)
    {
        if (!m_bSkillCreate)
        {
            Summon_Wraith();

            Set_WraithState((_uint)DellonsWraith_FSM::STATE::FX_Mn_Dellons_skill_5100);

            m_bSkillCreate = true;
        }
    }
    else if (Get_CurFrame() == 33 ||
        Get_CurFrame() == 40 ||
        Get_CurFrame() == 47 ||
        Get_CurFrame() == 60 ||
        Get_CurFrame() == 67 ||
        Get_CurFrame() == 72)
    {
        if (!m_bSkillCreate)
        {
            FORWARDMOVINGSKILLDESC desc;
            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
            desc.fMoveSpeed = 20.f;
            desc.fLifeTime = 1.f;
            desc.fLimitDistance = 3.5f;

            _float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) + Get_Transform()->Get_State(Transform_State::LOOK) * 2.f + _float3::Up;
            Create_ForwardMovingSkillCollider(vSkillPos, 1.f, desc, NORMAL_ATTACK);

            m_bSkillCreate = true;
        }
    }
    else if (Get_CurFrame() == 102)
    {
        if (!m_bSkillCreate)
        {
            FORWARDMOVINGSKILLDESC desc;
            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
            desc.fMoveSpeed = 20.f;
            desc.fLifeTime = 1.f;
            desc.fLimitDistance = 2.f;

            _float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) + Get_Transform()->Get_State(Transform_State::LOOK) * 2.f + _float3::Up;
            Create_ForwardMovingSkillCollider(vSkillPos, 1.f, desc, KNOCKDOWN_SKILL);

            m_bSkillCreate = true;
        }
    }
    else
    {
        m_bSkillCreate = false;
    }


    _float3 vInputVector = Get_InputDirVector();

    if (m_vInputTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vInputTurnVector, XM_PI * 5.f);

    if (Get_CurFrame() == 120)
        m_eCurState = STATE::b_idle;

    if (KEYPUSH(KEY_TYPE::SPACE))
    {
        if (!m_bEvadeCoolCheck)
        {
            if (vInputVector != _float3(0.f))
                m_eCurState = STATE::skill_91100;
            else
                m_eCurState = STATE::skill_93100;
        }
    }
}

void Dellons_FSM::skill_400100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_400100", 0.15f, false, m_fSkillAttack_AnimationSpeed);

    m_bCanCombo = false;

    m_vInputTurnVector = _float3(0.f);
    m_vInputTurnVector = Get_InputDirVector();

    AttackCollider_Off();

    m_bInvincible = true;
    m_bSuperArmor = true;
}

void Dellons_FSM::skill_501100()
{
    EvadeCoolCheck();

    if (Get_CurFrame() == 4)
    {
        //Summon Wraith
        if (!m_bSkillCreate)
        {
            Summon_Wraith();

            Set_WraithState((_uint)DellonsWraith_FSM::STATE::FX_Mn_Dellons_skill_500200);
        }
    }
    else
    {
        m_bSkillCreate = false;
    }

    _float3 vInputVector = Get_InputDirVector();

    if (m_vInputTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vInputTurnVector, XM_PI * 5.f);

    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;

    if (KEYPUSH(KEY_TYPE::SPACE))
    {
        if (!m_bEvadeCoolCheck)
        {
            if (vInputVector != _float3(0.f))
                m_eCurState = STATE::skill_91100;
            else
                m_eCurState = STATE::skill_93100;
        }
    }
}

void Dellons_FSM::skill_501100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_501100", 0.15f, false, 1.f);

    m_bCanCombo = false;

    m_vInputTurnVector = _float3(0.f);
    m_vInputTurnVector = Get_InputDirVector();

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = true;
}

void Dellons_FSM::EvadeCoolCheck()
{
    if (m_bEvadeCoolCheck)
    {
        m_tEvadeDelay.fAccTime += fDT;

        if (m_tEvadeDelay.fAccTime >= m_tEvadeDelay.fCoolTime)
        {
            m_bEvadeCoolCheck = false;
            m_tEvadeDelay.fAccTime = 0.f;
        }
    }
}

void Dellons_FSM::Create_ForwardMovingSkillCollider(const _float4& vPos, _float fSkillRange, FORWARDMOVINGSKILLDESC desc, const wstring& SkillType)
{
    shared_ptr<GameObject> SkillCollider = make_shared<GameObject>();

    m_pSkillCollider = SkillCollider;

    m_pSkillCollider.lock()->GetOrAddTransform();
    m_pSkillCollider.lock()->Get_Transform()->Set_State(Transform_State::POS, vPos);
    m_pSkillCollider.lock()->Add_Component(make_shared<SphereCollider>(fSkillRange));
    m_pSkillCollider.lock()->Get_Collider()->Set_CollisionGroup(Player_Skill);

    m_pSkillCollider.lock()->Add_Component(make_shared<AttackColliderInfoScript>());
    m_pSkillCollider.lock()->Get_Collider()->Set_Activate(true);
    m_pSkillCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_SkillName(SkillType);
    m_pSkillCollider.lock()->Set_Name(L"Player_SkillCollider");
    m_pSkillCollider.lock()->Add_Component(make_shared<ForwardMovingSkillScript>(desc));
    m_pSkillCollider.lock()->Get_Script<ForwardMovingSkillScript>()->Init();

    CUR_SCENE->Add_GameObject(m_pSkillCollider.lock());
}

void Dellons_FSM::Summon_Wraith()
{
    //Add. Player's Wraith
    shared_ptr<GameObject> ObjWraith = make_shared<GameObject>();

    ObjWraith->Add_Component(make_shared<Transform>());
    {
        shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Model.fx");

        shared_ptr<ModelAnimator> renderer = make_shared<ModelAnimator>(shader);
        {
            shared_ptr<Model> model = RESOURCES.Get<Model>(L"Dellons_Wraith");
            renderer->Set_Model(model);
        }

        ObjWraith->Add_Component(renderer);

    }
    ObjWraith->Add_Component(make_shared<DellonsWraith_FSM>());
    ObjWraith->Get_FSM()->Set_Target(m_pOwner.lock());
    ObjWraith->Get_FSM()->Init();
    ObjWraith->Set_Name(L"Dellons_Wraith");

    CUR_SCENE->Add_GameObject(ObjWraith);

    m_pDellonsWraith = ObjWraith;
}

void Dellons_FSM::Set_WraithState(_uint iAnimindex)
{
    if (!m_pDellonsWraith.expired())
        m_pDellonsWraith.lock()->Get_FSM()->Set_State(iAnimindex);
}
