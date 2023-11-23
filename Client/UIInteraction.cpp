#include "pch.h"
#include "UIInteraction.h" 

UIInteraction::UIInteraction()
{
}

HRESULT UIInteraction::Init()
{
    if (m_pOwner.expired())
        return E_FAIL;



    return S_OK;
}

void UIInteraction::Tick()
{
	if (m_pOwner.expired())
		return;


}
