#include "pch.h"
#include "Undead_Priest_FSM.h"
#include "ModelAnimator.h"
#include "SphereCollider.h"
#include "AttackColliderInfoScript.h"
#include "MainCameraScript.h"
#include "UiDamageCreate.h"

HRESULT Undead_Priest_FSM::Init()
{
    if (!m_bInitialize)
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
        attackCollider->Get_Collider()->Set_CollisionGroup(Monster_Attack);

        m_pAttackCollider = attackCollider;

        CUR_SCENE->Add_GameObject(m_pAttackCollider.lock());
        m_pAttackCollider.lock()->Get_Collider()->Set_Activate(false);

        m_pAttackCollider.lock()->Add_Component(make_shared<AttackColliderInfoScript>());
        m_pAttackCollider.lock()->Set_Name(L"Succubus_Whip_AttackCollider");
        m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_ColliderOwner(m_pOwner.lock());

        m_pCamera = CUR_SCENE->Get_MainCamera();



        m_fRunSpeed = 4.f;
        m_fSprintSpeed = 5.5f;
        m_fKnockBackSpeed = 4.f;
        m_fKnockDownSpeed = 4.f;
        m_fAttackRange = 7.f;

        m_fNormalAttack_AnimationSpeed = 1.3f;
        m_fSkillAttack_AnimationSpeed = 1.3f;

        m_fDetectRange = 10.f;


        m_bInitialize = true;
    }

    return S_OK;
}

void Undead_Priest_FSM::Tick()
{
    State_Tick();

    if (!m_pAttackCollider.expired())
    {
        //m_pAttack transform set forward
        m_pAttackCollider.lock()->Get_Transform()->Set_State(Transform_State::POS, Get_Transform()->Get_State(Transform_State::POS) + Get_Transform()->Get_State(Transform_State::LOOK) * 2.f + _float3::Up);
    }
}

void Undead_Priest_FSM::State_Tick()
{
    State_Init();

    m_iCurFrame = Get_CurFrame();

    switch (m_eCurState)
    {
    case STATE::b_idle:
        b_idle();
        break;
    case STATE::b_run:
        b_run();
        break;
    case STATE::n_run:
        n_run();
        break;
    case STATE::wander:
        wander();
        break;
    case STATE::die:
        die();
        break;
    case STATE::gaze_b:
        gaze_b();
        break;
    case STATE::gaze_bl:
        gaze_bl();
        break;
    case STATE::gaze_br:
        gaze_br();
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
    case STATE::skill_2100:
        skill_2100();
        break;
    case STATE::skill_3100:
        skill_3100();
        break;
    }

    if (!m_pGroupEffect.expired())
        m_pGroupEffect.lock()->Get_Transform()->Set_WorldMat(Get_Transform()->Get_WorldMatrix());

    if (m_iPreFrame != m_iCurFrame)
        m_iPreFrame = m_iCurFrame;
}

void Undead_Priest_FSM::State_Init()
{
    if (m_eCurState != m_ePreState)
    {
        switch (m_eCurState)
        {
        case STATE::b_idle:
            b_idle_Init();
            break;
        case STATE::b_run:
            b_run_Init();
            break;
        case STATE::n_run:
            n_run_Init();
            break;
        case STATE::wander:
            wander_Init();
            break;
        case STATE::die:
            die_Init();
            break;
        case STATE::gaze_b:
            gaze_b_Init();
            break;
        case STATE::gaze_bl:
            gaze_bl_Init();
            break;
        case STATE::gaze_br:
            gaze_br_Init();
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
        case STATE::skill_2100:
            skill_2100_Init();
            break;
        case STATE::skill_3100:
            skill_3100_Init();
            break;
        }
        m_ePreState = m_eCurState;
    }
}

void Undead_Priest_FSM::OnCollision(shared_ptr<BaseCollider> pCollider, _float fGap)
{
}

