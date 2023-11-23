#include "pch.h"
#include "MainCameraScript.h"
#include "Boss_DellonsWraith_FSM.h"
#include "ModelAnimator.h"
#include "SphereCollider.h"
#include "AttackColliderInfoScript.h"
#include "Model.h"



Boss_DellonsWraith_FSM::Boss_DellonsWraith_FSM()
{
}

Boss_DellonsWraith_FSM::~Boss_DellonsWraith_FSM()
{
}

HRESULT Boss_DellonsWraith_FSM::Init()
{
    if (!m_bInitialize)
    {
        auto animator = Get_Owner()->Get_Animator();
        if (animator)
        {
            animator->Set_CurrentAnim(L"FX_DellonsWraith_skill_30010", true, 1.f);
            m_eCurState = STATE::FX_DellonsWraith_skill_30010;
        }
        shared_ptr<GameObject> attackCollider = make_shared<GameObject>();
        attackCollider->GetOrAddTransform();
        attackCollider->Add_Component(make_shared<SphereCollider>(2.5f));
        attackCollider->Get_Collider()->Set_CollisionGroup(Monster_Attack);

        m_pAttackCollider = attackCollider;

        CUR_SCENE->Add_GameObject(m_pAttackCollider.lock());
        m_pAttackCollider.lock()->Get_Collider()->Set_Activate(false);

        m_pAttackCollider.lock()->Add_Component(make_shared<AttackColliderInfoScript>());
        m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_ColliderOwner(Get_Owner());

        m_pAttackCollider.lock()->Set_Name(L"Boss_Dellons_Wraith_AttackCollider");

        m_iSkillBoneIndex = m_pOwner.lock()->Get_Model()->Get_BoneIndexByName(L"Bip001-R-Hand");
    
    


        m_bInitialize = true;
    }

    return S_OK;
}

void Boss_DellonsWraith_FSM::Tick()
{
    State_Tick();

    if (!m_pAttackCollider.expired())
    {
        //(AnimationModel BonePos)
        matBoneMatrix = m_pOwner.lock()->Get_Animator()->Get_CurAnimTransform(m_iSkillBoneIndex) *
            _float4x4::CreateRotationX(XMConvertToRadians(-90.f)) * _float4x4::CreateScale(0.01f) * _float4x4::CreateRotationY(XM_PI) * m_pOwner.lock()->GetOrAddTransform()->Get_WorldMatrix();

        _float4 vBonePos = _float4{ matBoneMatrix.Translation().x, matBoneMatrix.Translation().y, matBoneMatrix.Translation().z , 1.f };
        m_pAttackCollider.lock()->Get_Transform()->Set_State(Transform_State::POS, vBonePos);
    }
}

void Boss_DellonsWraith_FSM::State_Tick()
{
    State_Init();

    m_iCurFrame = Get_CurFrame();

    switch (m_eCurState)
    {
    case STATE::FX_DellonsWraith_skill_30010:
        FX_DellonsWraith_skill_30010();
        break;
    case STATE::FX_DellonsWraith_skill_400100:
        FX_DellonsWraith_skill_400100();
        break;
    case STATE::FX_DellonsWraith_skill_601100:
        FX_DellonsWraith_skill_601100();
        break;
    case STATE::FX_DellonsWraith_skill_903100:
        FX_DellonsWraith_skill_903100();
        break;
    case STATE::FX_DellonsWraith01_Special_01:
        FX_DellonsWraith01_Special_01();
        break;
    case STATE::FX_SQ_Dellons_QuestEnd_2:
        FX_SQ_Dellons_QuestEnd_2();
        break;
    case STATE::FX_Mn_Dellons_skill_5100:
        FX_Mn_Dellons_skill_5100();
        break;
    case STATE::FX_Mn_Dellons_skill_500200:
        FX_Mn_Dellons_skill_500200();
        break;
    case STATE::FX_Mn_Dellons_skill_5200:
        FX_Mn_Dellons_skill_5200();
        break;
    case STATE::FX_Mn_Dellons_skill_6100:
        FX_Mn_Dellons_skill_6100();
        break;
    }

    if (!m_pGroupEffect.expired())
        m_pGroupEffect.lock()->Get_Transform()->Set_WorldMat(Get_Transform()->Get_WorldMatrix());

    if (m_iPreFrame != m_iCurFrame)
        m_iPreFrame = m_iCurFrame;
}

