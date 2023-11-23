#include "pch.h"
#include "MainCameraScript.h"
#include "Boss_Dellons_FSM.h"
#include "ModelAnimator.h"
#include "MotionTrailRenderer.h"
#include "Debug_CreateMotionTrail.h"
#include "SphereCollider.h"
#include "AttackColliderInfoScript.h"
#include "Model.h"
#include "Camera.h"
#include "Boss_DellonsWraith_FSM.h"
#include "MathUtils.h"
#include "UiDamageCreate.h"
#include "UIBossHpBar.h"

Boss_Dellons_FSM::Boss_Dellons_FSM()
{
}

Boss_Dellons_FSM::~Boss_Dellons_FSM()
{
}

HRESULT Boss_Dellons_FSM::Init()
{
    if (!m_bInitialize)
    {
        auto animator = Get_Owner()->Get_Animator();
        if (animator)
        {
            animator->Set_CurrentAnim(L"b_idle", true, 1.f);
            m_eCurState = STATE::n_idle;
        }
        shared_ptr<GameObject> attackCollider = make_shared<GameObject>();
        attackCollider->GetOrAddTransform();
        attackCollider->Add_Component(make_shared<SphereCollider>(1.f));
        attackCollider->Get_Collider()->Set_CollisionGroup(Monster_Attack);

        m_pAttackCollider = attackCollider;

        CUR_SCENE->Add_GameObject(m_pAttackCollider.lock());
        m_pAttackCollider.lock()->Get_Collider()->Set_Activate(false);

        m_pAttackCollider.lock()->Add_Component(make_shared<AttackColliderInfoScript>());
        m_pAttackCollider.lock()->Set_Name(L"Boss_Dellons_AttackCollider");
        m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_ColliderOwner(Get_Owner());

        m_pWeapon = CUR_SCENE->Get_GameObject(L"Weapon_Boss_Dellons");

        m_pCamera = CUR_SCENE->Get_MainCamera();

        m_iCenterBoneIndex = m_pOwner.lock()->Get_Model()->Get_BoneIndexByName(L"Dummy_CP");
        m_iCamBoneIndex = m_pOwner.lock()->Get_Model()->Get_BoneIndexByName(L"Dummy_Cam");
        m_iSkillCamBoneIndex = m_pOwner.lock()->Get_Model()->Get_BoneIndexByName(L"Dummy_SkillCam");

        m_fDetectRange = 7.f;
        m_fRunSpeed = 6.5f;
        m_fSprintSpeed = 8.5f;

        if (!m_pTarget.expired())
            Get_Transform()->LookAt(m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS));


        m_bInitialize = true;
    }


    return S_OK;
}

void Boss_Dellons_FSM::Tick()
{
    State_Tick();

    if (!m_pAttackCollider.expired())
    {
        //m_pAttack transform set forward
        m_pAttackCollider.lock()->Get_Transform()->Set_State(Transform_State::POS, Get_Transform()->Get_State(Transform_State::POS) + Get_Transform()->Get_State(Transform_State::LOOK) * 2.f + _float3::Up);
    }

    Calculate_CamBoneMatrix();
}

void Boss_Dellons_FSM::State_Tick()
{
    State_Init();

    m_iCurFrame = Get_CurFrame();

    switch (m_eCurState)
    {
    case STATE::n_idle:
        n_idle();
        break;
    case STATE::talk_01:
        talk_01();
        break;
    case STATE::Intro:
        Intro();
        break;
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
    case STATE::skill_901000:
        skill_901000();
        break;
    case STATE::skill_901100:
        skill_901100();
        break;
    case STATE::skill_902100:
        skill_902100();
        break;
    case STATE::skill_903100:
        skill_903100();
        break;
    case STATE::skill_904100:
        skill_904100();
        break;
    }

    if (!m_pGroupEffect.expired())
        m_pGroupEffect.lock()->Get_Transform()->Set_WorldMat(Get_Transform()->Get_WorldMatrix());

    if (m_iPreFrame != m_iCurFrame)
        m_iPreFrame = m_iCurFrame;
}

