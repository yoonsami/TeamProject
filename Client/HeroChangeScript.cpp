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

}

void HeroChangeScript::Change_Hero(HERO eHero)
{
    if (m_pOwner.lock()->Get_Model()->Get_ModelTag() == GET_DATA(eHero).ModelTag)
        Change_To_Input(HERO::PLAYER);
    else
        Change_To_Input(eHero);

}

void HeroChangeScript::Add_Character_Weapon(const wstring& weaponname)
{
    //Add. Player's Weapon
    shared_ptr<GameObject> ObjWeapon = make_shared<GameObject>();

    ObjWeapon->Add_Component(make_shared<Transform>());
    {
        shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Model.fx");

        shared_ptr<ModelRenderer> renderer = make_shared<ModelRenderer>(shader);
        {
            shared_ptr<Model> model = RESOURCES.Get<Model>(weaponname);
            renderer->Set_Model(model);
        }

        ObjWeapon->Add_Component(renderer);

        WeaponScript::WEAPONDESC desc;
        desc.strBoneName = L"Bip001-Prop1";
        desc.matPivot = _float4x4::CreateRotationX(-XM_PI / 2.f) * _float4x4::CreateRotationZ(XM_PI);
        desc.pWeaponOwner = m_pOwner;

        ObjWeapon->Add_Component(make_shared<WeaponScript>(desc));
    }
    ObjWeapon->Get_Transform()->Set_State(Transform_State::POS, _float4{ 0.f, -1000.f,0.f,1.f });
    ObjWeapon->Set_Name(weaponname);
    CUR_SCENE->Add_GameObject(ObjWeapon,true);
}

void HeroChangeScript::Change_To_Player()
{
    if (m_pOwner.expired())
        return;

    m_pOwner.lock()->Get_FSM()->Reset_Weapon();
    m_pOwner.lock()->Get_FSM()->Reset_Vehicle();

    //AnimIndex Reset
    m_pOwner.lock()->Get_Animator()->Set_CurrentAnim(0);

    //PlayerAttackCollider Remove
    CUR_SCENE->Remove_GameObject(CUR_SCENE->Get_GameObject(L"Player_AttackCollider"));
    CUR_SCENE->Remove_GameObject(CUR_SCENE->Get_GameObject(L"Vehicle_AttackCollider"));

    shared_ptr<Model> model = RESOURCES.Get<Model>(L"Player");

    m_pOwner.lock()->Get_Animator()->Set_Model(model);

    m_pOwner.lock()->Change_Component(make_shared<Player_FSM>());

    Add_Character_Weapon(L"Weapon_Player");

    m_pOwner.lock()->Get_FSM()->Init();
    if (m_pOwner.lock()->Get_Script<CoolTimeCheckScript>())
        m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Cur_Hero(HERO::PLAYER);
}

void HeroChangeScript::Change_To_Spear_Ace()
{
    if (m_pOwner.expired())
        return;

    m_pOwner.lock()->Get_FSM()->Reset_Weapon();
    m_pOwner.lock()->Get_FSM()->Reset_Vehicle();

    //AnimIndex Reset
    m_pOwner.lock()->Get_Animator()->Set_CurrentAnim(0);

    //PlayerAttackCollider Remove
    CUR_SCENE->Remove_GameObject(CUR_SCENE->Get_GameObject(L"Player_AttackCollider"));
    CUR_SCENE->Remove_GameObject(CUR_SCENE->Get_GameObject(L"Vehicle_AttackCollider"));

    shared_ptr<Model> model = RESOURCES.Get<Model>(L"Spear_Ace");

    m_pOwner.lock()->Get_Animator()->Set_Model(model);
    m_pOwner.lock()->Change_Component(make_shared<SpearAce_FSM>());

    Add_Character_Weapon(L"Weapon_Spear_Ace");

    m_pOwner.lock()->Get_FSM()->Init();

    if(m_pOwner.lock()->Get_Script<CoolTimeCheckScript>())
        m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Cur_Hero(HERO::ACE3);
}