void Undead_Priest_FSM::OnCollisionEnter(shared_ptr<BaseCollider> pCollider, _float fGap)
{
    if (pCollider->Get_Owner() == nullptr)
        return;

    if (!pCollider->Get_Owner()->Get_Script<AttackColliderInfoScript>())
        return;

    wstring strSkillName = pCollider->Get_Owner()->Get_Script<AttackColliderInfoScript>()->Get_SkillName();

    if (!m_bInvincible)
    {
        shared_ptr<GameObject> targetToLook = nullptr;

        if (strSkillName.find(L"_Skill") != wstring::npos)
            targetToLook = pCollider->Get_Owner();
        else
            targetToLook = pCollider->Get_Owner()->Get_Script<AttackColliderInfoScript>()->Get_ColliderOwner();

        if (targetToLook == nullptr)
            return;

        Get_Hit(strSkillName, targetToLook);
    }
}

void Undead_Priest_FSM::OnCollisionExit(shared_ptr<BaseCollider> pCollider, _float fGap)
{
}

void Undead_Priest_FSM::Get_Hit(const wstring& skillname, shared_ptr<GameObject> pLookTarget)
{
    CUR_SCENE->Get_UI(L"UI_Damage_Controller")->Get_Script<UiDamageCreate>()->Create_Damage_Font(Get_Owner());

    m_bDetected = true;
    m_pCamera.lock()->Get_Script<MainCameraScript>()->ShakeCamera(0.1f, 0.05f);
    _float3 vMyPos = Get_Transform()->Get_State(Transform_State::POS).xyz();
    _float3 vOppositePos = pLookTarget->Get_Transform()->Get_State(Transform_State::POS).xyz();

    m_vHitDir = vOppositePos - vMyPos;
    m_vHitDir.y = 0.f;
    m_vHitDir.Normalize();

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

void Undead_Priest_FSM::AttackCollider_On(const wstring& skillname)
{
    if (!m_pAttackCollider.expired())
    {
        m_pAttackCollider.lock()->Get_Collider()->Set_Activate(true);
        m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_SkillName(skillname);
    }
}

void Undead_Priest_FSM::AttackCollider_Off()
{
    if (!m_pAttackCollider.expired())
    {
        m_pAttackCollider.lock()->Get_Collider()->Set_Activate(false);
        m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_SkillName(L"");
    }
}

void Undead_Priest_FSM::Set_State(_uint iIndex)
{
}

void Undead_Priest_FSM::b_idle()
{
    if (!m_pTarget.expired())
        Soft_Turn_ToTarget(m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS), m_fTurnSpeed);

    m_tAttackCoolTime.fAccTime += fDT;

    if (!m_bDetected)
    {
        CalCulate_PatrolTime();

        if (m_bPatrolMove)
        {
            m_eCurState = STATE::n_run;
        }
    }
    else
    {
        if (!m_bSetPattern)
        {
            Set_AttackSkill();
        }
        else
        {
            if (Target_In_AttackRange())
                m_eCurState = m_ePatternState;
            else
                m_eCurState = STATE::b_run;
        }
    }
}

void Undead_Priest_FSM::b_idle_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"b_idle", 0.1f, true, 1.f);

    Get_Transform()->Set_Speed(m_fRunSpeed);

    m_tPatrolMoveCool.fAccTime = 0.f;
    m_vTurnVector = _float3(0.f);

    m_bSuperArmor = false;
}

void Undead_Priest_FSM::b_run()
{
    if (!m_pTarget.expired())
        Soft_Turn_ToTarget(m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS), XM_PI * 5.f);

    Get_Transform()->Go_Straight();

    if (Target_In_AttackRange())
        m_eCurState = m_ePatternState;

}

void Undead_Priest_FSM::b_run_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"b_run", 0.2f, true, 1.f);

    Get_Transform()->Set_Speed(m_fSprintSpeed);

    m_bSuperArmor = false;
}

void Undead_Priest_FSM::n_run()
{
    if (m_vTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vTurnVector, XM_PI * 5.f);

    _float4 vPrePos = Get_Transform()->Get_State(Transform_State::POS);

    if (Get_Transform()->Go_Straight())
    {
        m_vTurnVector = (Get_Transform()->Get_State(Transform_State::LOOK) * -1.f).xyz();
        Get_Transform()->Set_LookDir(m_vTurnVector);
    }

    m_fPatrolDistanceCnt += (Get_Transform()->Get_State(Transform_State::POS) - vPrePos).Length();

    if (m_fPatrolDistanceCnt >= m_fPatrolDistance)
    {
        m_fPatrolDistanceCnt = 0.f;
        m_bPatrolMove = false;
        m_eCurState = STATE::b_idle;
    }

    if (Target_In_DetectRange())
        m_bDetected = true;

    if (m_bDetected)
    {
        Set_AttackSkill();
        m_eCurState = STATE::b_run;
    }
}

