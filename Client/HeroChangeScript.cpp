#include "pch.h"
#include "HeroChangeScript.h"

#include "FSM.h"
#include "Model.h"
#include "Kyle_FSM.h"
#include "FontRenderer.h"
#include "WeaponScript.h"
#include "SpearAce_FSM.h"
#include "ModelAnimator.h"
#include "ModelRenderer.h"
#include "CoolTimeCheckScript.h"
#include "WeaponScript.h"
#include "FSM.h"
#include "Material.h"
#include "Player_FSM.h"
#include "SpearAce_FSM.h"
#include "Kyle_FSM.h"
#include "Yeopo_FSM.h"
#include "Dellons_FSM.h"
#include "Spike_FSM.h"
#include "Shane_FSM.h"
#include "Yeonhee_FSM.h"

HeroChangeScript::HeroChangeScript()
{
    auto shader = RESOURCES.Get<Shader>(L"Shader_Model.fx");


    for (_uint i = 0; i < static_cast<_uint>(HERO::MAX); ++i)
    {
		auto& materials = RESOURCES.Get<Model>(GET_DATA(static_cast<HERO>(i)).ModelTag)->Get_Materials();
		for (auto& material : materials)
			material->Set_Shader(shader);
    }
}

HeroChangeScript::~HeroChangeScript()
{
}

HRESULT HeroChangeScript::Init()
{
    return S_OK;
}

void HeroChangeScript::Tick()
{
    
    if (m_pOwner.expired())
        return;

    if (KEYTAP(KEY_TYPE::F1))
    {
        Change_Hero(HERO::ACE3);
    }
    else if (KEYTAP(KEY_TYPE::F2))
    {
        Change_Hero(HERO::KYLE);
    }
    else if (KEYTAP(KEY_TYPE::F3))
    {
        Change_Hero(HERO::YEOPO);
    }
    else if (KEYTAP(KEY_TYPE::F4))
    {
        Change_Hero(HERO::DELLONS);
    }
    else if (KEYTAP(KEY_TYPE::F5))
    {
        Change_Hero(HERO::SPIKE);
    }
	else if (KEYTAP(KEY_TYPE::F6))
	{
        Change_Hero(HERO::SHANE);
	}
	else if (KEYTAP(KEY_TYPE::F7))
	{
        Change_Hero(HERO::YEONHEE);
	}
    else if (KEYTAP(KEY_TYPE::R))
	{
        Change_Hero(HERO::PLAYER);
	}
}

void HeroChangeScript::Change_Hero(HERO eHero)
{
    if (m_pOwner.lock()->Get_Model()->Get_ModelTag() == GET_DATA(eHero).ModelTag)
        Change_To_Input(HERO::PLAYER);
    else
        Change_To_Input(eHero);
}

void HeroChangeScript::Change_Player()
{
    Change_To_Input(HERO::PLAYER);
}

void HeroChangeScript::Change_Character_Weapon(const wstring& weaponname, shared_ptr<GameObject> weapon)
{
    //Add. Player's Weapon
  
    {
       
        shared_ptr<ModelRenderer> renderer = weapon->Get_ModelRenderer();;
        {
            shared_ptr<Model> model = RESOURCES.Get<Model>(weaponname);
            renderer->Set_Model(model);
        }

    }
    auto script = weapon->Get_Script<WeaponScript>();
    if (script)
        script->Set_ModelChanged();
    weapon->Set_Name(weaponname);
    weapon->Get_ModelRenderer()->Set_RenderState(true);
}

void HeroChangeScript::Change_To_Input(HERO eHero)
{
    if (m_pOwner.expired() || HERO::MAX == eHero)
        return;

    //m_pOwner.lock()->Get_FSM()->Reset_Weapon();
    m_pOwner.lock()->Get_FSM()->Reset_Vehicle();

    Get_Owner()->Get_FSM()->FreeLoopMembers();
    //AnimIndex Reset
    m_pOwner.lock()->Get_Animator()->Set_CurrentAnim(0);

    //TimeSlowSetting Reset
    m_pOwner.lock()->Set_TimeSlowed(true);

    //CUR_SCENE->Remove_GameObject(CUR_SCENE->Get_GameObject(L"Player_AttackCollider"));
    //CUR_SCENE->Remove_GameObject(CUR_SCENE->Get_GameObject(L"Vehicle_AttackCollider"));
    shared_ptr<GameObject> attackCollider = m_pOwner.lock()->Get_FSM()->Get_AttackCollider();
    
   auto& tagData = GET_DATA(eHero);

    shared_ptr<Model> model = RESOURCES.Get<Model>(tagData.ModelTag);
    //shared_ptr<Model> model = RESOURCES.Get<Model>(L"Shane");

   m_pOwner.lock()->Get_Animator()->Set_Model(model);

    auto weapon = m_pOwner.lock()->Get_FSM()->Get_Weapon();

    switch (eHero)
    {
    case HERO::PLAYER:
	{
		shared_ptr<Model> model = RESOURCES.Get<Model>(L"Player");

		m_pOwner.lock()->Get_Animator()->Set_Model(model);
	}
        m_pOwner.lock()->Change_Component(make_shared<Player_FSM>());
        attackCollider->Get_Transform()->Scaled(_float3(1.f));
        break;
    case HERO::ACE3:
        m_pOwner.lock()->Change_Component(make_shared<SpearAce_FSM>());
        attackCollider->Get_Transform()->Scaled(_float3(1.f));
        break;
    case HERO::KYLE:
        m_pOwner.lock()->Change_Component(make_shared<Kyle_FSM>());
        attackCollider->Get_Transform()->Scaled(_float3(1.5f));
        break;
    case HERO::YEOPO:
        m_pOwner.lock()->Change_Component(make_shared<Yeopo_FSM>());
        attackCollider->Get_Transform()->Scaled(_float3(1.f));
        break;
    case HERO::DELLONS:
        m_pOwner.lock()->Change_Component(make_shared<Dellons_FSM>());
        attackCollider->Get_Transform()->Scaled(_float3(1.f));
        break;
    case HERO::SPIKE:
        m_pOwner.lock()->Change_Component(make_shared<Spike_FSM>());
        attackCollider->Get_Transform()->Scaled(_float3(1.5f));
        break;
	case HERO::SHANE:
		m_pOwner.lock()->Change_Component(make_shared<Shane_FSM>());
        attackCollider->Get_Transform()->Scaled(_float3(1.f));
		break;
	case HERO::YEONHEE:
		m_pOwner.lock()->Change_Component(make_shared<Yeonhee_FSM>());
        attackCollider->Get_Transform()->Scaled(_float3(1.f));
		break;
    case HERO::MAX:
        break;
    }

	m_pOwner.lock()->Get_FSM()->Set_AttackCollider(attackCollider);
    m_pOwner.lock()->Get_FSM()->Set_Weapon(weapon);
    //Add. Player's Weapon

    if(weapon && weapon->Get_ModelRenderer())
	{
		if (0 != tagData.WeaponTag.length())
			Change_Character_Weapon(tagData.WeaponTag, weapon);
		else
            weapon->Get_ModelRenderer()->Set_RenderState(false);
	}

    m_pOwner.lock()->Get_FSM()->Init();

	 if (m_pOwner.lock()->Get_Script<CoolTimeCheckScript>())
	     m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Cur_Hero(eHero);
}