void HeroChangeScript::Change_To_Kyle()
{
    if (m_pOwner.expired())
        return;

    m_pOwner.lock()->Get_FSM()->Reset_Weapon();
    m_pOwner.lock()->Get_FSM()->Reset_Vehicle();
    //AnimIndex Reset
    m_pOwner.lock()->Get_Animator()->Set_CurrentAnim(0);

    //PlayerAttackCollider Remove
    CUR_SCENE->Remove_GameObject(CUR_SCENE->Get_GameObject(L"Player_AttackCollider"));
    CUR_SCENE->Remove_GameObject(CUR_SCENE->Get_GameObject(L"Vehicle_AttackCollider"));

    shared_ptr<Model> model = RESOURCES.Get<Model>(L"Kyle");

    m_pOwner.lock()->Get_Animator()->Set_Model(model);
    m_pOwner.lock()->Change_Component(make_shared<Kyle_FSM>());
    m_pOwner.lock()->Get_FSM()->Init();
    if (m_pOwner.lock()->Get_Script<CoolTimeCheckScript>())
        m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Cur_Hero(HERO::KYLE);
}

void HeroChangeScript::Change_To_Yeopo()
{

    if (m_pOwner.expired())
        return;

    m_pOwner.lock()->Get_FSM()->Reset_Weapon();
    m_pOwner.lock()->Get_FSM()->Reset_Vehicle();
    //AnimIndex Reset
    m_pOwner.lock()->Get_Animator()->Set_CurrentAnim(0);

    //PlayerAttackCollider Remove
    CUR_SCENE->Remove_GameObject(CUR_SCENE->Get_GameObject(L"Player_AttackCollider"));
    CUR_SCENE->Remove_GameObject(CUR_SCENE->Get_GameObject(L"Vehicle_AttackCollider"));

    shared_ptr<Model> model = RESOURCES.Get<Model>(L"Yeopo");

    m_pOwner.lock()->Get_Animator()->Set_Model(model);
    m_pOwner.lock()->Change_Component(make_shared<Yeopo_FSM>());

    //Add. Player's Weapon
    Add_Character_Weapon(L"Weapon_Yeopo");

    m_pOwner.lock()->Get_FSM()->Init();
    if (m_pOwner.lock()->Get_Script<CoolTimeCheckScript>())
        m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Cur_Hero(HERO::YEOPO);
}

void HeroChangeScript::Change_To_Dellons()
{
    if (m_pOwner.expired())
        return;

    m_pOwner.lock()->Get_FSM()->Reset_Weapon();
    m_pOwner.lock()->Get_FSM()->Reset_Vehicle();
    //AnimIndex Reset
    m_pOwner.lock()->Get_Animator()->Set_CurrentAnim(0);

    //PlayerAttackCollider Remove
    CUR_SCENE->Remove_GameObject(CUR_SCENE->Get_GameObject(L"Player_AttackCollider"));
    CUR_SCENE->Remove_GameObject(CUR_SCENE->Get_GameObject(L"Vehicle_AttackCollider"));

    shared_ptr<Model> model = RESOURCES.Get<Model>(L"Dellons");

    m_pOwner.lock()->Get_Animator()->Set_Model(model);
    m_pOwner.lock()->Change_Component(make_shared<Dellons_FSM>());

    //Add. Player's Weapon
    Add_Character_Weapon(L"Weapon_Dellons");

    m_pOwner.lock()->Get_FSM()->Init();
    if (m_pOwner.lock()->Get_Script<CoolTimeCheckScript>())
        m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Cur_Hero(HERO::DELLONS);

}

void HeroChangeScript::Change_To_Spike()
{
    if (m_pOwner.expired())
        return;

    m_pOwner.lock()->Get_FSM()->Reset_Weapon();
    m_pOwner.lock()->Get_FSM()->Reset_Vehicle();
    //AnimIndex Reset
    m_pOwner.lock()->Get_Animator()->Set_CurrentAnim(0);

    //PlayerAttackCollider Remove
    CUR_SCENE->Remove_GameObject(CUR_SCENE->Get_GameObject(L"Player_AttackCollider"));
    CUR_SCENE->Remove_GameObject(CUR_SCENE->Get_GameObject(L"Vehicle_AttackCollider"));

    shared_ptr<Model> model = RESOURCES.Get<Model>(L"Spike");

    m_pOwner.lock()->Get_Animator()->Set_Model(model);
    m_pOwner.lock()->Change_Component(make_shared<Spike_FSM>());

    //Add. Player's Weapon
    Add_Character_Weapon(L"Weapon_Spike");

    m_pOwner.lock()->Get_FSM()->Init();
    if (m_pOwner.lock()->Get_Script<CoolTimeCheckScript>())
        m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Cur_Hero(HERO::SPIKE);

}