void Undead_Priest_FSM::n_run_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"n_run", 0.2f, true, 1.f);

    Get_Transform()->Set_Speed(m_fRunSpeed / 2.f);

    m_vTurnVector = _float3{ (rand() * 2 / _float(RAND_MAX) - 1), 0.f, (rand() * 2 / _float(RAND_MAX) - 1) };
    m_vTurnVector.Normalize();

    m_bSuperArmor = false;
}

void Undead_Priest_FSM::wander()
{
    if (!m_bDetected)
    {
        CalCulate_PatrolTime();

        if (m_bPatrolMove)
            m_eCurState = STATE::n_run;

        if (Target_In_DetectRange())
            m_bDetected = true;
    }
    else
    {
        Entry_Battle();
    }
}

void Undead_Priest_FSM::wander_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"wander", 0.2f, true, 1.f);

    m_vTurnVector = _float3{ (rand() * 2 / _float(RAND_MAX) - 1), 0.f, (rand() * 2 / _float(RAND_MAX) - 1) };
    m_vTurnVector.Normalize();

    m_bSuperArmor = false;
}

void Undead_Priest_FSM::die()
{
}

void Undead_Priest_FSM::die_Init()
{
}

void Undead_Priest_FSM::gaze_b()
{
    if (!m_pTarget.expired())
        Soft_Turn_ToTarget(m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS), XM_PI * 5.f);

    m_tAttackCoolTime.fAccTime += fDT;

    Get_Transform()->Go_Backward();

    if (!m_bSetPattern)
    {
        if (m_tAttackCoolTime.fAccTime >= m_tAttackCoolTime.fCoolTime)
            Set_AttackSkill();
    }
    else
    {
        _float fGap;
        
        if (Target_In_AttackRange(&fGap))
        {
            m_eCurState = m_ePatternState;

            if (fGap <= 3.f)
                m_eCurState = STATE::skill_2100;
        }
        else
            m_eCurState = STATE::b_run;
    }
}

void Undead_Priest_FSM::gaze_b_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"gaze_b", 0.2f, true, 1.f);

    Get_Transform()->Set_Speed(m_fRunSpeed / 2.f);

    m_bSuperArmor = false;
}

void Undead_Priest_FSM::gaze_bl()
{
    if (!m_pTarget.expired())
        Soft_Turn_ToTarget(m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS), XM_PI * 5.f);

    m_tAttackCoolTime.fAccTime += fDT;

    _float3 vDir = (Get_Transform()->Get_State(Transform_State::LOOK) * -1.f + Get_Transform()->Get_State(Transform_State::RIGHT)).xyz();
    vDir.Normalize();

    Get_Transform()->Go_Dir(vDir * fDT);

    if (!m_bSetPattern)
    {
        if (m_tAttackCoolTime.fAccTime >= m_tAttackCoolTime.fCoolTime)
            Set_AttackSkill();
    }
    else
    {
        _float fGap;

        if (Target_In_AttackRange(&fGap))
        {
            m_eCurState = m_ePatternState;

            if (fGap <= 3.f)
                m_eCurState = STATE::skill_2100;
        }
        else
            m_eCurState = STATE::b_run;
    }
}

void Undead_Priest_FSM::gaze_bl_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"gaze_bl", 0.2f, true, 1.f);

    Get_Transform()->Set_Speed(m_fRunSpeed / 2.f);

    m_bSuperArmor = false;
}

void Undead_Priest_FSM::gaze_br()
{
    if (!m_pTarget.expired())
        Soft_Turn_ToTarget(m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS), XM_PI * 5.f);

    m_tAttackCoolTime.fAccTime += fDT;

    _float3 vDir = (Get_Transform()->Get_State(Transform_State::LOOK) * -1.f + Get_Transform()->Get_State(Transform_State::RIGHT) * -1.f).xyz();
    vDir.Normalize();

    Get_Transform()->Go_Dir(vDir * fDT);

    if (!m_bSetPattern)
    {
        if (m_tAttackCoolTime.fAccTime >= m_tAttackCoolTime.fCoolTime)
            Set_AttackSkill();
    }
    else
    {
        _float fGap;

        if (Target_In_AttackRange(&fGap))
        {
            m_eCurState = m_ePatternState;

            if (fGap <= 3.f)
                m_eCurState = STATE::skill_2100;
        }
        else
            m_eCurState = STATE::b_run;
    }
}