void Boss_DellonsWraith_FSM::State_Init()
{
    if (m_eCurState != m_ePreState)
    {
        switch (m_eCurState)
        {
        case STATE::FX_DellonsWraith_skill_30010:
            FX_DellonsWraith_skill_30010_Init();
            break;
        case STATE::FX_DellonsWraith_skill_400100:
            FX_DellonsWraith_skill_400100_Init();
            break;
        case STATE::FX_DellonsWraith_skill_601100:
            FX_DellonsWraith_skill_601100_Init();
            break;
        case STATE::FX_DellonsWraith_skill_903100:
            FX_DellonsWraith_skill_903100_Init();
            break;
        case STATE::FX_DellonsWraith01_Special_01:
            FX_DellonsWraith01_Special_01_Init();
            break;
        case STATE::FX_SQ_Dellons_QuestEnd_2:
            FX_SQ_Dellons_QuestEnd_2_Init();
            break;
        case STATE::FX_Mn_Dellons_skill_5100:
            FX_Mn_Dellons_skill_5100_Init();
            break;
        case STATE::FX_Mn_Dellons_skill_500200:
            FX_Mn_Dellons_skill_500200_Init();
            break;
        case STATE::FX_Mn_Dellons_skill_5200:
            FX_Mn_Dellons_skill_5200_Init();
            break;
        case STATE::FX_Mn_Dellons_skill_6100:
            FX_Mn_Dellons_skill_6100_Init();
            break;
        }
        m_ePreState = m_eCurState;
    }
}

void Boss_DellonsWraith_FSM::OnCollision(shared_ptr<BaseCollider> pCollider, _float fGap)
{
}


void Boss_DellonsWraith_FSM::OnCollisionEnter(shared_ptr<BaseCollider> pCollider, _float fGap)
{

}

void Boss_DellonsWraith_FSM::OnCollisionExit(shared_ptr<BaseCollider> pCollider, _float fGap)
{
}

void Boss_DellonsWraith_FSM::Get_Hit(const wstring& skillname, shared_ptr<GameObject> pLookTarget)
{

}

void Boss_DellonsWraith_FSM::AttackCollider_On(const wstring& skillname)
{
    if (!m_pAttackCollider.expired())
    {
        m_pAttackCollider.lock()->Get_Collider()->Set_Activate(true);
        m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_SkillName(skillname);
    }
}

void Boss_DellonsWraith_FSM::AttackCollider_Off()
{
    if (!m_pAttackCollider.expired())
    {
        m_pAttackCollider.lock()->Get_Collider()->Set_Activate(false);
        m_pAttackCollider.lock()->Get_Script<AttackColliderInfoScript>()->Set_SkillName(L"");
    }
}

void Boss_DellonsWraith_FSM::Set_State(_uint iIndex)
{
    m_eCurState = (STATE)iIndex;
}

void Boss_DellonsWraith_FSM::FX_DellonsWraith_skill_30010()
{
    if (Get_CurFrame() == 42)
        AttackCollider_On(NORMAL_ATTACK);
    else if (Get_CurFrame() == 50)
        AttackCollider_Off();
    else if (Get_CurFrame() == 54)
    {
        if (m_iPreFrame != m_iCurFrame)
        {
            FORWARDMOVINGSKILLDESC desc;
            desc.vSkillDir = Get_Transform()->Get_State(Transform_State::LOOK);
            desc.fMoveSpeed = 0.f;
            desc.fLifeTime = 0.5f;
            desc.fLimitDistance = 0.f;


            _float4 vSkillPos = Get_Transform()->Get_State(Transform_State::POS) +
                Get_Transform()->Get_State(Transform_State::RIGHT) * -4.f +
                Get_Transform()->Get_State(Transform_State::LOOK) * 3.f +
                _float3::Up;

            Create_ForwardMovingSkillCollider(vSkillPos, 1.5f, desc, KNOCKBACK_ATTACK);

            vSkillPos = vSkillPos + Get_Transform()->Get_State(Transform_State::LOOK) +
                Get_Transform()->Get_State(Transform_State::RIGHT) * 2.f;

            Create_ForwardMovingSkillCollider(vSkillPos, 1.5f, desc, KNOCKBACK_ATTACK);

            vSkillPos = vSkillPos + Get_Transform()->Get_State(Transform_State::LOOK) +
                Get_Transform()->Get_State(Transform_State::RIGHT) * 2.f;

            Create_ForwardMovingSkillCollider(vSkillPos, 1.5f, desc, KNOCKBACK_ATTACK);

            vSkillPos = vSkillPos - Get_Transform()->Get_State(Transform_State::LOOK) +
                Get_Transform()->Get_State(Transform_State::RIGHT) * 2.f;

            Create_ForwardMovingSkillCollider(vSkillPos, 1.5f, desc, KNOCKBACK_ATTACK);

            vSkillPos = vSkillPos - Get_Transform()->Get_State(Transform_State::LOOK) +
                Get_Transform()->Get_State(Transform_State::RIGHT) * 2.f;

            Create_ForwardMovingSkillCollider(vSkillPos, 1.5f, desc, KNOCKBACK_ATTACK);
        }
    }

    if (Is_AnimFinished())
    {
        CUR_SCENE->Remove_GameObject(m_pAttackCollider.lock());
        CUR_SCENE->Remove_GameObject(m_pOwner.lock());
    }
}