void Boss_Dellons_FSM::State_Init()
{
    if (m_eCurState != m_ePreState)
    {
        switch (m_eCurState)
        {
        case STATE::n_idle:
            n_idle_Init();
            break;
        case STATE::talk_01:
            talk_01_Init();
            break;
        case STATE::Intro:
            Intro_Init();
            break;
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
        case STATE::skill_901000:
            skill_901000_Init();
            break;
        case STATE::skill_901100:
            skill_901100_Init();
            break;
        case STATE::skill_902100:
            skill_902100_Init();
            break;
        case STATE::skill_903100:
            skill_903100_Init();
            break;
        case STATE::skill_904100:
            skill_904100_Init();
            break;
        }
        m_ePreState = m_eCurState;
    }
}

void Boss_Dellons_FSM::OnCollision(shared_ptr<BaseCollider> pCollider, _float fGap)
{
}

void Boss_Dellons_FSM::OnCollisionEnter(shared_ptr<BaseCollider> pCollider, _float fGap)
{
    if (pCollider->Get_Owner() == nullptr)
        return;

    wstring strSkillName = pCollider->Get_Owner()->Get_Script<AttackColliderInfoScript>()->Get_SkillName();

    if (!m_bInvincible)
    {
        shared_ptr<GameObject> targetToLook = nullptr;
        // skillName에 _Skill 포함이면
        if (strSkillName.find(L"_Skill") != wstring::npos)
        {
            targetToLook = pCollider->Get_Owner(); // Collider owner를 넘겨준다
        }
        else // 아니면
            targetToLook = pCollider->Get_Owner()->Get_Script<AttackColliderInfoScript>()->Get_ColliderOwner(); // Collider를 만든 객체를 넘겨준다

        if (targetToLook == nullptr)
            return;

        CUR_SCENE->Get_GameObject(L"UI_Damage_Controller")->Get_Script<UiDamageCreate>()->Create_Damage_Font(Get_Owner());
        Get_Hit(strSkillName, targetToLook);
    }
}

void Boss_Dellons_FSM::OnCollisionExit(shared_ptr<BaseCollider> pCollider, _float fGap)
{
}

void Boss_Dellons_FSM::Get_Hit(const wstring& skillname, shared_ptr<GameObject> pLookTarget)
{
    CUR_SCENE->Get_UI(L"UI_Damage_Controller")->Get_Script<UiDamageCreate>()->Create_Damage_Font(Get_Owner());

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

            if (rand() % 3 == 0)
                m_eCurState = STATE::skill_91100;
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


            if (rand() % 3 == 0)
                m_eCurState = STATE::skill_91100;
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


            if (rand() % 3 == 0)
                m_eCurState = STATE::skill_91100;
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

            if (rand() % 3 == 0)
                m_eCurState = STATE::skill_91100;
        }
    }
}

void Boss_Dellons_FSM::AttackCollider_On(const wstring& skillname)
{
    if (!m_pAttackCollider.expired())
    {
        m_pAttackCollider.lock()->Get_Collider()->Set_Activate(true);
        m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_SkillName(skillname);
    }
}

void Boss_Dellons_FSM::AttackCollider_Off()
{
    if (!m_pAttackCollider.expired())
    {
        m_pAttackCollider.lock()->Get_Collider()->Set_Activate(false);
        m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_SkillName(L"");
    }
}

void Boss_Dellons_FSM::Set_State(_uint iIndex)
{
}

void Boss_Dellons_FSM::n_idle()
{
    if (Target_In_DetectRange())
        m_bDetected = true;

    if (m_bDetected)
    {
        //Add_BossHp UI
        if (!m_pOwner.expired())
        {
            auto pScript = make_shared<UIBossHpBar>(BOSS::DELLONS);
            m_pOwner.lock()->Add_Component(pScript);
            pScript->Init();
        }

        m_eCurState = STATE::b_idle;
    }
}

void Boss_Dellons_FSM::n_idle_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"b_idle", 0.1f, true, 1.f);
    
    Get_Transform()->Set_Speed(m_fRunSpeed);

    AttackCollider_Off();

    m_bInvincible = true;
    m_bSuperArmor = false;
    m_bSetPattern = false;
}

void Boss_Dellons_FSM::talk_01()
{
}

void Boss_Dellons_FSM::talk_01_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"talk_01", 0.1f, true, 1.f);

    AttackCollider_Off();

    m_bInvincible = true;
    m_bSuperArmor = false;
}

