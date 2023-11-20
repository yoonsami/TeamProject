#include "pch.h"
#include "UiMouseClick.h" 

#include "MeshRenderer.h"

UiMouseClick::UiMouseClick()
{
}

HRESULT UiMouseClick::Init()
{
    if (m_pOwner.expired())
        return E_FAIL;

    m_fMaxTime = 0.25f;
    m_fCheckTime = 0.f;
    m_fRatio = 1.f / m_fMaxTime;

    m_pOwner.lock()->Get_MeshRenderer()->Get_RenderParamDesc().vec4Params[0].w = 0.f;

    return S_OK;
}

void UiMouseClick::Tick()
{
	if (m_pOwner.expired())
		return;

    if (KEYTAP(KEY_TYPE::LBUTTON))
        Start_Effect();

    if (STATE::START == m_eState)
        Start();
}

void UiMouseClick::Start_Effect()
{
    m_fCheckTime = 0.f;
    m_eState = STATE::START;
    m_pOwner.lock()->Get_MeshRenderer()->Get_RenderParamDesc().vec4Params[0].w = 1.f;

    POINT ptMouse = INPUT.GetMousePosToPoint();
    _float4 vecPos = m_pOwner.lock()->GetOrAddTransform()->Get_State(Transform_State::POS);
    vecPos.x = static_cast<_float>(ptMouse.x - g_iWinSizeX / 2.f);
    vecPos.y = static_cast<_float>(ptMouse.y * -1.f + g_iWinSizeY / 2.f);
    m_pOwner.lock()->GetOrAddTransform()->Set_State(Transform_State::POS, vecPos);
}

void UiMouseClick::Start()
{
    m_fCheckTime += fDT;
    m_pOwner.lock()->Get_MeshRenderer()->Get_RenderParamDesc().vec4Params[0].w = 1.f - m_fCheckTime * m_fRatio;

    if (m_fMaxTime < m_fCheckTime)
    {
        m_fCheckTime = 0.f;
        m_eState = STATE::NONE;
        m_pOwner.lock()->Get_MeshRenderer()->Get_RenderParamDesc().vec4Params[0].w = 0.f;
    }
}
