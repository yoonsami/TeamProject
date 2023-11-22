#include "pch.h"
#include "UiDamageCreate.h" 

#include "Camera.h"
#include "FontRenderer.h"
#include "UiDamageMove.h"
#include "CoolTimeCheckScript.h"

UiDamageCreate::UiDamageCreate()
{
}

HRESULT UiDamageCreate::Init()
{
    if (m_pOwner.expired())
        return E_FAIL;

    m_pPlayer = CUR_SCENE->Get_GameObject(L"Player");
    m_pCamera = CUR_SCENE->Get_Camera(L"Default");

    m_Color.resize(ElementType::ElementEnd);
    m_Color[ElementType::DARK]  = Color{ 0.5451f , 0.2706f, 0.8196f , 1.f };
    m_Color[ElementType::EARTH] = Color{ 0.7961f , 0.5020f, 0.3765f , 1.f };
    m_Color[ElementType::FIRE]  = Color{ 0.9647f , 0.2863f, 0.2863f , 1.f };
    m_Color[ElementType::LIGHT] = Color{ 0.9843f , 0.8784f, 0.5686f , 1.f };
    m_Color[ElementType::WATER] = Color{ 0.2000f , 0.6039f, 0.9412f , 1.f };
    m_Color[ElementType::WIND]  = Color{ 0.1882f , 0.6863f, 0.5490f , 1.f };

    m_fSize = 2.f;

    return S_OK;
}

void UiDamageCreate::Create_Damage_Font(weak_ptr<GameObject> pObj)
{
    if (true == m_pOwner.expired() ||
        true == m_pPlayer.expired() ||
        true == m_pCamera.expired())
        return;

    if (true == pObj.expired())
        return;

    m_fPos = pObj.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
    Check_In_Screen();
    if (false == m_bIsIn)
        return;

    
    auto pFont = make_shared<GameObject>();
    pFont->GetOrAddTransform()->Set_State(Transform_State::POS, _float4(0.f, 0.f, 0.f, 1.f));

    // 데미지 계산
    pFont->Add_Component(make_shared<FontRenderer>(L"1234"));
    HERO eHero = m_pPlayer.lock()->Get_Script<CoolTimeCheckScript>()->Get_Cur_Hero();

    pFont->Get_FontRenderer()->Set_Font(RESOURCES.Get<CustomFont>(L"136ex"), m_Color[GET_DATA(eHero).Element], m_fSize);
    pFont->Add_Component(make_shared<UiDamageMove>(pObj));
    
    pFont->Set_LayerIndex(Layer_UI);
    pFont->Init();
    CUR_SCENE->Add_GameObject(pFont);

    return;
}

void UiDamageCreate::Create_Damage_Hit()
{

}

void UiDamageCreate::Check_In_Screen()
{
    _float3 cullPos = m_fPos.xyz();
    _float cullRadius = 1.5f;
    Frustum frustum = m_pCamera.lock()->Get_Camera()->Get_Frustum();

    m_bIsIn = false;
    if (frustum.Contain_Sphere(cullPos, cullRadius))
        m_bIsIn = true;
}

void UiDamageCreate::Change_Pos_2D()
{
    _float4x4 viewPos = m_pCamera.lock()->Get_Camera()->Get_ViewMat();
    _float4x4 projPos = m_pCamera.lock()->Get_Camera()->Get_ProjMat();
    m_fPos = XMVector3TransformCoord(m_fPos, viewPos);
    m_fPos = XMVector3TransformCoord(m_fPos, projPos);

    m_fPos.x = (m_fPos.x + 1.f) * 0.5f * 1920.f;
    m_fPos.y = ((m_fPos.y * -1.f) + 1.f) * 0.5f * 1080.f;

    m_fPos.x -= 1920 * 0.5f;
    m_fPos.y = (m_fPos.y * -1.f) + 1080 * 0.5f;
    
    m_fPos.x -= 150.f;
    m_fPos.y += 300.f;
}