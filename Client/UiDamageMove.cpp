#include "pch.h"
#include "UiDamageMove.h"

#include "Camera.h"
#include "FontRenderer.h"

UiDamageMove::UiDamageMove()
{
}

HRESULT UiDamageMove::Init()
{
    if (m_pOwner.expired())
        return E_FAIL;

    //m_pCamera = CUR_SCENE->Get_Camera(L"Default");

    m_vecOriginPos = m_pOwner.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);

    m_fOriginSize = m_pOwner.lock()->Get_FontRenderer()->Get_Size();

    m_fMinSize = 1.f;
    m_fChangeScale = m_fOriginSize - m_fMinSize;

    m_fMaxTime = 0.5f;
    m_fRatio = 1.f / m_fMaxTime;

    return S_OK;
}

void UiDamageMove::Tick()
{
	if (m_pOwner.expired())
		return;

    m_fCheckTime += fDT;
    /*Check_Render_State();
    if (false == m_bIsRender)
        return;*/

    Change_Pos();
    Change_Size();
    //Change_Alpha();
    Check_Remove();
}

//void UiDamageMove::Check_Render_State()
//{
//    _float3 cullPos = m_pOwner.lock()->Get_Transform()->Get_State(Transform_State::POS).xyz();
//    _float cullRadius = 1.5f;
//    Frustum frustum = m_pCamera.lock()->Get_Camera()->Get_Frustum();
//
//    m_bIsRender = false;
//    if (frustum.Contain_Sphere(cullPos, cullRadius))
//        m_bIsRender = true;
//}

void UiDamageMove::Change_Pos()
{
    _float4 vecPos = m_pOwner.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
    vecPos.x += fDT * 100.f * m_fRatio;
    //vecPos.y += fDT * 200.f;
    m_pOwner.lock()->GetOrAddTransform()->Set_State(Transform_State::POS, vecPos);
}

void UiDamageMove::Change_Size()
{
    _float fSize = m_fOriginSize - (m_fChangeScale * m_fCheckTime * m_fRatio);
    m_pOwner.lock()->Get_FontRenderer()->Get_Size() = fSize;
}

void UiDamageMove::Change_Alpha()
{
    Color vColor = m_pOwner.lock()->Get_FontRenderer()->Get_Color();
    vColor.w = 1.f - m_fCheckTime * m_fRatio;
    m_pOwner.lock()->Get_FontRenderer()->Get_Color() = vColor;
}

void UiDamageMove::Check_Remove()
{
    if (m_fMaxTime < m_fCheckTime)
        CUR_SCENE->Remove_GameObject(m_pOwner.lock());
}
