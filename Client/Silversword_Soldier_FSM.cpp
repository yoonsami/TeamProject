#include "pch.h"
#include "Silversword_Soldier_FSM.h"
#include "ModelAnimator.h"
#include "SphereCollider.h"
#include "AttackColliderInfoScript.h"

HRESULT Silversword_Soldier_FSM::Init()
{
    auto animator = Get_Owner()->Get_Animator();
    if (animator)
    {
        // 다음 애니메이션 세팅해주는데, 보간할 예정
        animator->Set_CurrentAnim(L"b_idle"/*애니메이션 이름*/, true/*반복 애니메이션*/, 1.f/*애니메이션 속도*/);
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
    m_pAttackCollider.lock()->Set_Name(L"SilverSword_Soldier_AttackCollider");

    return S_OK;
}

void Silversword_Soldier_FSM::Tick()
{
    State_Tick();

    if (!m_pAttackCollider.expired())
    {
        //m_pAttack transform set forward
        m_pAttackCollider.lock()->Get_Transform()->Set_State(Transform_State::POS, Get_Transform()->Get_State(Transform_State::POS) + Get_Transform()->Get_State(Transform_State::LOOK) * 2.f + _float3::Up);
    }
}

void Silversword_Soldier_FSM::State_Tick()
{
    State_Init();

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
    case STATE::die:
        die();
        break;
    case STATE::gaze_b:
        gaze_b();
        break;
    case STATE::gaze_f:
        gaze_f();
        break;
    case STATE::gaze_l:
        gaze_l();
        break;
    case STATE::gaze_r:
        gaze_r();
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
}

void Silversword_Soldier_FSM::State_Init()
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
        case STATE::die:
            die_Init();
            break;
        case STATE::gaze_b:
            gaze_b_Init();
            break;
        case STATE::gaze_f:
            gaze_f_Init();
            break;
        case STATE::gaze_l:
            gaze_l_Init();
            break;
        case STATE::gaze_r:
            gaze_r_Init();
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

void Silversword_Soldier_FSM::OnCollision(shared_ptr<BaseCollider> pCollider, _float fGap)
{
}

void Silversword_Soldier_FSM::OnCollisionEnter(shared_ptr<BaseCollider> pCollider, _float fGap)
{
    wstring strSkillName = pCollider->Get_Owner()->Get_Script<AttackColliderInfoScript>()->Get_SkillName();

    if (!m_bInvincible)
    {
        m_vHitDir = pCollider->Get_CenterPos() - Get_Transform()->Get_State(Transform_State::POS);
        m_vHitDir.y = 0.f;
        m_vHitDir.Normalize();

        Get_Hit(strSkillName, pCollider);
    }
}

void Silversword_Soldier_FSM::OnCollisionExit(shared_ptr<BaseCollider> pCollider, _float fGap)
{
}

void Silversword_Soldier_FSM::Get_Hit(const wstring& skillname, shared_ptr<BaseCollider> pOppositeCollider)
{
    m_bDetected = true;

    if (skillname == NORMAL_SKILL)
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
    else if (skillname == KNOCKDOWN_SKILL)
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
    else if (skillname == AIRBORNE_SKILL)
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

void Silversword_Soldier_FSM::AttackCollider_On(const wstring& skillname)
{
    if (!m_pAttackCollider.expired())
    {
        m_pAttackCollider.lock()->Get_Collider()->Set_Activate(true);
        m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_SkillName(skillname);
    }
}

void Silversword_Soldier_FSM::AttackCollider_Off()
{
    if (!m_pAttackCollider.expired())
    {
        m_pAttackCollider.lock()->Get_Collider()->Set_Activate(false);
        m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_SkillName(L"");
    }
}

void Silversword_Soldier_FSM::b_idle()
{
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
        if (Target_In_AttackRange())
        {
            _uint iRan = rand() % 3;

            if (iRan == 0)
                m_eCurState = STATE::skill_1100;
            else if (iRan == 1)
                m_eCurState = STATE::skill_2100;
            else if (iRan == 2)
                m_eCurState = STATE::skill_3100;
        }
        else
            m_eCurState = STATE::b_run;
    }
}

void Silversword_Soldier_FSM::b_idle_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"b_idle", 0.1f, true, 1.f);

    Get_Transform()->Set_Speed(m_fRunSpeed);

    m_tPatrolMoveCool.fAccTime = 0.f;
    m_vTurnVector = _float3(0.f);

    m_bSuperArmor = false;
}

