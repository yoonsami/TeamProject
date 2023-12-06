#include "pch.h"
#include "Friend_FSM.h"
#include "ModelAnimator.h"
#include "ObjectDissolve.h"
#include "ModelRenderer.h"
#include "WeaponScript.h"
#include "Model.h"

Friend_FSM::Friend_FSM(HERO eType)
    : m_eType(eType)
{
}

HRESULT Friend_FSM::Init()
{
	shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Model.fx");
	{
		shared_ptr<ModelAnimator> animator = make_shared<ModelAnimator>(shader);
		shared_ptr<Model> model = RESOURCES.Get<Model>(GET_DATA(m_eType).ModelTag);
		animator->Set_Model(model);
		Get_Owner()->Add_Component(animator);
	}
	_float3 vPos = Get_Transform()->Get_State(Transform_State::POS).xyz();
	m_fBottomHeight = vPos.y;

	Get_Transform()->Set_State(Transform_State::POS, _float4(vPos, 1.f) + _float3::Up * 5.f);

    if(!GET_DATA(m_eType).WeaponTag.empty())
    {
        shared_ptr<GameObject> weapon = make_shared<GameObject>();
        weapon->GetOrAddTransform();
        shared_ptr<ModelRenderer> renderer = make_shared<ModelRenderer>(shader);
        shared_ptr<Model> model = RESOURCES.Get<Model>(GET_DATA(m_eType).WeaponTag);
        renderer->Set_Model(model);
        weapon->Add_Component(renderer);

		WeaponScript::WEAPONDESC desc;
		desc.strBoneName = L"Bip001-Prop1";
		desc.matPivot = _float4x4::CreateRotationX(-XM_PI / 2.f) * _float4x4::CreateRotationZ(XM_PI);
		desc.pWeaponOwner = Get_Owner();
        weapon->Add_Component(make_shared<WeaponScript>(desc));
        weapon->Set_DrawShadow(true);
        weapon->Set_VelocityMap(true);
        weapon->Set_Name(L"Friend_Weapon");
        weapon->Init();
        EVENTMGR.Create_Object(weapon);
        Set_Weapon(weapon);
    }



    m_eCurState = STATE::fall_loop;
	{
		shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();
		auto animation = Get_Owner()->Get_Model()->Get_AnimationByName(L"fall_loop");
		if (animation)
			animator->Set_CurrentAnim(L"fall_loop", true, 1.f);
		else
		{
			animation = Get_Owner()->Get_Model()->Get_AnimationByName(L"fall");
			if (animation)
				animator->Set_CurrentAnim(L"fall", true, 1.f);
		}
	}

    return S_OK;
}

void Friend_FSM::Tick()
{
    State_Tick();
}

void Friend_FSM::Get_Hit(const wstring& skillname, _float fDamage, shared_ptr<GameObject> pLookTarget)
{
}

void Friend_FSM::State_Tick()
{
    State_Init();

    switch (m_eCurState)
    {
    case Friend_FSM::STATE::fall_loop:
        fall_loop();
        break;
    case Friend_FSM::STATE::fall_end:
        fall_end();
        break;
    case Friend_FSM::STATE::ATTACK:
        ATTACK();
        break;
    case Friend_FSM::STATE::EXIT:
        EXIT();
        break;
    }
}

void Friend_FSM::State_Init()
{
    if (m_eCurState != m_ePreState)
    {
		switch (m_eCurState)
		{
		case Friend_FSM::STATE::fall_loop:
            fall_loop_Init();
            break;
        case Friend_FSM::STATE::fall_end:
            fall_end_Init();
            break;
        case Friend_FSM::STATE::ATTACK:
            ATTACK_Init();
            break;
        case Friend_FSM::STATE::EXIT:
            EXIT_Init();
            break;
        }
        m_ePreState = m_eCurState;
    }
}

void Friend_FSM::Set_State(_uint iIndex)
{
}

void Friend_FSM::fall_loop()
{
    _float3 vPos = Get_Transform()->Get_State(Transform_State::POS).xyz();
    m_fStateAcc -= 9.8f * fDT;
    vPos += _float3(0.f, m_fStateAcc, 0.f) * fDT;


    if (vPos.y < m_fBottomHeight)
    {
        vPos.y = m_fBottomHeight;
        m_eCurState = STATE::fall_end;
    }

    Get_Transform()->Set_State(Transform_State::POS, _float4(vPos, 1.F));
}

void Friend_FSM::fall_loop_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();
    auto animation = Get_Owner()->Get_Model()->Get_AnimationByName(L"fall_loop");
    if(animation)
	    animator->Set_NextTweenAnim(L"fall_loop", 0.15f, true, 1.f);
    else
    {
        animation = Get_Owner()->Get_Model()->Get_AnimationByName(L"fall");
        if(animation)
            animator->Set_NextTweenAnim(L"fall", 0.15f, true, 1.f);
    }
    m_fStateAcc = 0.f;
}

void Friend_FSM::fall_end()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::ATTACK;
}

void Friend_FSM::fall_end_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_CurrentAnim(L"fall_end",  false, 10.f);
	m_fStateAcc = 0.f;
}

void Friend_FSM::ATTACK()
{
    if (Is_AnimFinished())
        m_eCurState = STATE::EXIT;
}

void Friend_FSM::ATTACK_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

    switch (m_eType)
    {
    case HERO::PLAYER:
        break;
    case HERO::ACE3:
        animator->Set_NextTweenAnim(L"skill_500100", 0.15f, false, 1.5f);
        break;
    case HERO::KYLE:
        animator->Set_NextTweenAnim(L"skill_500100", 0.15f, false, 1.5f);
        break;
    case HERO::YEOPO:
        animator->Set_NextTweenAnim(L"skill_501100", 0.15f, false, 1.5f);
        break;
    case HERO::DELLONS:
        animator->Set_NextTweenAnim(L"skill_400100", 0.15f, false, 1.5f);
        break;
    case HERO::SPIKE:
        animator->Set_NextTweenAnim(L"skill_300100", 0.15f, false, 1.5f);
        break;
	case HERO::YEONHEE:
		animator->Set_NextTweenAnim(L"skill_501100", 0.15f, false, 1.5f);
		break;
	case HERO::SHANE:
		animator->Set_NextTweenAnim(L"skill_502100", 0.15f, false, 1.5f);
		break;
    case HERO::MAX:
        break;
    default:
        break;
    }

	
}

void Friend_FSM::EXIT()
{
}

void Friend_FSM::EXIT_Init()
{
	shared_ptr<ModelAnimator> animator = Get_Owner()->Get_Animator();

	animator->Set_NextTweenAnim(L"b_idle", 0.15f, true, 1.f);

	{
		auto script = make_shared<ObjectDissolve>(1.f);
		Get_Owner()->Add_Component(script);
		script->Init();
	}
    if(!m_pWeapon.expired())
    {
		auto script = make_shared<ObjectDissolve>(1.f);
        m_pWeapon.lock()->Add_Component(script);
		script->Init();
    }
   
}
