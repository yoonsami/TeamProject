#include "pch.h"
#include "MainCameraScript.h"
#include "SpearAce_FSM.h"
#include "ModelAnimator.h"
#include "MotionTrailRenderer.h"
#include "Debug_CreateMotionTrail.h"
#include "SphereCollider.h"
#include "AttackColliderInfoScript.h"
#include "ForwardMovingSkillScript.h"
#include "Model.h"



SpearAce_FSM::SpearAce_FSM()
{
}

SpearAce_FSM::~SpearAce_FSM()
{
}

HRESULT SpearAce_FSM::Init()
{
    auto animator = Get_Owner()->Get_Animator();
    if (animator)
    {
        // ���� �ִϸ��̼� �������ִµ�, ������ ����
        animator->Set_CurrentAnim(L"b_idle"/*�ִϸ��̼� �̸�*/, true/*�ݺ� �ִϸ��̼�*/, 1.f/*�ִϸ��̼� �ӵ�*/);
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
 
    m_pWeapon = CUR_SCENE->Get_GameObject(L"Weapon_Spear_Ace");

    m_iSkillBoneIndex = m_pOwner.lock()->Get_Model()->Get_BoneIndexByName(L"B_nose");


    Get_Owner()->Add_Component(make_shared<Debug_CreateMotionTrail>());

    return S_OK;
}

void SpearAce_FSM::Tick()
{
    State_Tick();

    if (!m_pAttackCollider.expired())
    {
        //m_pAttack transform set forward
        m_pAttackCollider.lock()->Get_Transform()->Set_State(Transform_State::POS, Get_Transform()->Get_State(Transform_State::POS) + Get_Transform()->Get_State(Transform_State::LOOK) * 2.f + _float3::Up);
    }
}

void SpearAce_FSM::State_Tick()
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
    case STATE::skill_1100:
        skill_1100();
        break;
    case STATE::skill_1200:
        skill_1200();
        break;
    case STATE::skill_1300:
        skill_1300();
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
    case STATE::skill_200100:
        skill_200100();
        break;
    case STATE::skill_200200:
        skill_200200();
        break;
    case STATE::skill_300100:
        skill_300100();
        break;
    case STATE::skill_502100:
        skill_502100();
        break;
    case STATE::skill_500100:
        skill_500100();
        break;
    }
}

void SpearAce_FSM::State_Init()
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
        case STATE::skill_1100:
            skill_1100_Init();
            break;
        case STATE::skill_1200:
            skill_1200_Init();
            break;
        case STATE::skill_1300:
            skill_1300_Init();
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
        case STATE::skill_200100:
            skill_200100_Init();
            break;
        case STATE::skill_200200:
            skill_200200_Init();
            break;
        case STATE::skill_300100:
            skill_300100_Init();
            break;
        case STATE::skill_502100:
            skill_502100_Init();
            break;
        case STATE::skill_500100:
            skill_500100_Init();
            break;
        }
        m_ePreState = m_eCurState;
    }
}

void SpearAce_FSM::OnCollision(shared_ptr<BaseCollider> pCollider, _float fGap)
{
}

void SpearAce_FSM::OnCollisionEnter(shared_ptr<BaseCollider> pCollider, _float fGap)
{
}

void SpearAce_FSM::OnCollisionExit(shared_ptr<BaseCollider> pCollider, _float fGap)
{
}

void SpearAce_FSM::Get_Hit(const wstring& skillname, shared_ptr<BaseCollider> pOppositeCollider)
{
}

void SpearAce_FSM::AttackCollider_On(const wstring& skillname)
{
    if (!m_pAttackCollider.expired())
    {
        m_pAttackCollider.lock()->Get_Collider()->Set_Activate(true);
        m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_SkillName(skillname);
    }
}

void SpearAce_FSM::AttackCollider_Off()
{
    if (!m_pAttackCollider.expired())
    {
        m_pAttackCollider.lock()->Get_Collider()->Set_Activate(false);
        m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_SkillName(L"");
    }
}

void SpearAce_FSM::b_idle()
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
        m_eCurState = STATE::skill_502100;
    else if (KEYPUSH(KEY_TYPE::KEY_5))
        m_eCurState = STATE::skill_500100;
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