void Silversword_Soldier_FSM::b_run()
{
    if (!m_pTarget.expired())
        Soft_Turn_ToTarget(m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS), XM_PI * 5.f);


    Get_Transform()->Go_Straight();

    if (Target_In_AttackRange())
    {
        _uint iRan = rand() % 3;

        if (iRan == 0)
            m_eCurState = STATE::skill_1100;
        else if (iRan == 1)
            m_eCurState = STATE::skill_2100;
        else if (iRan == 2)
            m_eCurState = STATE::skill_3100;
    }
}

void Silversword_Soldier_FSM::b_run_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"b_run", 0.2f, true, 1.f);

    Get_Transform()->Set_Speed(m_fRunSpeed);

    m_bSuperArmor = false;
}

void Silversword_Soldier_FSM::n_run()
{
    if (m_vTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vTurnVector, XM_PI * 5.f);

    Get_Transform()->Go_Straight();
    
    Ray ray;
    ray.position = Get_Transform()->Get_State(Transform_State::POS).xyz();
    ray.direction = m_vTurnVector;
    physx::PxRaycastBuffer hit{};
    physx::PxQueryFilterData filterData;
    filterData.flags = physx::PxQueryFlag::eSTATIC;
    if (PHYSX.Get_PxScene()->raycast({ ray.position.x,ray.position.y,ray.position.z }, { ray.direction.x,ray.direction.y,ray.direction.z }, 3.f, hit, PxHitFlags(physx::PxHitFlag::eDEFAULT), filterData))
    {
        m_vTurnVector.x = m_vTurnVector.x * -1.f;
        m_vTurnVector.z = m_vTurnVector.z * -1.f;
    }

    if ((Get_Transform()->Get_State(Transform_State::POS) - m_vPatrolFirstPos).Length() >= m_fPatrolDistance)
    {
        m_bPatrolMove = false;
        m_eCurState = STATE::b_idle;
    }
}

void Silversword_Soldier_FSM::n_run_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"n_run", 0.2f, true, 1.f);

    Get_Transform()->Set_Speed(m_fRunSpeed / 2.f);

    m_vTurnVector = _float3{ (rand() * 2 / _float(RAND_MAX) - 1), 0.f, (rand() * 2 / _float(RAND_MAX) - 1) };
    m_vTurnVector.Normalize();

    m_bSuperArmor = false;
}

void Silversword_Soldier_FSM::die()
{
}

void Silversword_Soldier_FSM::die_Init()
{
}

void Silversword_Soldier_FSM::gaze_b()
{
    if (!m_pTarget.expired())
        Soft_Turn_ToTarget(m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS), XM_PI * 5.f);
    
    m_tAttackCoolTime.fAccTime += fDT;

    Get_Transform()->Go_Backward();

    if (m_tAttackCoolTime.fAccTime >= m_tAttackCoolTime.fCoolTime)
    {
        if (Target_In_AttackRange())
        {
            _uint iRan = rand() % 3;

            if (iRan == 0)
                m_eCurState = STATE::skill_1100;
            else if (iRan == 1)
                m_eCurState = STATE::skill_2100;
            else if (iRan == 2)
                m_eCurState = STATE::skill_3100;
        }
        else
            m_eCurState = STATE::b_run;
    }
}

void Silversword_Soldier_FSM::gaze_b_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"gaze_b", 0.2f, true, 1.f);

    Get_Transform()->Set_Speed(m_fRunSpeed / 2.f);

    m_bSuperArmor = false;
}

void Silversword_Soldier_FSM::gaze_f()
{
    if (!m_pTarget.expired())
        Soft_Turn_ToTarget(m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS), XM_PI * 5.f);

    m_tAttackCoolTime.fAccTime += fDT;

    Get_Transform()->Go_Straight();

    if (m_tAttackCoolTime.fAccTime >= m_tAttackCoolTime.fCoolTime)
    {
        if (Target_In_AttackRange())
        {
            _uint iRan = rand() % 3;

            if (iRan == 0)
                m_eCurState = STATE::skill_1100;
            else if (iRan == 1)
                m_eCurState = STATE::skill_2100;
            else if (iRan == 2)
                m_eCurState = STATE::skill_3100;
        }
        else
            m_eCurState = STATE::b_run;
    }
}

