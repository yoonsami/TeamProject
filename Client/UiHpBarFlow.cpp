#include "pch.h"
#include "UiHpBarFlow.h"

#include "MeshRenderer.h"

UiHpBarFlow::UiHpBarFlow()
{
}

HRESULT UiHpBarFlow::Init()
{
    if (m_pOwner.expired())
        return E_FAIL;

    return S_OK;
}

void UiHpBarFlow::Tick()
{
	if (m_pOwner.expired())
		return;

    m_fValue += fDT * 0.2f;
    m_pOwner.lock()->Get_MeshRenderer()->Get_RenderParamDesc().floatParams[1] = m_fValue;
}