void Boss_Dellons_FSM::Intro()
{
    
    if (m_iPreFrame != m_iCurFrame)
    {
        Summon_Wraith();

        Set_WraithState((_uint)Boss_DellonsWraith_FSM::STATE::FX_Mn_Dellons_skill_500200);
    }
    
    if (Is_AnimFinished())
        m_eCurState = STATE::n_idle;
}

void Boss_Dellons_FSM::Intro_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_902100", 0.1f, false, 1.f);

    AttackCollider_Off();

    m_bInvincible = true;
    m_bSuperArmor = false;
}

void Boss_Dellons_FSM::b_idle()
{
    Battle_Start();

    if (m_bBattleStart)
    {
        if (!m_pTarget.expired())
            Soft_Turn_ToTarget(m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS), m_fTurnSpeed);

        m_tAttackCoolTime.fAccTime += fDT;

        if (!m_bSetPattern)
        {
            if (m_tAttackCoolTime.fAccTime >= m_tAttackCoolTime.fCoolTime)
                Set_AttackSkill_Phase1();
        }
        else
        {
            if (Target_In_AttackRange())
                m_eCurState = m_ePatternState;
            else
                m_eCurState = STATE::b_run_start;
        }
    }
}

void Boss_Dellons_FSM::b_idle_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    if (m_ePreState != STATE::skill_400100)
        animator->Set_NextTweenAnim(L"b_idle", 0.1f, true, 1.f);
    else
        animator->Set_NextTweenAnim(L"b_idle", 0.3f, true, 1.f);

    Get_Transform()->Set_Speed(m_fRunSpeed);

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Boss_Dellons_FSM::b_run_start()
{
    if (!m_pTarget.expired())
        Soft_Turn_ToTarget(m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS), m_fTurnSpeed);

    Get_Transform()->Go_Straight();
    
    if (Is_AnimFinished())
        m_eCurState = STATE::b_run;

    if (Target_In_AttackRange())
        m_eCurState = m_ePatternState;
    else
    {
        if (Is_AnimFinished())
            m_eCurState = STATE::b_run;
    }
}

void Boss_Dellons_FSM::b_run_start_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"b_run_start", 0.1f, false, 1.5f);

    Get_Transform()->Set_Speed(m_fRunSpeed);

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Boss_Dellons_FSM::b_run()
{
    if (!m_pTarget.expired())
        Soft_Turn_ToTarget(m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS), m_fTurnSpeed);

    Get_Transform()->Go_Straight();

    m_tSprintCoolTime.fAccTime += fDT;
    
    if (Target_In_AttackRange())
    {
        m_eCurState = m_ePatternState;
    }
    else
    {
        if (m_tSprintCoolTime.fAccTime >= m_tSprintCoolTime.fCoolTime)
            m_bSprint = true;

        if (m_bSprint)
        {
            if (Get_CurFrame() == 1)
            {
                m_bSprint = false;
                m_eCurState = STATE::b_sprint;
            }
        }
    }
}

void Boss_Dellons_FSM::b_run_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"b_run", 0.2f, true, 1.f);

    Get_Transform()->Set_Speed(m_fRunSpeed);

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;
    m_bSprint = false;
}

void Boss_Dellons_FSM::b_run_end_r()
{
}

void Boss_Dellons_FSM::b_run_end_r_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"b_run_end_r", 0.1f, false, 1.5f);

    Get_Transform()->Set_Speed(m_fRunSpeed);
    m_tRunEndDelay.fAccTime = 0.f;

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Boss_Dellons_FSM::b_run_end_l()
{
}

void Boss_Dellons_FSM::b_run_end_l_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"b_run_end_l", 0.1f, false, 1.5f);

    Get_Transform()->Set_Speed(m_fRunSpeed);
    m_tRunEndDelay.fAccTime = 0.f;

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Boss_Dellons_FSM::b_sprint()
{
    if (!m_pTarget.expired())
        Soft_Turn_ToTarget(m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS), m_fTurnSpeed);

    Get_Transform()->Go_Straight();

    if (Target_In_AttackRange())
        m_eCurState = m_ePatternState;
}

void Boss_Dellons_FSM::b_sprint_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"b_sprint", 0.2f, true, 1.f);

    Get_Transform()->Set_Speed(m_fSprintSpeed);

    AttackCollider_Off();

    m_tSprintCoolTime.fAccTime = 0.f;

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Boss_Dellons_FSM::b_walk()
{
}

void Boss_Dellons_FSM::b_walk_Init()
{
}

