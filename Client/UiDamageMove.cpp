#include "pch.h"
#include "UiDamageMove.h"

#include "Camera.h"
#include "FontRenderer.h"
#include "MeshRenderer.h"

UiDamageMove::UiDamageMove(weak_ptr<GameObject> pObj, _uint iIndex, _float2 vecRandAdd, shared_ptr<GameObject> pFirstNum)
    : m_pTarget(pObj)
    , m_iIndex(iIndex)
    , m_vecRandAddPos(vecRandAdd)
    , m_pFirstNum(pFirstNum)
{
}

HRESULT UiDamageMove::Init()
{
    if (m_pOwner.expired())
        return E_FAIL;

    m_pCamera = CUR_SCENE->Get_Camera(L"Default");

    m_fMaxTime = 1.f;
    m_fRatio = 1.f / m_fMaxTime;

    m_pOwner.lock()->Get_MeshRenderer()->Get_RenderParamDesc().floatParams[0] = 100.f;
    m_pOwner.lock()->Get_MeshRenderer()->Get_RenderParamDesc().floatParams[1] = static_cast<_float>(m_iIndex);
    m_pOwner.lock()->Get_MeshRenderer()->Get_RenderParamDesc().vec2Params[0] = m_vecRandAddPos;

    

    return S_OK;
}

void UiDamageMove::Tick()
{
    m_fCheckTime += fDT;
    Check_Remove();

	if (true == m_pOwner.expired() ||
        true == m_pTarget.expired() ||
        true == m_pCamera.expired())
		return;

    Check_Render_State();
    if (false == m_bIsRender)
        return;

    Change_Alpha();
    Change_Pos();
}

void UiDamageMove::Check_Render_State()
{
    _float3 cullPos = m_pTarget.lock()->Get_Transform()->Get_State(Transform_State::POS).xyz();
    _float cullRadius = 1.f;
    Frustum frustum = m_pCamera.lock()->Get_Camera()->Get_Frustum();

    m_bIsRender = false;
    if (frustum.Contain_Sphere(cullPos, cullRadius))
    {
        m_pOwner.lock()->Set_Render(true);
        m_bIsRender = true;
    }
    else
        m_pOwner.lock()->Set_Render(false);
}

void UiDamageMove::Change_Pos()
{
    m_pOwner.lock()->Get_MeshRenderer()->Get_RenderParamDesc().floatParams[2] = m_fCheckTime * 50.f;

    _float4 vecPos = m_pTarget.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
    vecPos.y += 1.f;
    m_pOwner.lock()->Get_MeshRenderer()->Get_RenderParamDesc().vec4Params[1] = vecPos;

    _float4x4 matView = m_pCamera.lock()->Get_Camera()->Get_ViewMat();
    _float4x4 matProj = m_pCamera.lock()->Get_Camera()->Get_ProjMat();

    m_pOwner.lock()->Get_MeshRenderer()->Get_RenderParamDesc().matParams[0] = matView;
    m_pOwner.lock()->Get_MeshRenderer()->Get_RenderParamDesc().matParams[1] = matProj;
}

void UiDamageMove::Check_Remove()
{
    if (m_fMaxTime < m_fCheckTime && false == m_pOwner.expired() && 0 == m_iIndex)
        EVENTMGR.Delete_Object(m_pOwner.lock());

    else if(0 != m_iIndex && false == m_pOwner.expired() && true == m_pFirstNum.expired())
        EVENTMGR.Delete_Object(m_pOwner.lock());
}

void UiDamageMove::Change_Alpha()
{
    _float fAlpha = 1.f - m_fCheckTime * m_fRatio;
    m_pOwner.lock()->Get_MeshRenderer()->Get_RenderParamDesc().vec4Params[0].w = fAlpha;
}

void UiDamageMove::Change_Faram()
{

}