void Boss_DellonsWraith_FSM::FX_DellonsWraith_skill_30010_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"FX_DellonsWraith_skill_30010", 0.15f, false, 1.f);

    if (!m_pTarget.expired())
        Get_Transform()->Set_WorldMat(m_pTarget.lock()->Get_Transform()->Get_WorldMatrix());
}

void Boss_DellonsWraith_FSM::FX_DellonsWraith_skill_400100()
{
    if (Get_CurFrame() == 12)
        AttackCollider_On(KNOCKBACK_ATTACK);
    else if (Get_CurFrame() == 20)
        AttackCollider_Off();


    if (Is_AnimFinished())
    {
        CUR_SCENE->Remove_GameObject(m_pAttackCollider.lock());
        CUR_SCENE->Remove_GameObject(m_pOwner.lock());
    }
}

void Boss_DellonsWraith_FSM::FX_DellonsWraith_skill_400100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"FX_DellonsWraith_skill_400100", 0.15f, false, 1.f);//1.5f);

    if (!m_pTarget.expired())
        Get_Transform()->Set_WorldMat(m_pTarget.lock()->Get_Transform()->Get_WorldMatrix());
}

void Boss_DellonsWraith_FSM::FX_DellonsWraith_skill_601100()
{
    if (Get_CurFrame() == 126)
        AttackCollider_On(KNOCKBACK_ATTACK);
    else if (Get_CurFrame() == 140)
        AttackCollider_Off();


    if (Is_AnimFinished())
    {
        CUR_SCENE->Remove_GameObject(m_pAttackCollider.lock());
        CUR_SCENE->Remove_GameObject(m_pOwner.lock());
    }
}

void Boss_DellonsWraith_FSM::FX_DellonsWraith_skill_601100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"FX_DellonsWraith_skill_601100", 0.15f, false, 1.f);

    if (!m_pTarget.expired())
        Get_Transform()->Set_WorldMat(m_pTarget.lock()->Get_Transform()->Get_WorldMatrix());
}

void Boss_DellonsWraith_FSM::FX_DellonsWraith_skill_903100()
{
    if (Get_CurFrame() == 65)
        AttackCollider_On(NORMAL_ATTACK);
    else if (Get_CurFrame() == 72)
        AttackCollider_Off();
    else if (Get_CurFrame() == 77)
        AttackCollider_On(NORMAL_ATTACK);
    else if (Get_CurFrame() == 82)
        AttackCollider_Off();
    else if (Get_CurFrame() == 109)
        AttackCollider_On(KNOCKBACK_ATTACK);
    else if (Get_CurFrame() == 116)
        AttackCollider_Off();

    if (Is_AnimFinished())
    {
        CUR_SCENE->Remove_GameObject(m_pAttackCollider.lock());
        CUR_SCENE->Remove_GameObject(m_pOwner.lock());
    }
}

void Boss_DellonsWraith_FSM::FX_DellonsWraith_skill_903100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"FX_DellonsWraith_skill_903100", 0.15f, false, 1.f);

    if (!m_pTarget.expired())
        Get_Transform()->Set_WorldMat(m_pTarget.lock()->Get_Transform()->Get_WorldMatrix());
}

void Boss_DellonsWraith_FSM::FX_DellonsWraith01_Special_01()
{
    if (Is_AnimFinished())
    {
        CUR_SCENE->Remove_GameObject(m_pAttackCollider.lock());
        CUR_SCENE->Remove_GameObject(m_pOwner.lock());
    }
}

void Boss_DellonsWraith_FSM::FX_DellonsWraith01_Special_01_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"FX_DellonsWraith01_Special_01", 0.15f, false, 1.f);

    if (!m_pTarget.expired())
        Get_Transform()->Set_WorldMat(m_pTarget.lock()->Get_Transform()->Get_WorldMatrix());
}


void Boss_DellonsWraith_FSM::FX_SQ_Dellons_QuestEnd_2()
{
    if (Get_CurFrame() == 85)
        AttackCollider_On(KNOCKDOWN_ATTACK);
    else if (Get_CurFrame() == 100)
        AttackCollider_Off();

    if (Is_AnimFinished())
    {
        CUR_SCENE->Remove_GameObject(m_pAttackCollider.lock());
        CUR_SCENE->Remove_GameObject(m_pOwner.lock());
    }
}

void Boss_DellonsWraith_FSM::FX_SQ_Dellons_QuestEnd_2_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"FX_SQ_Dellons_QuestEnd_2", 0.15f, false, 1.f);

    if (!m_pTarget.expired())
        Get_Transform()->Set_WorldMat(m_pTarget.lock()->Get_Transform()->Get_WorldMatrix());
}

