#include "pch.h"
#include "MainCameraScript.h"
#include "YeopoHorse_FSM.h"
#include "ModelAnimator.h"
#include "MotionTrailRenderer.h"
#include "Debug_CreateMotionTrail.h"
#include "SphereCollider.h"
#include "AttackColliderInfoScript.h"
#include "Model.h"



YeopoHorse_FSM::YeopoHorse_FSM()
{
}

YeopoHorse_FSM::~YeopoHorse_FSM()
{
}

HRESULT YeopoHorse_FSM::Init()
{
    auto animator = Get_Owner()->Get_Animator();
    if (animator)
    {
        animator->Set_CurrentAnim(L"n_idle", true, 1.f);
        m_eCurState = STATE::n_idle;
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

   
    return S_OK;
}

void YeopoHorse_FSM::Tick()
{
    if (!m_pTarget.expired())
        Get_Transform()->Set_WorldMat(m_pTarget.lock()->Get_Transform()->Get_WorldMatrix());
    

    State_Tick();
}

void YeopoHorse_FSM::State_Tick()
{
    State_Init();

    switch (m_eCurState)
    {
    case STATE::n_idle:
        n_idle();
        break;
    case STATE::skill_400100_fx:
        skill_400100_fx();
        break;
    case STATE::skill_902100_fx:
        skill_902100_fx();
        break;
    }
}

void YeopoHorse_FSM::State_Init()
{
    if (m_eCurState != m_ePreState)
    {
        switch (m_eCurState)
        {
        case STATE::n_idle:
            n_idle_Init();
            break;
        case STATE::skill_400100_fx:
            skill_400100_fx_Init();
            break;
        case STATE::skill_902100_fx:
            skill_902100_fx_Init();
            break;
        }
        m_ePreState = m_eCurState;
    }
}

void YeopoHorse_FSM::OnCollision(shared_ptr<BaseCollider> pCollider, _float fGap)
{
}

void YeopoHorse_FSM::OnCollisionEnter(shared_ptr<BaseCollider> pCollider, _float fGap)
{

}

void YeopoHorse_FSM::OnCollisionExit(shared_ptr<BaseCollider> pCollider, _float fGap)
{
}

void YeopoHorse_FSM::Get_Hit(const wstring& skillname, shared_ptr<BaseCollider> pOppositeCollider)
{
    
}

void YeopoHorse_FSM::AttackCollider_On(const wstring& skillname)
{
  
}

void YeopoHorse_FSM::AttackCollider_Off()
{
   
}

void YeopoHorse_FSM::n_idle()
{
}

void YeopoHorse_FSM::n_idle_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"n_idle", 0.1f, true, 1.f);
}

void YeopoHorse_FSM::skill_400100_fx()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::n_idle;
}

void YeopoHorse_FSM::skill_400100_fx_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_400100_fx", 0.1f, true, 1.f);
}

void YeopoHorse_FSM::skill_902100_fx()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::n_idle;
}

void YeopoHorse_FSM::skill_902100_fx_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"skill_902100_fx", 0.1f, true, 1.f);
}