void SpearAce_FSM::b_idle_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"b_idle", 0.1f, true, 1.f);

    Get_Transform()->Set_Speed(m_fRunSpeed);
    m_tRunEndDelay.fAccTime = 0.f;

    m_pAttackCollider.lock()->Get_Collider()->Set_Activate(false);
    m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_SkillName(L"");

    m_bInvincible = false;
}

void SpearAce_FSM::b_run_start()
{
    EvadeCoolCheck();

    Get_Transform()->Go_Straight();

    _float3 vInputVector = Get_InputDirVector();

    // ����Ű�� �ƹ��͵� ������ ������ ���¸� ����
    if (vInputVector == _float3(0.f))
    {
        m_tRunEndDelay.fAccTime += fDT;

        if (m_tRunEndDelay.fAccTime >= m_tRunEndDelay.fCoolTime)
            m_eCurState = STATE::b_idle;
    }
    else
    {
        // �ִϸ��̼��� ������ �ε�, ���� �ð������� ����� ���� �־,
        // �ٷ� �̾����� �ִϸ��̼��� ��쿡��
        // ������ ���ϸ鼭 �ٲٰų�, 
        // �Ʒ�ó�� ������ ���� ���� �̸� �ٲٸ� �ڿ������� ��ȯ��
        // �� �� ����
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
            m_eCurState = STATE::skill_502100;
        else if (KEYPUSH(KEY_TYPE::KEY_5))
            m_eCurState = STATE::skill_500100;
        else if (KEYPUSH(KEY_TYPE::SPACE))
        {
            if (!m_bEvadeCoolCheck)
                m_eCurState = STATE::skill_91100;
        }
    }
}

void SpearAce_FSM::b_run_start_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"b_run_start", 0.1f, false, 1.5f);

    Get_Transform()->Set_Speed(m_fRunSpeed);
    m_tRunEndDelay.fAccTime = 0.f;
    m_pAttackCollider.lock()->Get_Collider()->Set_Activate(false);
    m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_SkillName(L"");
    m_bInvincible = false;
}

void SpearAce_FSM::b_run()
{
    EvadeCoolCheck();

    Get_Transform()->Go_Straight();

    _float3 vInputVector = Get_InputDirVector();

    // ����Ű�� �ƹ��͵� ������ ������ ���¸� ����
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
        if ((Get_CurFrame() == 1) || Get_CurFrame() >= 19)
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
        m_eCurState = STATE::skill_502100;
    else if (KEYPUSH(KEY_TYPE::KEY_5))
        m_eCurState = STATE::skill_500100;
    else if (KEYPUSH(KEY_TYPE::SPACE))
    {
        if (!m_bEvadeCoolCheck)
            m_eCurState = STATE::skill_91100;
    }
}

void SpearAce_FSM::b_run_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"b_run", 0.2f, true, 1.f);

    Get_Transform()->Set_Speed(m_fRunSpeed);

    m_pAttackCollider.lock()->Get_Collider()->Set_Activate(false);
    m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_SkillName(L"");
    m_bInvincible = false;
}

void SpearAce_FSM::b_run_end_r()
{
    EvadeCoolCheck();

    _float3 vInputVector = Get_InputDirVector();

    // ����Ű�� �ƹ��͵� ������ ������ ���¸� ����
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
        m_eCurState = STATE::skill_502100;
    else if (KEYPUSH(KEY_TYPE::KEY_5))
        m_eCurState = STATE::skill_500100;
    else if (KEYPUSH(KEY_TYPE::SPACE))
    {
        if (!m_bEvadeCoolCheck)
            m_eCurState = STATE::skill_93100;
    }

}

void SpearAce_FSM::b_run_end_r_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"b_run_end_r", 0.1f, false, 1.5f);

    Get_Transform()->Set_Speed(m_fRunSpeed);
    m_tRunEndDelay.fAccTime = 0.f;

    m_pAttackCollider.lock()->Get_Collider()->Set_Activate(false);
    m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_SkillName(L"");
    m_bInvincible = false;
}

void SpearAce_FSM::b_run_end_l()
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
        m_eCurState = STATE::skill_502100;
    else if (KEYPUSH(KEY_TYPE::KEY_5))
        m_eCurState = STATE::skill_500100;
    else if (KEYPUSH(KEY_TYPE::SPACE))
    {
        if (!m_bEvadeCoolCheck)
            m_eCurState = STATE::skill_93100;
    }
}

