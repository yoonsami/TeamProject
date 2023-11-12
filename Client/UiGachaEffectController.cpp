#include "pch.h"
#include "UiGachaEffectController.h"

#include "MeshRenderer.h"

UiGachaEffectController::UiGachaEffectController()
{
}

HRESULT UiGachaEffectController::Init()
{
    if (m_pOwner.expired())
        return E_FAIL;

    m_eState = STATE::DECREASE;
    m_fMaxTime = 0.5f;
    m_fCheckTime = 0.f;

    m_fRatio = 1.f / m_fMaxTime;

    return S_OK;
}

void UiGachaEffectController::Tick()
{
	if (m_pOwner.expired())
		return;

    switch (m_eState)
    {
    case UiGachaEffectController::STATE::DECREASE:
        Decrease_Alpha();
        break;
    case UiGachaEffectController::STATE::REMOVE:
        Remove();
        break;
    }
}

void UiGachaEffectController::Decrease_Alpha()
{
    m_fCheckTime += fDT;
    if (m_fMaxTime < m_fCheckTime)
    {
        m_eState = STATE::REMOVE;
        return;
    }

    m_pOwner.lock()->Get_MeshRenderer()->Get_RenderParamDesc().vec4Params[0].w = 1.f - (m_fCheckTime * m_fRatio);
}

void UiGachaEffectController::Remove()
{
    CUR_SCENE->Remove_GameObject(m_pOwner.lock());
}