void Boss_Dellons_FSM::die()
{
}

void Boss_Dellons_FSM::die_Init()
{
}

void Boss_Dellons_FSM::airborne_start()
{
    Soft_Turn_ToInputDir(m_vHitDir, m_fTurnSpeed);

    if (Is_AnimFinished())
        m_eCurState = STATE::airborne_end;
}

void Boss_Dellons_FSM::airborne_start_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"airborne_start", 0.2f, false, 1.f);

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = true;
}

void Boss_Dellons_FSM::airborne_end()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::airborne_up;
}

void Boss_Dellons_FSM::airborne_end_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"airborne_end", 0.2f, false, 1.f);

    m_bInvincible = false;
    m_bSuperArmor = true;
}

void Boss_Dellons_FSM::airborne_up()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Boss_Dellons_FSM::airborne_up_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_Up", 0.1f, false, 1.f);

    m_bInvincible = false;
    m_bSuperArmor = true;
}

void Boss_Dellons_FSM::hit()
{
    Soft_Turn_ToInputDir(m_vHitDir, m_fTurnSpeed);

    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Boss_Dellons_FSM::hit_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"hit", 0.1f, false, 1.f);

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Boss_Dellons_FSM::knock_start()
{
    Soft_Turn_ToInputDir(m_vHitDir, m_fTurnSpeed);

    Get_Transform()->Go_Backward();

    if (Is_AnimFinished())
        m_eCurState = STATE::knock_end;
}

void Boss_Dellons_FSM::knock_start_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_start", 0.1f, false, 1.f);

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = true;

    Get_Transform()->Set_Speed(m_fKnockBackSpeed * 0.8f);
}

void Boss_Dellons_FSM::knock_end()
{
    if (Get_CurFrame() < 16)
        Get_Transform()->Go_Backward();

    if (Is_AnimFinished())
        m_eCurState = STATE::knock_end_loop;
}

void Boss_Dellons_FSM::knock_end_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_end", 0.2f, false, 1.f);

    m_bInvincible = false;
    m_bSuperArmor = true;

    Get_Transform()->Set_Speed(m_fKnockBackSpeed * 0.5f);
}

void Boss_Dellons_FSM::knock_end_loop()
{
    m_tKnockDownEndCoolTime.fAccTime += fDT;

    if (Get_CurFrame() > Get_FinalFrame() / 2)
        m_eCurState = STATE::knock_up;
}

void Boss_Dellons_FSM::knock_end_loop_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_end_loop", 0.2f, false, 1.f);

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Boss_Dellons_FSM::knock_end_hit()
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

void Boss_Dellons_FSM::knock_end_hit_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_end_hit", 0.2f, false, 1.f);

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Boss_Dellons_FSM::knock_up()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Boss_Dellons_FSM::knock_up_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knock_Up", 0.2f, false, 1.f);

    m_bInvincible = false;
    m_bSuperArmor = true;

    m_tKnockDownEndCoolTime.fAccTime = 0.f;

    Get_Transform()->Set_Speed(m_fRunSpeed);
}

void Boss_Dellons_FSM::knockdown_start()
{
    Soft_Turn_ToInputDir(m_vHitDir, m_fTurnSpeed);

    Get_Transform()->Go_Backward();

    if (Is_AnimFinished())
        m_eCurState = STATE::knockdown_end;
}

void Boss_Dellons_FSM::knockdown_start_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"knockdown_start", 0.1f, false, 2.f);

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = true;

    Get_Transform()->Set_Speed(m_fKnockDownSpeed);
}

void Boss_Dellons_FSM::knockdown_end()
{
    if (Get_CurFrame() < 16)
        Get_Transform()->Go_Backward();

    if (Is_AnimFinished())
        m_eCurState = STATE::knock_up;
}

void Boss_Dellons_FSM::knockdown_end_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_CurrentAnim(L"knockdown_end", false, 1.5f);

    m_bInvincible = false;
    m_bSuperArmor = true;

    Get_Transform()->Set_Speed(m_fKnockDownSpeed * 0.5f);
}

void Boss_Dellons_FSM::stun()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Boss_Dellons_FSM::stun_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"stun", 0.1f, false, 1.f);

    m_bInvincible = false;
    m_bSuperArmor = true;
}