void SpearAce_FSM::b_run_end_l_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"b_run_end_l", 0.1f, false, 1.5f);

    Get_Transform()->Set_Speed(m_fRunSpeed);
    m_tRunEndDelay.fAccTime = 0.f;

    m_pAttackCollider.lock()->Get_Collider()->Set_Activate(false);
    m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_SkillName(L"");
    m_bInvincible = false;
}

void SpearAce_FSM::b_sprint()
{
    EvadeCoolCheck();

    Get_Transform()->Go_Straight();

    _float3 vInputVector = Get_InputDirVector();

    // ����Ű�� �ƹ��͵� ������ ������ ���¸� ����
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
        m_eCurState = STATE::skill_502100;
    else if (KEYPUSH(KEY_TYPE::KEY_5))
        m_eCurState = STATE::skill_500100;
    else if (KEYPUSH(KEY_TYPE::SPACE))
    {
        if (!m_bEvadeCoolCheck)
            m_eCurState = STATE::skill_91100;
    }
}

void SpearAce_FSM::b_sprint_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"b_sprint", 0.2f, true, 1.f);

    Get_Transform()->Set_Speed(m_fSprintSpeed);

    m_pAttackCollider.lock()->Get_Collider()->Set_Activate(false);
    m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_SkillName(L"");
    m_bInvincible = false;
}

void SpearAce_FSM::b_walk()
{
}

void SpearAce_FSM::b_walk_Init()
{
}

void SpearAce_FSM::die()
{
}

void SpearAce_FSM::die_Init()
{
}

void SpearAce_FSM::skill_1100()
{
    EvadeCoolCheck();

    if (Get_CurFrame() == 19)
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
        m_eCurState = STATE::skill_502100;
    else if (KEYPUSH(KEY_TYPE::KEY_5))
        m_eCurState = STATE::skill_500100;
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

void SpearAce_FSM::skill_1100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_1100", 0.15f, false, m_fNormalAttack_AnimationSpeed);

    m_bCanCombo = false;

    m_vInputTurnVector = _float3(0.f);
    m_vInputTurnVector = Get_InputDirVector();

    AttackCollider_On(NORMAL_SKILL);

    m_bInvincible = false;
}

void SpearAce_FSM::skill_1200()
{
    EvadeCoolCheck();


    if (Get_CurFrame() == 1)
        AttackCollider_On(NORMAL_SKILL);
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
        m_eCurState = STATE::skill_502100;
    else if (KEYPUSH(KEY_TYPE::KEY_5))
        m_eCurState = STATE::skill_500100;
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

void SpearAce_FSM::skill_1200_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_1200", 0.15f, false, m_fNormalAttack_AnimationSpeed);

    m_bCanCombo = false;

    m_vInputTurnVector = _float3(0.f);
    m_vInputTurnVector = Get_InputDirVector();

    AttackCollider_Off();
    
    m_bInvincible = false;
}

void SpearAce_FSM::skill_1300()
{
    EvadeCoolCheck();

    if (Get_CurFrame() == 4)
        AttackCollider_On(NORMAL_SKILL);
    else if (Get_CurFrame() == 28)
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
        m_eCurState = STATE::skill_502100;
    else if (KEYPUSH(KEY_TYPE::KEY_5))
        m_eCurState = STATE::skill_500100;
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

void SpearAce_FSM::skill_1300_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_1300", 0.15f, false, m_fNormalAttack_AnimationSpeed);

    m_bCanCombo = false;

    m_vInputTurnVector = _float3(0.f);
    m_vInputTurnVector = Get_InputDirVector();

    AttackCollider_Off();

    m_bInvincible = false;
}

void SpearAce_FSM::skill_91100()
{
    m_bEvadeCoolCheck = true;

    _float3 vInputVector = Get_InputDirVector();

    if (m_vInputTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vInputTurnVector, XM_PI * 5.f);

    if (Is_AnimFinished())
        m_eCurState = STATE::b_idle;

    if (Get_CurFrame() >= 9)
    {
        if (vInputVector != _float3(0.f))
            m_eCurState = STATE::b_run;
    }
}