void Undead_Priest_FSM::gaze_br_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"gaze_br", 0.2f, true, 1.f);

    Get_Transform()->Set_Speed(m_fRunSpeed / 2.f);

    m_bSuperArmor = false;
}

void Undead_Priest_FSM::airborne_start()
{
    Soft_Turn_ToInputDir(m_vHitDir, XM_PI * 10.f);

    if (Is_AnimFinished())
        m_eCurState = STATE::airborne_end;
}

void Undead_Priest_FSM::airborne_start_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"airborne_start", 0.2f, false, 1.f);

    AttackCollider_Off();

    m_bSuperArmor = true;
}

void Undead_Priest_FSM::airborne_end()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::airborne_up;
}

void Undead_Priest_FSM::airborne_end_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"airborne_end", 0.2f, false, 1.f);

    m_bSuperArmor = true;
}

void Undead_Priest_FSM::airborne_up()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Undead_Priest_FSM::airborne_up_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_Up", 0.1f, false, 1.f);

    m_bSuperArmor = true;
}

void Undead_Priest_FSM::hit()
{
    Soft_Turn_ToInputDir(m_vHitDir, XM_PI * 10.f);

    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Undead_Priest_FSM::hit_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"hit", 0.1f, false, 1.f);

    AttackCollider_Off();
}

void Undead_Priest_FSM::knock_start()
{
    Soft_Turn_ToInputDir(m_vHitDir, XM_PI * 10.f);

    Get_Transform()->Go_Backward();

    if (Is_AnimFinished())
        m_eCurState = STATE::knock_end;
}

void Undead_Priest_FSM::knock_start_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_start", 0.1f, false, 1.f);

    AttackCollider_Off();

    m_bSuperArmor = true;

    Get_Transform()->Set_Speed(m_fRunSpeed * 0.5f);
}

void Undead_Priest_FSM::knock_end()
{
    if (Get_CurFrame() < 13)
        Get_Transform()->Go_Backward();

    if (Is_AnimFinished())
        m_eCurState = STATE::knock_end_loop;
}

void Undead_Priest_FSM::knock_end_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_end", 0.2f, false, 1.f);

    m_bSuperArmor = true;
}

void Undead_Priest_FSM::knock_end_loop()
{
    if (Get_CurFrame() > Get_FinalFrame() / 2)
        m_eCurState = STATE::knock_up;
}

void Undead_Priest_FSM::knock_end_loop_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_end_loop", 0.2f, false, 1.f);

    m_bSuperArmor = false;
}

void Undead_Priest_FSM::knock_end_hit()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::knock_end_loop;
}

void Undead_Priest_FSM::knock_end_hit_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_end_hit", 0.2f, false, 1.f);

    m_bSuperArmor = false;
}

void Undead_Priest_FSM::knock_up()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Undead_Priest_FSM::knock_up_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_Up", 0.2f, false, 1.f);

    m_bSuperArmor = true;

    Get_Transform()->Set_Speed(m_fRunSpeed);
}

void Undead_Priest_FSM::knockdown_start()
{
    Soft_Turn_ToInputDir(m_vHitDir, XM_PI * 5.f);

    Get_Transform()->Go_Backward();

    if (Is_AnimFinished())
        m_eCurState = STATE::knockdown_end;
}

void Undead_Priest_FSM::knockdown_start_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knockdown_start", 0.2f, false, 2.f);

    AttackCollider_Off();

    m_bSuperArmor = true;

    Get_Transform()->Set_Speed(m_fKnockDownSpeed);
}

void Undead_Priest_FSM::knockdown_end()
{
    if (Get_CurFrame() < 16)
        Get_Transform()->Go_Backward();

    if (Is_AnimFinished())
        m_eCurState = STATE::knock_up;
}

void Undead_Priest_FSM::knockdown_end_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knockdown_end", 0.2f, false, 1.5f);

    m_bSuperArmor = true;

    Get_Transform()->Set_Speed(m_fKnockDownSpeed * 0.5f);
}