void Boss_Dellons_FSM::skill_1100()
{
    if (m_vTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vTurnVector, m_fTurnSpeed);

    if (Get_CurFrame() == 9)
        AttackCollider_On(NORMAL_ATTACK);
    else if (Get_CurFrame() == 19)
        AttackCollider_Off();

    if (Get_CurFrame() == 25)
        m_eCurState = STATE::skill_1200;
}

void Boss_Dellons_FSM::skill_1100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_1100", 0.15f, false, 1.5f);

    m_vTurnVector = Calculate_TargetTurnVector();

    m_tAttackCoolTime.fAccTime = 0.f;
    m_bSetPattern = false;

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Boss_Dellons_FSM::skill_1200()
{
    if (m_vTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vTurnVector, m_fTurnSpeed);

    if (Get_CurFrame() == 8)
        AttackCollider_On(NORMAL_ATTACK);
    else if (Get_CurFrame() == 18)
        AttackCollider_Off();
  
    if (Get_CurFrame() == 21)
        m_eCurState = STATE::skill_1300;
}

void Boss_Dellons_FSM::skill_1200_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_1200", 0.15f, false, 1.5f);

    m_vTurnVector = Calculate_TargetTurnVector();

    m_tAttackCoolTime.fAccTime = 0.f;
    m_bSetPattern = false;

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Boss_Dellons_FSM::skill_1300()
{
    if (m_vTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vTurnVector, m_fTurnSpeed);

    if (Get_CurFrame() == 8)
        AttackCollider_On(NORMAL_ATTACK);
    else if (Get_CurFrame() == 33)
        AttackCollider_Off();

    if (Get_CurFrame() == 19)
        m_eCurState = STATE::skill_1400;
}

void Boss_Dellons_FSM::skill_1300_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_1300", 0.15f, false, 1.5f);

    m_vTurnVector = Calculate_TargetTurnVector();

    m_tAttackCoolTime.fAccTime = 0.f;
    m_bSetPattern = false;

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Boss_Dellons_FSM::skill_1400()
{
    if (m_vTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vTurnVector, m_fTurnSpeed);

    if (Get_CurFrame() == 8)
        AttackCollider_On(NORMAL_ATTACK);
    else if (Get_CurFrame() == 14)
        AttackCollider_Off();
    else if (Get_CurFrame() == 16)
        AttackCollider_On(KNOCKBACK_ATTACK);
    else if (Get_CurFrame() == 24)
        AttackCollider_Off();

    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Boss_Dellons_FSM::skill_1400_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_1400", 0.15f, false, 1.5f);

    m_vTurnVector = Calculate_TargetTurnVector();

    m_tAttackCoolTime.fAccTime = 0.f;
    m_bSetPattern = false;

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = false;
}

void Boss_Dellons_FSM::skill_91100()
{
    if (m_vTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vTurnVector, m_fTurnSpeed);

    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Boss_Dellons_FSM::skill_91100_Init()
{
    m_vTurnVector = MathUtils::Get_RandomVector(_float3{ -1.f,0.f,-1.f }, _float3{ 1.f,0.f,1.f });
    m_vTurnVector.Normalize();

    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_91100", 0.15f, false, m_fEvade_AnimationSpeed);

    AttackCollider_Off();
    
    m_bInvincible = true;
    m_bSuperArmor = false;
}

void Boss_Dellons_FSM::skill_93100()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Boss_Dellons_FSM::skill_93100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_93100", 0.15f, false, m_fEvade_AnimationSpeed);

    AttackCollider_Off();

    m_bInvincible = true;
    m_bSuperArmor = false;
}

void Boss_Dellons_FSM::skill_100100()
{
    if (m_vTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vTurnVector, m_fTurnSpeed);

    if (Get_CurFrame() == 12)
    {
        if (m_iPreFrame != m_iCurFrame)
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
        }
    }

    if (Get_CurFrame() == 27)
        m_eCurState = STATE::skill_100200;
}

void Boss_Dellons_FSM::skill_100100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_100100", 0.15f, false, 1.f);

    m_vTurnVector = Calculate_TargetTurnVector();

    m_tAttackCoolTime.fAccTime = 0.f;
    m_bSetPattern = false;

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = true;
}

void Boss_Dellons_FSM::skill_100200()
{
    if (m_vTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vTurnVector, m_fTurnSpeed);

    if (Get_CurFrame() == 15)
    {
        if (m_iPreFrame != m_iCurFrame)
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
        }
    }
 
    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
}