void SpearAce_FSM::skill_91100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_91100", 0.15f, false, m_fEvade_AnimationSpeed);

    m_bCanCombo = false;

    m_vInputTurnVector = _float3(0.f);
    m_vInputTurnVector = Get_InputDirVector();

    AttackCollider_Off();

    m_bInvincible = true;
}

void SpearAce_FSM::skill_93100()
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

void SpearAce_FSM::skill_93100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_93100", 0.15f, false, m_fEvade_AnimationSpeed);

    m_bCanCombo = false;
    
    AttackCollider_Off();

    m_bInvincible = true;
}

void SpearAce_FSM::skill_100100()
{
    EvadeCoolCheck();

    if (Get_CurFrame() == 11)
    {
        if (!m_bSkillCreate)
        {
            FORWARDMOVINGSKILLDESC desc;
            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
            desc.fMoveSpeed = 15.f;
            desc.fLifeTime = 2.f;
            desc.fLimitDistance = 6.f;
            
            _float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) + Get_Transform()->Get_State(Transform_State::LOOK) * 2.f + _float3::Up;
            Create_ForwardMovingSkillCollider(vSkillPos, 1.f, desc, NORMAL_SKILL);
        
            m_bSkillCreate = true;
        }
    }
    else if (Get_CurFrame() == 28)
    {
        AttackCollider_On(KNOCKDOWN_SKILL);
    }
    else if (Get_CurFrame() == 58)
        AttackCollider_Off();
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

void SpearAce_FSM::skill_100100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_100100", 0.15f, false, m_fSkillAttack_AnimationSpeed);

    m_bCanCombo = false;

    m_vInputTurnVector = _float3(0.f);
    m_vInputTurnVector = Get_InputDirVector();

    AttackCollider_Off();

    m_bInvincible = false;
}

void SpearAce_FSM::skill_200100()
{
    EvadeCoolCheck();

    if (Get_CurFrame() == 21)
        AttackCollider_Off();
    else if (Get_CurFrame() == 25)
        AttackCollider_On(NORMAL_SKILL);
    else if (Get_CurFrame() == 35)
        AttackCollider_Off();
    else if (Get_CurFrame() == 38)
        AttackCollider_On(NORMAL_SKILL);
    else if (Get_CurFrame() == 61)
        AttackCollider_Off();

    _float3 vInputVector = Get_InputDirVector();

    if (m_vInputTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vInputTurnVector, XM_PI * 5.f);

    if (Get_CurFrame() >= 50)
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

void SpearAce_FSM::skill_200100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_200100", 0.15f, false, m_fSkillAttack_AnimationSpeed);

    m_bCanCombo = false;

    m_vInputTurnVector = _float3(0.f);
    m_vInputTurnVector = Get_InputDirVector();

    AttackCollider_On(NORMAL_SKILL);

    m_bInvincible = false;
}

