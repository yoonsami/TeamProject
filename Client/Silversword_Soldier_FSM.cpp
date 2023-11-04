#include "pch.h"
#include "Silversword_Soldier_FSM.h"
#include "ModelAnimator.h"
#include "SphereCollider.h"

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
    m_pAttackCollider.lock()->Get_Collider()->Set_Activate(true);

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
        }
        m_ePreState = m_eCurState;
    }
}

void Silversword_Soldier_FSM::OnCollision(shared_ptr<BaseCollider> pCollider, _float fGap)
{
}

void Silversword_Soldier_FSM::OnCollisionEnter(shared_ptr<BaseCollider> pCollider, _float fGap)
{
}

void Silversword_Soldier_FSM::OnCollisionExit(shared_ptr<BaseCollider> pCollider, _float fGap)
{
}

void Silversword_Soldier_FSM::b_idle()
{
 
}

void Silversword_Soldier_FSM::b_idle_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"b_idle", 0.1f, true, 1.f);

    Get_Transform()->Set_Speed(m_fRunSpeed);

}

void Silversword_Soldier_FSM::b_run()
{
    Get_Transform()->Go_Straight();

 
}

void Silversword_Soldier_FSM::b_run_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"b_run", 0.2f, true, 1.f);

    Get_Transform()->Set_Speed(m_fRunSpeed);
}

void Silversword_Soldier_FSM::die()
{
}

void Silversword_Soldier_FSM::die_Init()
{
}

void Silversword_Soldier_FSM::skill_1100()
{
 /*   _float3 vInputVector = Get_InputDirVector();

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
    }*/
}

void Silversword_Soldier_FSM::skill_1100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_1100", 0.15f, false, m_fNormalAttack_AnimationSpeed);

    m_vInputTurnVector = _float3(0.f);
    m_vInputTurnVector = Get_InputDirVector();
}

void Silversword_Soldier_FSM::skill_1200()
{

    //_float3 vInputVector = Get_InputDirVector();

    //if (m_vInputTurnVector != _float3(0.f))
    //    Soft_Turn_ToInputDir(m_vInputTurnVector, XM_PI * 5.f);


    //if (_float(Get_CurFrame()) / _float(Get_FinalFrame()) >= 0.25f)
    //    m_bCanCombo = true;

    //if (m_bCanCombo)
    //{
    //    if (KEYTAP(KEY_TYPE::LBUTTON))
    //        m_eCurState = STATE::skill_1300;
    //}

    //if (Is_AnimFinished())
    //{
    //    m_bCanCombo = false;
    //    m_eCurState = STATE::b_idle;
    //}

    //if (KEYPUSH(KEY_TYPE::KEY_1))
    //    m_eCurState = STATE::skill_100100;
    //else if (KEYPUSH(KEY_TYPE::KEY_2))
    //    m_eCurState = STATE::skill_200100;
    //else if (KEYPUSH(KEY_TYPE::KEY_3))
    //    m_eCurState = STATE::skill_300100;
    //else if (KEYPUSH(KEY_TYPE::KEY_4))
    //    m_eCurState = STATE::skill_502100;
    //else if (KEYPUSH(KEY_TYPE::KEY_5))
    //    m_eCurState = STATE::skill_500100;
    //else if (KEYPUSH(KEY_TYPE::SPACE))
    //{
    //    if (!m_bEvadeCoolCheck)
    //    {
    //        if (vInputVector != _float3(0.f))
    //            m_eCurState = STATE::skill_91100;
    //        else
    //            m_eCurState = STATE::skill_93100;
    //    }
    //}
}

void Silversword_Soldier_FSM::skill_1200_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_1200", 0.15f, false, m_fNormalAttack_AnimationSpeed);

    m_vInputTurnVector = _float3(0.f);
    m_vInputTurnVector = Get_InputDirVector();
}

void Silversword_Soldier_FSM::skill_1300()
{
    _float3 vInputVector = Get_InputDirVector();

    if (m_vInputTurnVector != _float3(0.f))
        Soft_Turn_ToInputDir(m_vInputTurnVector, XM_PI * 5.f);

    if (Is_AnimFinished())
    {
        m_eCurState = STATE::b_idle;
    }
}

void Silversword_Soldier_FSM::skill_1300_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_1300", 0.15f, false, m_fNormalAttack_AnimationSpeed);

    m_vInputTurnVector = _float3(0.f);
    m_vInputTurnVector = Get_InputDirVector();
}