void Boss_Dellons_FSM::skill_100200_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_100200", 0.15f, false, 1.f);

    m_vTurnVector = Calculate_TargetTurnVector();

    m_tAttackCoolTime.fAccTime = 0.f;
    m_bSetPattern = false;

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = true;
}

void Boss_Dellons_FSM::skill_200100()
{
    if (m_vTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vTurnVector, m_fTurnSpeed);

    if (Get_CurFrame() == 7)
        AttackCollider_On(KNOCKBACK_ATTACK);
    else if (Get_CurFrame() == 12)
        AttackCollider_Off();

    if (Get_CurFrame() == 21)
        m_eCurState = STATE::skill_200200;
}

void Boss_Dellons_FSM::skill_200100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_200100", 0.15f, false, 1.f);

    m_vTurnVector = Calculate_TargetTurnVector();

    m_tAttackCoolTime.fAccTime = 0.f;
    m_bSetPattern = false;

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = true;
}

void Boss_Dellons_FSM::skill_200200()
{
    if (m_vTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vTurnVector, m_fTurnSpeed);

    if (Get_CurFrame() == 7)
    {
        if (m_iPreFrame != m_iCurFrame)
        {
            FORWARDMOVINGSKILLDESC desc;
            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
            desc.fMoveSpeed = 0.f;
            desc.fLifeTime = 1.f;
            desc.fLimitDistance = 0.f;

            _float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) + Get_Transform()->Get_State(Transform_State::LOOK) * 3.f + _float3::Up;
            Create_ForwardMovingSkillCollider(vSkillPos, 2.f, desc, KNOCKBACK_SKILL);
        }
    }

    if (Is_AnimFinished())
    {
        m_eCurState = STATE::b_idle;
    }

}

void Boss_Dellons_FSM::skill_200200_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_200200", 0.15f, false, 1.f);

    m_vTurnVector = Calculate_TargetTurnVector();

    m_tAttackCoolTime.fAccTime = 0.f;
    m_bSetPattern = false;

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = true;
}

void Boss_Dellons_FSM::skill_300100()
{
    if (m_vTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vTurnVector, m_fTurnSpeed);

    /*if (Get_CurFrame() >= 10)
    {
        if (!m_pCamera.expired())
        {
            _float4 vDestinationPos = (Get_Transform()->Get_State(Transform_State::POS)) +
                m_vSkillCamRight +
                (Get_Transform()->Get_State(Transform_State::LOOK) * -3.f)
                + _float3::Up * 6.f;
            _float4 vDir = vDestinationPos - m_vSkillCamBonePos;

            vDir.Normalize();

            m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_FollowSpeed(1.5f);
            m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_FixedLookTarget(m_vSkillCamBonePos.xyz());
            m_pCamera.lock()->Get_Script<MainCameraScript>()->Fix_Camera(0.3f, vDir.xyz(), 13.f);
        }
    }*/


    if (Get_CurFrame() == 10)
    {
        if (m_iPreFrame != m_iCurFrame)
        {
            Summon_Wraith();

            Set_WraithState((_uint)Boss_DellonsWraith_FSM::STATE::FX_DellonsWraith_skill_30010);
        }
    }

    if (Is_AnimFinished())
    {
        m_eCurState = STATE::b_idle;
    }
}

void Boss_Dellons_FSM::skill_300100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_300100", 0.15f, false, 1.f);

    m_vTurnVector = Calculate_TargetTurnVector();

    m_tAttackCoolTime.fAccTime = 0.f;
    m_bSetPattern = false;

    AttackCollider_Off();

    m_bInvincible = true;
    m_bSuperArmor = true;

    Calculate_SkillCamRight();
}