void Silversword_Soldier_FSM::gaze_f_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"gaze_f", 0.2f, true, 1.f);
   
    Get_Transform()->Set_Speed(m_fRunSpeed / 2.f);

    m_bSuperArmor = false;
}

void Silversword_Soldier_FSM::gaze_l()
{
    if (!m_pTarget.expired())
        Soft_Turn_ToTarget(m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS), XM_PI * 5.f);

    m_tAttackCoolTime.fAccTime += fDT;

    Get_Transform()->Go_Left();

    if (m_tAttackCoolTime.fAccTime >= m_tAttackCoolTime.fCoolTime)
    {
        if (Target_In_AttackRange())
        {
            _uint iRan = rand() % 3;

            if (iRan == 0)
                m_eCurState = STATE::skill_1100;
            else if (iRan == 1)
                m_eCurState = STATE::skill_2100;
            else if (iRan == 2)
                m_eCurState = STATE::skill_3100;
        }
        else
            m_eCurState = STATE::b_run;
    }
}

void Silversword_Soldier_FSM::gaze_l_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"gaze_l", 0.2f, true, 1.f);

    Get_Transform()->Set_Speed(m_fRunSpeed / 2.f);

    m_bSuperArmor = false;
}

void Silversword_Soldier_FSM::gaze_r()
{
    if (!m_pTarget.expired())
        Soft_Turn_ToTarget(m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS), XM_PI * 5.f);

    m_tAttackCoolTime.fAccTime += fDT;

    Get_Transform()->Go_Right();

    if (m_tAttackCoolTime.fAccTime >= m_tAttackCoolTime.fCoolTime)
    {
        if (Target_In_AttackRange())
        {
            _uint iRan = rand() % 3;

            if (iRan == 0)
                m_eCurState = STATE::skill_1100;
            else if (iRan == 1)
                m_eCurState = STATE::skill_2100;
            else if (iRan == 2)
                m_eCurState = STATE::skill_3100;
        }
        else
            m_eCurState = STATE::b_run;
    }
}

void Silversword_Soldier_FSM::gaze_r_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"gaze_r", 0.2f, true, 1.f);

    Get_Transform()->Set_Speed(m_fRunSpeed / 2.f);

    m_bSuperArmor = false;
}

void Silversword_Soldier_FSM::airborne_start()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::airborne_end;
}

void Silversword_Soldier_FSM::airborne_start_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"airborne_start", 0.2f, false, 1.f);

    m_bSuperArmor = true;

    Soft_Turn_ToInputDir(m_vHitDir, XM_PI * 10.f);
}

void Silversword_Soldier_FSM::airborne_end()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::airborne_up;
}

void Silversword_Soldier_FSM::airborne_end_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"airborne_end",0.2f, false, 1.f);

    m_bSuperArmor = true;
}

void Silversword_Soldier_FSM::airborne_up()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Silversword_Soldier_FSM::airborne_up_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_Up",0.1f,  false, 1.f);

    m_bSuperArmor = true;
}

void Silversword_Soldier_FSM::hit()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Silversword_Soldier_FSM::hit_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"hit", 0.1f, false, 1.f);

    Soft_Turn_ToInputDir(m_vHitDir, XM_PI * 10.f);
}

void Silversword_Soldier_FSM::knock_start()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::knock_end;
}

void Silversword_Soldier_FSM::knock_start_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_start", 0.1f, false, 1.f);

    m_bSuperArmor = true;

    Soft_Turn_ToInputDir(m_vHitDir, XM_PI * 10.f);
}

void Silversword_Soldier_FSM::knock_end()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::knock_end_loop;
}

void Silversword_Soldier_FSM::knock_end_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_end", 0.2f, false, 1.f);

    m_bSuperArmor = true;
}

void Silversword_Soldier_FSM::knock_end_loop()
{
    if (Get_CurFrame() > Get_FinalFrame() / 2)
        m_eCurState = STATE::knock_up;
}

void Silversword_Soldier_FSM::knock_end_loop_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_end_loop", 0.2f, false, 1.f);

    m_bSuperArmor = false;
}

void Silversword_Soldier_FSM::knock_end_hit()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::knock_end_loop;
}

void Silversword_Soldier_FSM::knock_end_hit_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_end_hit", 0.2f, false, 1.f);
    
    m_bSuperArmor = false;
}