void SpearAce_FSM::skill_200200()
{
    EvadeCoolCheck();

    if (Get_CurFrame() == 22)
    {
        if (!m_bSkillCreate)
        {
            FORWARDMOVINGSKILLDESC desc;
            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
            desc.fMoveSpeed = 25.f;
            desc.fLifeTime = 1.f;
            desc.fLimitDistance = 12.f;

            _float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) + Get_Transform()->Get_State(Transform_State::LOOK) * 2.f + _float3::Up;
            Create_ForwardMovingSkillCollider(vSkillPos, 1.5f, desc, KNOCKDOWN_SKILL);

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

void SpearAce_FSM::skill_200200_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_200200", 0.15f, false, m_fSkillAttack_AnimationSpeed);

    m_bCanCombo = false;

    m_vInputTurnVector = _float3(0.f);
    m_vInputTurnVector = Get_InputDirVector();

    AttackCollider_Off();

    m_bInvincible = false;
}

void SpearAce_FSM::skill_300100()
{
    EvadeCoolCheck();

    
    if (Get_CurFrame() >= 78 && Get_CurFrame() <= 110)
    {
        /*if (!m_pCamera.expired())
        {
            matBoneMatrix = m_pOwner.lock()->Get_Animator()->Get_CurAnimTransform(m_iSkillBoneIndex) * 
                _float4x4::CreateRotationX(XMConvertToRadians(-90.f)) * _float4x4::CreateScale(0.01f) * _float4x4::CreateRotationY(XM_PI) * m_pOwner.lock()->GetOrAddTransform()->Get_WorldMatrix();
            
            m_pCamera.lock()->Get_Script<MainCameraScript>()->Set_FixedLookTarget(matBoneMatrix.Translation());
        }*/
    }

    if (Get_CurFrame() == 78)
    {       
        if (!m_bSkillCreate)
        {
            //if (!m_pCamera.expired())
            //{
            //    _float3 vDir = matBoneMatrix.Translation() - (CUR_SCENE->Get_Camera(L"Default")->Get_Transform()->Get_State(Transform_State::POS).xyz() + _float3{ 0.f,5.f,0.f });
            //    //vDir = vDir /2.f;
            //    vDir.Normalize();

            //    m_pCamera.lock()->Get_Script<MainCameraScript>()->Fix_Camera(1.5f, vDir * -1.f, 5.f);
            //}
            
            _float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
                Get_Transform()->Get_State(Transform_State::LOOK) * 5.f +
                Get_Transform()->Get_State(Transform_State::RIGHT) * -5.f +
                _float3::Up;

            _float4 vSkillDir = (Get_Transform()->Get_State(Transform_State::POS) - vSkillPos);
            vSkillDir.y = 0.f;

            FORWARDMOVINGSKILLDESC desc;
            desc.vSkillDir = vSkillDir.Normalize();
            desc.fMoveSpeed = 30.f;
            desc.fLifeTime = 1.f;
            desc.fLimitDistance = 9.f;

            Create_ForwardMovingSkillCollider(vSkillPos, 1.f, desc, NORMAL_SKILL);

            m_bSkillCreate = true;
        }
    }
    else if (Get_CurFrame() == 84)
    {
        if (!m_bSkillCreate)
        {
            _float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
                Get_Transform()->Get_State(Transform_State::LOOK) * 5.f +
                Get_Transform()->Get_State(Transform_State::RIGHT) * 5.f +
                _float3::Up;

            _float4 vSkillDir = (Get_Transform()->Get_State(Transform_State::POS) - vSkillPos);
            vSkillDir.y = 0.f;

            FORWARDMOVINGSKILLDESC desc;
            desc.vSkillDir = vSkillDir.Normalize();
            desc.fMoveSpeed = 30.f;
            desc.fLifeTime = 1.f;
            desc.fLimitDistance = 9.f;

            Create_ForwardMovingSkillCollider(vSkillPos, 1.f, desc, NORMAL_SKILL);

            m_bSkillCreate = true;
        }
    }
    else if (Get_CurFrame() == 89)
    {
        if (!m_bSkillCreate)
        {
            _float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
                Get_Transform()->Get_State(Transform_State::LOOK) * -5.f +
                Get_Transform()->Get_State(Transform_State::RIGHT) * 5.f +
                _float3::Up;

            _float4 vSkillDir = (Get_Transform()->Get_State(Transform_State::POS) - vSkillPos);
            vSkillDir.y = 0.f;

            FORWARDMOVINGSKILLDESC desc;
            desc.vSkillDir = vSkillDir.Normalize();
            desc.fMoveSpeed = 30.f;
            desc.fLifeTime = 1.f;
            desc.fLimitDistance = 9.f;

            Create_ForwardMovingSkillCollider(vSkillPos, 1.f, desc, NORMAL_SKILL);

            m_bSkillCreate = true;
        }
    }
    else if (Get_CurFrame() == 94)
    {
        if (!m_bSkillCreate)
        {
            _float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
                Get_Transform()->Get_State(Transform_State::LOOK) * -5.f +
                _float3::Up;

            _float4 vSkillDir = (Get_Transform()->Get_State(Transform_State::POS) - vSkillPos);
            vSkillDir.y = 0.f;

            FORWARDMOVINGSKILLDESC desc;
            desc.vSkillDir = vSkillDir.Normalize();
            desc.fMoveSpeed = 30.f;
            desc.fLifeTime = 1.f;
            desc.fLimitDistance = 9.f;

            Create_ForwardMovingSkillCollider(vSkillPos, 1.f, desc, NORMAL_SKILL);

            m_bSkillCreate = true;
        }
    }
    else if (Get_CurFrame() == 99)
    {
        if (!m_bSkillCreate)
        {
            _float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
                Get_Transform()->Get_State(Transform_State::LOOK) * -5.f +
                Get_Transform()->Get_State(Transform_State::RIGHT) * -5.f +
                _float3::Up;

            _float4 vSkillDir = (Get_Transform()->Get_State(Transform_State::POS) - vSkillPos);
            vSkillDir.y = 0.f;

            FORWARDMOVINGSKILLDESC desc;
            desc.vSkillDir = vSkillDir.Normalize();
            desc.fMoveSpeed = 30.f;
            desc.fLifeTime = 1.f;
            desc.fLimitDistance = 9.f;

            Create_ForwardMovingSkillCollider(vSkillPos, 1.f, desc, NORMAL_SKILL);

            m_bSkillCreate = true;
        }
    }
    else if (Get_CurFrame() == 110)
    {
        if (!m_bSkillCreate)
        {
            _float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS);

            FORWARDMOVINGSKILLDESC desc;
            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
            desc.fMoveSpeed = 0.f;
            desc.fLifeTime = 1.f;
            desc.fLimitDistance = 0.f;

            Create_ForwardMovingSkillCollider(vSkillPos, 2.5f, desc, AIRBORNE_SKILL);

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

void SpearAce_FSM::skill_300100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_300100", 0.15f, false, m_fSkillAttack_AnimationSpeed);

    m_bCanCombo = false;

    m_vInputTurnVector = _float3(0.f);
    m_vInputTurnVector = Get_InputDirVector();

    FORWARDMOVINGSKILLDESC desc;
    desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
    desc.fMoveSpeed = 0.f;
    desc.fLifeTime = 2.f;
    desc.fLimitDistance = 0.f;

    _float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
                        Get_Transform()->Get_State(Transform_State::LOOK) +
                        _float3::Up;

    Create_ForwardMovingSkillCollider(vSkillPos, 1.5f, desc, NORMAL_SKILL);

    AttackCollider_Off();

    m_bInvincible = false;
}

void SpearAce_FSM::skill_502100()
{
    EvadeCoolCheck();

    if (Get_CurFrame() == 17)
    {
        if (!m_bSkillCreate)
        {
            FORWARDMOVINGSKILLDESC desc;
            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
            desc.fMoveSpeed = 20.f;
            desc.fLifeTime = 2.f;
            desc.fLimitDistance = 3.5f;
            
            _float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) + Get_Transform()->Get_State(Transform_State::LOOK) * 2.f + _float3::Up;
            Create_ForwardMovingSkillCollider(vSkillPos, 1.f, desc, NORMAL_SKILL);

            m_bSkillCreate = true;
        }
    }
    else if (Get_CurFrame() == 44)
        AttackCollider_On(KNOCKDOWN_SKILL);
    else if (Get_CurFrame() == 62)
        AttackCollider_Off();
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

void SpearAce_FSM::skill_502100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_502100", 0.15f, false, m_fSkillAttack_AnimationSpeed);

    m_bCanCombo = false;

    m_vInputTurnVector = _float3(0.f);
    m_vInputTurnVector = Get_InputDirVector();

    AttackCollider_Off();

    m_bInvincible = false;
}

void SpearAce_FSM::skill_500100()
{
    EvadeCoolCheck();

    if (Get_CurFrame() == 15)
        AttackCollider_On(KNOCKDOWN_SKILL);
    else if (Get_CurFrame() == 33)
        AttackCollider_Off();

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

void SpearAce_FSM::skill_500100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_500100", 0.15f, false, m_fSkillAttack_AnimationSpeed);

    m_bCanCombo = false;

    m_vInputTurnVector = _float3(0.f);
    m_vInputTurnVector = Get_InputDirVector();

    AttackCollider_Off();

    m_bInvincible = false;
}

void SpearAce_FSM::EvadeCoolCheck()
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

void SpearAce_FSM::Create_ForwardMovingSkillCollider(const _float4& vPos, _float fSkillRange, FORWARDMOVINGSKILLDESC desc, const wstring& SkillType)
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