void Boss_Dellons_FSM::skill_400100()
{
    if (m_vTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vTurnVector, m_fTurnSpeed);

    /*if (Get_CurFrame() >= 13)
    {
        if (!m_pCamera.expired())
        {
            _float4 vDestinationPos = (Get_Transform()->Get_State(Transform_State::POS)) +
                m_vSkillCamRight +
                (Get_Transform()->Get_State(Transform_State::LOOK) * -3.f)
                + _float3::Up * 6.f;
            _float4 vDir = vDestinationPos - m_vSkillCamBonePos;

            vDir.Normalize();

            m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_FollowSpeed(1.5f);
            m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_FixedLookTarget(m_vSkillCamBonePos.xyz());
            m_pCamera.lock()->Get_Script<MainCameraScript>()->Fix_Camera(0.3f, vDir.xyz(), 4.5f);
        }
    }*/

    if (Get_CurFrame() == 20)
    {
        if (m_iPreFrame != m_iCurFrame)
        {
            Summon_Wraith();

            Set_WraithState((_uint)Boss_DellonsWraith_FSM::STATE::FX_Mn_Dellons_skill_5100);
        }
    }
    else if (Get_CurFrame() == 33 ||
             Get_CurFrame() == 40 ||
             Get_CurFrame() == 47 ||
             Get_CurFrame() == 60 ||
             Get_CurFrame() == 67 ||
             Get_CurFrame() == 72)
    {
        if (m_iPreFrame != m_iCurFrame)
        {
            FORWARDMOVINGSKILLDESC desc;
            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
            desc.fMoveSpeed = 20.f;
            desc.fLifeTime = 0.5f;
            desc.fLimitDistance = 3.5f;

            _float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) + Get_Transform()->Get_State(Transform_State::LOOK) * 2.f + _float3::Up;
            Create_ForwardMovingSkillCollider(vSkillPos, 1.f, desc, NORMAL_ATTACK);
        }
    }
    else if (Get_CurFrame() == 102)
    {
        if (m_iPreFrame != m_iCurFrame)
        {
            FORWARDMOVINGSKILLDESC desc;
            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
            desc.fMoveSpeed = 20.f;
            desc.fLifeTime = 1.f;
            desc.fLimitDistance = 2.f;

            _float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) + Get_Transform()->Get_State(Transform_State::LOOK) * 2.f + _float3::Up;
            Create_ForwardMovingSkillCollider(vSkillPos, 1.f, desc, KNOCKDOWN_SKILL);
        }
    }

    if (Get_CurFrame() == 120)
    {
        m_eCurState = STATE::b_idle;
    }
}

void Boss_Dellons_FSM::skill_400100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_400100", 0.15f, false, m_fSkillAttack_AnimationSpeed);

    m_vTurnVector = Calculate_TargetTurnVector();

    m_tAttackCoolTime.fAccTime = 0.f;
    m_bSetPattern = false;

    AttackCollider_Off();

    m_bInvincible = true;
    m_bSuperArmor = true;

    Calculate_SkillCamRight();
}

void Boss_Dellons_FSM::skill_501100()
{
    if (m_vTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vTurnVector, m_fTurnSpeed);


    if (Get_CurFrame() == 4)
    {
        //Summon Wraith
        if (m_iPreFrame != m_iCurFrame)
        {
            Summon_Wraith();

            Set_WraithState((_uint)Boss_DellonsWraith_FSM::STATE::FX_Mn_Dellons_skill_500200);
        }
    }

    if (Is_AnimFinished())
    {
        m_eCurState = STATE::b_idle;
    }

}

void Boss_Dellons_FSM::skill_501100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_501100", 0.15f, false, 1.f);

    m_vTurnVector = Calculate_TargetTurnVector();

    m_tAttackCoolTime.fAccTime = 0.f;
    m_bSetPattern = false;

    AttackCollider_Off();

    m_bInvincible = false;
    m_bSuperArmor = true;
}

void Boss_Dellons_FSM::skill_901000()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;
    
}

void Boss_Dellons_FSM::skill_901000_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_901100", 0.15f, false, 1.f);
    
    AttackCollider_Off();

    m_tAttackCoolTime.fAccTime = 0.f;
    m_bSetPattern = false;

    m_bInvincible = false;
    m_bSuperArmor = true;
}

void Boss_Dellons_FSM::skill_901100()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;

}

void Boss_Dellons_FSM::skill_901100_Init()
{
    m_tAttackCoolTime.fAccTime = 0.f;
    m_bSetPattern = false;
}

void Boss_Dellons_FSM::skill_902100()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;

}

void Boss_Dellons_FSM::skill_902100_Init()
{
    m_tAttackCoolTime.fAccTime = 0.f;
    m_bSetPattern = false;
}

void Boss_Dellons_FSM::skill_903100()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;

}

void Boss_Dellons_FSM::skill_903100_Init()
{
    m_tAttackCoolTime.fAccTime = 0.f;
    m_bSetPattern = false;
}