void Silversword_Soldier_FSM::knock_up()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Silversword_Soldier_FSM::knock_up_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_Up", 0.2f, false, 1.f);

    m_bSuperArmor = true;
}

void Silversword_Soldier_FSM::skill_1100()
{
    if (Get_CurFrame() < 17)
    {
        if (!m_pTarget.expired())
            Soft_Turn_ToTarget(m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS), XM_PI * 5.f);
    }
    else if (Get_CurFrame() == 22)
        AttackCollider_On(NORMAL_SKILL);
    else if (Get_CurFrame() == 26)
        AttackCollider_Off();

    if (Is_AnimFinished())
    {
        _uint iRan = 0;
        
        if (Target_In_AttackRange())
            iRan = rand() % 3;
        else
            iRan = rand() % 4;

        if (iRan == 0)
            m_eCurState = STATE::gaze_b;
        else if (iRan == 1)
            m_eCurState = STATE::gaze_l;
        else if (iRan == 2)
            m_eCurState = STATE::gaze_r;
        else if (iRan == 3)
            m_eCurState = STATE::gaze_f;
    }
}

void Silversword_Soldier_FSM::skill_1100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_1100", 0.15f, false, m_fNormalAttack_AnimationSpeed);
    
    m_tAttackCoolTime.fAccTime = 0.f;

    m_bSuperArmor = false;
}

void Silversword_Soldier_FSM::skill_2100()
{
    if (Get_CurFrame() < 11)
    {
        if (!m_pTarget.expired())
            Soft_Turn_ToTarget(m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS), XM_PI * 5.f);
    }
    else if (Get_CurFrame() == 17)
        AttackCollider_On(NORMAL_SKILL);
    else if (Get_CurFrame() == 20)
        AttackCollider_Off();
    else if (Get_CurFrame() == 33)
        AttackCollider_On(NORMAL_SKILL);
    else if (Get_CurFrame() == 38)
        AttackCollider_Off();

    if (Is_AnimFinished())
    {
        _uint iRan = 0;

        if (Target_In_AttackRange())
            iRan = rand() % 3;
        else
            iRan = rand() % 4;

        if (iRan == 0)
            m_eCurState = STATE::gaze_b;
        else if (iRan == 1)
            m_eCurState = STATE::gaze_l;
        else if (iRan == 2)
            m_eCurState = STATE::gaze_r;
        else if (iRan == 3)
            m_eCurState = STATE::gaze_f;
    }
}

void Silversword_Soldier_FSM::skill_2100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_2100", 0.15f, false, m_fNormalAttack_AnimationSpeed);

    m_tAttackCoolTime.fAccTime = 0.f;

    m_bSuperArmor = false;
}

void Silversword_Soldier_FSM::skill_3100()
{
    if (Get_CurFrame() < 11)
    {
        if (!m_pTarget.expired())
            Soft_Turn_ToTarget(m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS), XM_PI * 5.f);
    }
    else if (Get_CurFrame() == 28)
        AttackCollider_On(NORMAL_SKILL);
    else if (Get_CurFrame() == 34)
        AttackCollider_Off();
    else if (Get_CurFrame() == 60)
        AttackCollider_On(KNOCKDOWN_SKILL);
    else if (Get_CurFrame() == 67)
        AttackCollider_Off();

    if (Get_CurFrame() >= 48 && Get_CurFrame() < 69)
        m_bSuperArmor = true;
    else 
        m_bSuperArmor = false;

    if (Is_AnimFinished())
    {
        _uint iRan = 0;

        if (Target_In_AttackRange())
            iRan = rand() % 3;
        else
            iRan = rand() % 4;

        if (iRan == 0)
            m_eCurState = STATE::gaze_b;
        else if (iRan == 1)
            m_eCurState = STATE::gaze_l;
        else if (iRan == 2)
            m_eCurState = STATE::gaze_r;
        else if (iRan == 3)
            m_eCurState = STATE::gaze_f;
    }
}

void Silversword_Soldier_FSM::skill_3100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_3100", 0.15f, false, m_fNormalAttack_AnimationSpeed);

    m_tAttackCoolTime.fAccTime = 0.f;

    m_bSuperArmor = false;
}

void Silversword_Soldier_FSM::CalCulate_PatrolTime()
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