void HeroChangeScript::Change_To_Yeonhee()
{
	if (m_pOwner.expired())
		return;

	m_pOwner.lock()->Get_FSM()->Reset_Weapon();
	m_pOwner.lock()->Get_FSM()->Reset_Vehicle();
	//AnimIndex Reset
	m_pOwner.lock()->Get_Animator()->Set_CurrentAnim(0);

	//PlayerAttackCollider Remove
	CUR_SCENE->Remove_GameObject(CUR_SCENE->Get_GameObject(L"Player_AttackCollider"));
	CUR_SCENE->Remove_GameObject(CUR_SCENE->Get_GameObject(L"Vehicle_AttackCollider"));

	shared_ptr<Model> model = RESOURCES.Get<Model>(L"Yeonhee");

	m_pOwner.lock()->Get_Animator()->Set_Model(model);
	m_pOwner.lock()->Change_Component(make_shared<Yeonhee_FSM>());

	m_pOwner.lock()->Get_FSM()->Init();
    if (m_pOwner.lock()->Get_Script<CoolTimeCheckScript>())
	    m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Cur_Hero(HERO::YEONHEE);
}

void HeroChangeScript::Change_To_Shane()
{
	if (m_pOwner.expired())
		return;

	m_pOwner.lock()->Get_FSM()->Reset_Weapon();
	m_pOwner.lock()->Get_FSM()->Reset_Vehicle();
	//AnimIndex Reset
	m_pOwner.lock()->Get_Animator()->Set_CurrentAnim(0);

	//PlayerAttackCollider Remove
	CUR_SCENE->Remove_GameObject(CUR_SCENE->Get_GameObject(L"Player_AttackCollider"));
	CUR_SCENE->Remove_GameObject(CUR_SCENE->Get_GameObject(L"Vehicle_AttackCollider"));

	shared_ptr<Model> model = RESOURCES.Get<Model>(L"Shane");

	m_pOwner.lock()->Get_Animator()->Set_Model(model);
	m_pOwner.lock()->Change_Component(make_shared<Shane_FSM>());

	//Add. Player's Weapon
	Add_Character_Weapon(L"Weapon_Shane");

	m_pOwner.lock()->Get_FSM()->Init();
    if (m_pOwner.lock()->Get_Script<CoolTimeCheckScript>())
	    m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Cur_Hero(HERO::SHANE);
}

void HeroChangeScript::Change_To_Input(HERO eHero)
{
    if (m_pOwner.expired() || HERO::MAX == eHero)
        return;

    m_pOwner.lock()->Get_FSM()->Reset_Weapon();
    m_pOwner.lock()->Get_FSM()->Reset_Vehicle();


    //AnimIndex Reset
    m_pOwner.lock()->Get_Animator()->Set_CurrentAnim(0);


    CUR_SCENE->Remove_GameObject(CUR_SCENE->Get_GameObject(L"Player_AttackCollider"));
    CUR_SCENE->Remove_GameObject(CUR_SCENE->Get_GameObject(L"Vehicle_AttackCollider"));

    

    auto& tagData = GET_DATA(eHero);
    shared_ptr<Model> model = RESOURCES.Get<Model>(tagData.ModelTag);

    m_pOwner.lock()->Get_Animator()->Set_Model(model);

    switch (eHero)
    {
    case HERO::PLAYER:
        m_pOwner.lock()->Change_Component(make_shared<Player_FSM>());
        break;
    case HERO::ACE3:
        m_pOwner.lock()->Change_Component(make_shared<SpearAce_FSM>());
        break;
    case HERO::KYLE:
        m_pOwner.lock()->Change_Component(make_shared<Kyle_FSM>());
        break;
    case HERO::YEOPO:
        m_pOwner.lock()->Change_Component(make_shared<Yeopo_FSM>());
        break;
    case HERO::DELLONS:
        m_pOwner.lock()->Change_Component(make_shared<Dellons_FSM>());
        break;
    case HERO::SPIKE:
        m_pOwner.lock()->Change_Component(make_shared<Spike_FSM>());
        break;
	case HERO::SHANE:
		m_pOwner.lock()->Change_Component(make_shared<Shane_FSM>());
		break;
	case HERO::YEONHEE:
		m_pOwner.lock()->Change_Component(make_shared<Yeonhee_FSM>());
		break;
    case HERO::MAX:
        break;
    }

    //Add. Player's Weapon
    if(0 != tagData.WeaponTag.length())
        Add_Character_Weapon(tagData.WeaponTag);

    m_pOwner.lock()->Get_FSM()->Init();

    if (m_pOwner.lock()->Get_Script<CoolTimeCheckScript>())
        m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Cur_Hero(eHero);
}