void Boss_DellonsWraith_FSM::FX_Mn_Dellons_skill_500200()
{
    if (Get_CurFrame() == 19)
        AttackCollider_On(KNOCKDOWN_ATTACK);

    if (Is_AnimFinished())
    {
        CUR_SCENE->Remove_GameObject(m_pAttackCollider.lock());
        CUR_SCENE->Remove_GameObject(m_pOwner.lock());
    }
}

void Boss_DellonsWraith_FSM::FX_Mn_Dellons_skill_500200_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"FX_Mn_Dellons_skill_500200", 0.15f, false, 1.f);//1.5f);

    if (!m_pTarget.expired())
        Get_Transform()->Set_WorldMat(m_pTarget.lock()->Get_Transform()->Get_WorldMatrix());
}

void Boss_DellonsWraith_FSM::FX_Mn_Dellons_skill_5100()
{
    if (Get_CurFrame() == 80)
        AttackCollider_On(KNOCKDOWN_ATTACK);
    else if (Get_CurFrame() == 94)
    {
        CUR_SCENE->Remove_GameObject(m_pAttackCollider.lock());
        CUR_SCENE->Remove_GameObject(m_pOwner.lock());
    }
}

void Boss_DellonsWraith_FSM::FX_Mn_Dellons_skill_5100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"FX_Mn_Dellons_skill_5100", 0.15f, false, 1.5f);

    if (!m_pTarget.expired())
        Get_Transform()->Set_WorldMat(m_pTarget.lock()->Get_Transform()->Get_WorldMatrix());
}


void Boss_DellonsWraith_FSM::FX_Mn_Dellons_skill_6100()
{
    if (Get_CurFrame() == 59)
        AttackCollider_On(KNOCKDOWN_ATTACK);
    else if (Get_CurFrame() == 69)
        AttackCollider_Off();

    if (Is_AnimFinished())
    {
        CUR_SCENE->Remove_GameObject(m_pAttackCollider.lock());
        CUR_SCENE->Remove_GameObject(m_pOwner.lock());
    }
}

void Boss_DellonsWraith_FSM::FX_Mn_Dellons_skill_6100_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"FX_Mn_Dellons_skill_6100", 0.15f, false, 1.f);

    if (!m_pTarget.expired())
        Get_Transform()->Set_WorldMat(m_pTarget.lock()->Get_Transform()->Get_WorldMatrix());
}

void Boss_DellonsWraith_FSM::FX_Mn_Dellons_skill_5200()
{
    if (Get_CurFrame() == 90)
        AttackCollider_On(KNOCKDOWN_ATTACK);
    else if (Get_CurFrame() == 114)
        AttackCollider_Off();

    if (Is_AnimFinished())
    {
        CUR_SCENE->Remove_GameObject(m_pAttackCollider.lock());
        CUR_SCENE->Remove_GameObject(m_pOwner.lock());
    }
}

void Boss_DellonsWraith_FSM::FX_Mn_Dellons_skill_5200_Init()
{
    shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    animator->Set_NextTweenAnim(L"FX_Mn_Dellons_skill_5200", 0.15f, false, 1.5f);

    if (!m_pTarget.expired())
        Get_Transform()->Set_WorldMat(m_pTarget.lock()->Get_Transform()->Get_WorldMatrix());
}



void Boss_DellonsWraith_FSM::Create_ForwardMovingSkillCollider(const _float4& vPos, _float fSkillRange, FORWARDMOVINGSKILLDESC desc, const wstring& SkillType)
{
    shared_ptr<GameObject> SkillCollider = make_shared<GameObject>();

    SkillCollider->GetOrAddTransform();
    SkillCollider->Get_Transform()->Set_State(Transform_State::POS, vPos);
    SkillCollider->Add_Component(make_shared<SphereCollider>(fSkillRange));
    SkillCollider->Get_Collider()->Set_CollisionGroup(Monster_Skill);

    SkillCollider->Add_Component(make_shared<AttackColliderInfoScript>());
    SkillCollider->Get_Collider()->Set_Activate(true);
    SkillCollider->Get_Script<AttackColliderInfoScript>()->Set_SkillName(SkillType);
    SkillCollider->Get_Script<AttackColliderInfoScript>()->Set_ColliderOwner(m_pOwner.lock());
    SkillCollider->Set_Name(L"Boss_Dellons_Wraith_SkillCollider");
    SkillCollider->Add_Component(make_shared<ForwardMovingSkillScript>(desc));
    SkillCollider->Get_Script<ForwardMovingSkillScript>()->Init();

    CUR_SCENE->Add_GameObject(SkillCollider);
}
