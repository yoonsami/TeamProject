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
#include "Player_FSM.h"
#include "SpearAce_FSM.h"
#include "Kyle_FSM.h"
#include "Yeopo_FSM.h"
#include "Dellons_FSM.h"

HeroChangeScript::HeroChangeScript()
{
}

HeroChangeScript::~HeroChangeScript()
{
}

void HeroChangeScript::Tick()
{
    
    if (m_pOwner.expired())
        return;

    if (KEYTAP(KEY_TYPE::F1))
    {
        if (m_pOwner.lock()->Get_Model()->Get_ModelTag() == (L"Spear_Ace"))
            Change_To_Player();
        else
            Change_To_Spear_Ace();
    }
    else if (KEYTAP(KEY_TYPE::F2))
    {
        if (m_pOwner.lock()->Get_Model()->Get_ModelTag() == (L"Kyle"))
            Change_To_Player();
        else
            Change_To_Kyle();
    }
    else if (KEYTAP(KEY_TYPE::F3))
    {
        if (m_pOwner.lock()->Get_Model()->Get_ModelTag() == (L"Yeopo"))
            Change_To_Player();
        else
            Change_To_Yeopo();
    }
    else if (KEYTAP(KEY_TYPE::F4))
    {
        if (m_pOwner.lock()->Get_Model()->Get_ModelTag() == (L"Dellons"))
            Change_To_Player();
        else
            Change_To_Dellons();
    }

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
    CUR_SCENE->Add_GameObject(ObjWeapon);
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

    m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Cur_Hero(HERO::ACE3);
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
    m_pOwner.lock()->Get_Script<CoolTimeCheckScript>()->Set_Cur_Hero(HERO::DELLONS);

}