void Undead_Priest_FSM::skill_1100()
{
    if (m_vTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vTurnVector, m_fTurnSpeed);

    if (Init_CurFrame(40))
    {
        FORWARDMOVINGSKILLDESC desc;
        desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
        desc.fMoveSpeed = 20.f;
        desc.fLifeTime = 1.f;
        desc.fLimitDistance = 20.f;

        _float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) + Get_Transform()->Get_State(Transform_State::LOOK) * 2.f + _float3::Up;
        Create_ForwardMovingSkillCollider(vSkillPos, 1.f, desc, NONE_HIT);
    }
    
    Set_Gaze();
}

void Undead_Priest_FSM::skill_1100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_1100", 0.15f, false, m_fNormalAttack_AnimationSpeed);

    m_tAttackCoolTime.fAccTime = 0.f;

    m_vTurnVector = Calculate_TargetTurnVector();

    m_bSuperArmor = false;
    m_bSetPattern = false;
}

void Undead_Priest_FSM::skill_2100()
{
    if (m_vTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vTurnVector, m_fTurnSpeed);

    if (Init_CurFrame(30))
        AttackCollider_On(NORMAL_ATTACK);
    else if (Init_CurFrame(34))
        AttackCollider_Off();
    else if (Init_CurFrame(66))
    {
        FORWARDMOVINGSKILLDESC desc;
        desc.vSkillDir = -Get_Transform()->Get_State(Transform_State::UP);
        desc.fMoveSpeed = 15.f;
        desc.fLifeTime = 0.2f;
        desc.fLimitDistance = 3.f;

        _float4 vSkillPos = _float4(0.f);

        if (!m_pTarget.expired())
            vSkillPos = m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS) + _float3::Up * 5.f;
     
        Create_ForwardMovingSkillCollider(vSkillPos, 1.f, desc, NORMAL_ATTACK);
    }

    Set_Gaze();
}

void Undead_Priest_FSM::skill_2100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_2100", 0.15f, false, m_fNormalAttack_AnimationSpeed);

    m_tAttackCoolTime.fAccTime = 0.f;

    m_vTurnVector = Calculate_TargetTurnVector();

    m_bSuperArmor = false;
    m_bSetPattern = false;
}

void Undead_Priest_FSM::skill_3100()
{
    if (m_vTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vTurnVector, m_fTurnSpeed);

    if (Init_CurFrame(73))
    {
        FORWARDMOVINGSKILLDESC desc;
        desc.vSkillDir = -Get_Transform()->Get_State(Transform_State::UP);
        desc.fMoveSpeed = 15.f;
        desc.fLifeTime = 0.2f;
        desc.fLimitDistance = 3.f;

        _float4 vSkillPos = _float4(0.f);

        if (!m_pTarget.expired())
            vSkillPos = m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS) + _float3::Up * 5.f;

        Create_ForwardMovingSkillCollider(vSkillPos, 1.f, desc, NONE_HIT);
    }
    
    Set_Gaze();
}

void Undead_Priest_FSM::skill_3100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_3100", 0.15f, false, m_fNormalAttack_AnimationSpeed);

    m_tAttackCoolTime.fAccTime = 0.f;

    m_vTurnVector = Calculate_TargetTurnVector();

    m_bSuperArmor = false;
    m_bSetPattern = false;
}


void Undead_Priest_FSM::CalCulate_PatrolTime()
{
    m_tPatrolMoveCool.fAccTime += fDT;

    if (m_tPatrolMoveCool.fAccTime >= m_tPatrolMoveCool.fCoolTime)
    {
        m_tPatrolMoveCool.fAccTime = 0.f;
        m_tPatrolMoveCool.fCoolTime = _float(rand() % 3) + 2.f;
        m_fPatrolDistance = _float(rand() % 4) + 6.f;
        m_vPatrolFirstPos = Get_Transform()->Get_State(Transform_State::POS);

        m_bPatrolMove = true;
    }
}



void Undead_Priest_FSM::Set_Gaze()
{
    if (Is_AnimFinished())
    {
        _uint iRan = rand() % 3;


        if (iRan == 0)
            m_eCurState = STATE::gaze_b;
        else if (iRan == 1)
            m_eCurState = STATE::gaze_bl;
        else if (iRan == 2)
            m_eCurState = STATE::gaze_br;
    }
}

