#include "pch.h"
#include "HeroChangeScript.h"
#include "FontRenderer.h"
#include "Model.h"
#include "ModelAnimator.h"
#include "ModelRenderer.h"
#include "FSM.h"
#include "SpearAce_FSM.h"
#include "Kyle_FSM.h"
#include "WeaponScript.h"

HeroChangeScript::HeroChangeScript(shared_ptr<GameObject> pPlayer)
{
    m_pPlayer = pPlayer;
}

HeroChangeScript::~HeroChangeScript()
{
}

void HeroChangeScript::Tick()
{
    // weak_ptr expired() -> 이 포인터가 이미 소멸해서 사용할 수 없는 상태면 true 반환
    if (m_pPlayer.expired())
        return;

    if (KEYTAP(KEY_TYPE::F1))
    {
        if (m_pPlayer.lock()->Get_Model()->Get_ModelTag() == (L"Spear_Ace"))
            return;
        else
        {
            m_pPlayer.lock()->Get_Animator()->Set_CurrentAnim(0);
            //m_pPlayer.lock()->Get_Animator()->Set_CurrentAnim(L"b_idle", true/*반복 애니메이션*/, 1.f/*애니메이션 속도*/);

            shared_ptr<Model> model = RESOURCES.Get<Model>(L"Spear_Ace");

            m_pPlayer.lock()->Get_Animator()->Set_Model(model);
            m_pPlayer.lock()->Change_Component(make_shared<SpearAce_FSM>());
            m_pPlayer.lock()->Get_FSM()->Init();

            //Add. Player's Weapon
            shared_ptr<GameObject> ObjWeapon = make_shared<GameObject>();

            ObjWeapon->Add_Component(make_shared<Transform>());
            {
                shared_ptr<Shader> shader = RESOURCES.Get<Shader>(L"Shader_Model.fx");

                shared_ptr<ModelRenderer> renderer = make_shared<ModelRenderer>(shader);
                {
                    shared_ptr<Model> model = RESOURCES.Get<Model>(L"Weapon_Spear_Ace");
                    renderer->Set_Model(model);
                }

                ObjWeapon->Add_Component(renderer);

                WeaponScript::WEAPONDESC desc;
                desc.strBoneName = L"Bip001-Prop1";
                desc.matPivot = _float4x4::CreateRotationX(-XM_PI / 2.f) * _float4x4::CreateRotationZ(XM_PI);
                desc.pWeaponOwner = m_pPlayer;

                ObjWeapon->Add_Component(make_shared<WeaponScript>(desc));
            }

            ObjWeapon->Set_Name(L"Weapon_Spear_Ace");
            CUR_SCENE->Add_GameObject(ObjWeapon);
        }
    }
    else if (KEYTAP(KEY_TYPE::F2))
    {
        if (m_pPlayer.lock()->Get_Model()->Get_ModelTag() == (L"Kyle"))
            return;
        else
        {
            if (m_pPlayer.lock()->Get_Model()->Get_ModelTag() == (L"Spear_Ace"))
                CUR_SCENE->Remove_GameObject(CUR_SCENE->Get_GameObject(L"Weapon_Spear_Ace"));

            m_pPlayer.lock()->Get_Animator()->Set_CurrentAnim(0);
            //m_pPlayer.lock()->Get_Animator()->Set_CurrentAnim(L"b_idle", true/*반복 애니메이션*/, 1.f/*애니메이션 속도*/);

            shared_ptr<Model> model = RESOURCES.Get<Model>(L"Kyle");

            m_pPlayer.lock()->Get_Animator()->Set_Model(model);
            m_pPlayer.lock()->Change_Component(make_shared<Kyle_FSM>());
            m_pPlayer.lock()->Get_FSM()->Init();
        }
    }

}