void Boss_Dellons_FSM::skill_904100()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;

}

void Boss_Dellons_FSM::skill_904100_Init()
{
    m_tAttackCoolTime.fAccTime = 0.f;
    m_bSetPattern = false;
}


void Boss_Dellons_FSM::Battle_Start()
{
    if (!m_bBattleStart)
    {
        m_tBattleStartTime.fAccTime += fDT;

        if (m_tBattleStartTime.fAccTime >= m_tBattleStartTime.fCoolTime)
            m_bBattleStart = true;
    }
}

void Boss_Dellons_FSM::Create_ForwardMovingSkillCollider(const _float4& vPos, _float fSkillRange, FORWARDMOVINGSKILLDESC desc, const wstring& SkillType)
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
    m_pSkillCollider.lock()->Set_Name(L"Boss_Dellons_SkillCollider");
    m_pSkillCollider.lock()->Add_Component(make_shared<ForwardMovingSkillScript>(desc));
    m_pSkillCollider.lock()->Get_Script<ForwardMovingSkillScript>()->Init();

    CUR_SCENE->Add_GameObject(m_pSkillCollider.lock());
}

void Boss_Dellons_FSM::Summon_Wraith()
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
    ObjWraith->Add_Component(make_shared<Boss_DellonsWraith_FSM>());
    ObjWraith->Get_FSM()->Set_Target(m_pOwner.lock());
    ObjWraith->Get_FSM()->Init();
    ObjWraith->Set_Name(L"Boss_Dellons_Wraith");

    CUR_SCENE->Add_GameObject(ObjWraith);

    m_pDellonsWraith = ObjWraith;
}

void Boss_Dellons_FSM::Set_WraithState(_uint iAnimindex)
{
    if (!m_pDellonsWraith.expired())
        m_pDellonsWraith.lock()->Get_FSM()->Set_State(iAnimindex);
}

void Boss_Dellons_FSM::Calculate_SkillCamRight()
{
    _float4 vDir = _float4(0.f);
    vDir = m_pCamera.lock()->Get_Transform()->Get_State(Transform_State::POS) - Get_Transform()->Get_State(Transform_State::POS);
    vDir.y = 0.f;
    vDir.Normalize();

    _float4 vCross = _float4(0.f);
    vCross = XMVector3Cross(Get_Transform()->Get_State(Transform_State::LOOK), vDir);

    if (XMVectorGetY(vCross) < 0.f) // LEFT 
        m_vSkillCamRight = (Get_Transform()->Get_State(Transform_State::RIGHT) * -3.f);
    else //RIGHT	
        m_vSkillCamRight = (Get_Transform()->Get_State(Transform_State::RIGHT) * 3.f);
}

void Boss_Dellons_FSM::Set_AttackSkill_Phase1()
{
    _uint iRan = rand() % 6;

    while (true)
    {
        if (iRan == m_iPreAttack)
            iRan = rand() % 6;
        else
            break;
    }

    if (iRan == 0)
    {
        m_fAttackRange = 2.f;
        m_ePatternState = STATE::skill_1100;
        m_iPreAttack = 0;
    }
    else if (iRan == 1)
    {
        m_fAttackRange = 3.f;
        m_ePatternState = STATE::skill_100100;
        m_iPreAttack = 1;
    }
    else if (iRan == 2)
    {
        m_fAttackRange = 5.f;
        m_ePatternState = STATE::skill_200100;
        m_iPreAttack = 2;
    }
    else if (iRan == 3)
    {
        m_fAttackRange = 5.f;
        m_ePatternState = STATE::skill_300100;
        m_iPreAttack = 3;
    }
    else if (iRan == 4)
    {
        m_fAttackRange = 2.5f;
        m_ePatternState = STATE::skill_400100;
        m_iPreAttack = 4;
    }
    else if (iRan == 5)
    {
        m_fAttackRange = 10.f;
        m_ePatternState = STATE::skill_501100;
        m_iPreAttack = 5;
    }

    m_bSetPattern = true;
}


_float3 Boss_Dellons_FSM::Calculate_TargetTurnVector()
{
    if (m_pTarget.expired())
        return _float3(0.f);
    else
        return m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS).xyz() - m_pOwner.lock()->Get_Transform()->Get_State(Transform_State::POS).xyz();
}