void Undead_Priest_FSM::Entry_Battle()
{
    _uint iRan = rand() % 3;

    if (iRan == 0)
        m_eCurState = STATE::gaze_b;
    else if (iRan == 1)
        m_eCurState = STATE::gaze_bl;
    else if (iRan == 2)
        m_eCurState = STATE::gaze_br;

}

void Undead_Priest_FSM::Set_AttackSkill()
{
    _uint iRan = rand() % 2;



    while (true)
    {
        if (iRan == m_iPreAttack)
            iRan = rand() % 2;
        else
            break;
    }

    if (iRan == 0)
    {
        m_fAttackRange = 8.f;
        m_ePatternState = STATE::skill_1100;
        m_iPreAttack = 0;
    }
    else if (iRan == 1)
    {
        m_fAttackRange = 8.f;
        m_ePatternState = STATE::skill_3100;
        m_iPreAttack = 1;
    }

    m_bSetPattern = true;
}

_float3 Undead_Priest_FSM::Calculate_TargetTurnVector()
{
    if (m_pTarget.expired())
        return _float3(0.f);
    else
        return m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS).xyz() - m_pOwner.lock()->Get_Transform()->Get_State(Transform_State::POS).xyz();
}

void Undead_Priest_FSM::Create_ForwardMovingSkillCollider(const _float4& vPos, _float fSkillRange, FORWARDMOVINGSKILLDESC desc, const wstring& SkillType)
{
    shared_ptr<GameObject> SkillCollider = make_shared<GameObject>();

    m_pSkillCollider = SkillCollider;

    m_pSkillCollider.lock()->GetOrAddTransform();
    m_pSkillCollider.lock()->Get_Transform()->Set_State(Transform_State::POS, vPos);

    auto pSphereCollider = make_shared<SphereCollider>(fSkillRange);
    pSphereCollider->Set_CenterPos(_float3{ vPos.x,vPos.y, vPos.z });
    m_pSkillCollider.lock()->Add_Component(pSphereCollider);

    m_pSkillCollider.lock()->Get_Collider()->Set_CollisionGroup(Monster_Skill);

    m_pSkillCollider.lock()->Add_Component(make_shared<AttackColliderInfoScript>());
    m_pSkillCollider.lock()->Get_Collider()->Set_Activate(true);
    m_pSkillCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_SkillName(SkillType);
    m_pSkillCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_ColliderOwner(m_pOwner.lock());
    m_pSkillCollider.lock()->Set_Name(L"Undead_Priest_SkillCollider");
    m_pSkillCollider.lock()->Add_Component(make_shared<ForwardMovingSkillScript>(desc));
    m_pSkillCollider.lock()->Get_Script<ForwardMovingSkillScript>()->Init();

    CUR_SCENE->Add_GameObject(m_pSkillCollider.lock());
}

void Undead_Priest_FSM::Create_InstallationSkillCollider(const _float4& vPos, _float fSkillRange, INSTALLATIONSKILLDESC desc)
{
    shared_ptr<GameObject> InstallationSkillCollider = make_shared<GameObject>();

    InstallationSkillCollider->GetOrAddTransform();
    InstallationSkillCollider->Get_Transform()->Set_State(Transform_State::POS, vPos);

    auto pSphereCollider = make_shared<SphereCollider>(fSkillRange);
    pSphereCollider->Set_CenterPos(_float3{ vPos.x,vPos.y, vPos.z });
    InstallationSkillCollider->Add_Component(pSphereCollider);
    InstallationSkillCollider->Get_Collider()->Set_CollisionGroup(Monster_Skill);

    InstallationSkillCollider->Add_Component(make_shared<AttackColliderInfoScript>());
    InstallationSkillCollider->Get_Script<AttackColliderInfoScript>()->Set_ColliderOwner(m_pOwner.lock());
    InstallationSkillCollider->Add_Component(make_shared<InstallationSkill_Script>(desc));
    InstallationSkillCollider->Get_Script<InstallationSkill_Script>()->Init();

    InstallationSkillCollider->Set_Name(L"Undead_Priest_InstallationSkillCollider");

    CUR_SCENE->Add_GameObject(InstallationSkillCollider);
}
