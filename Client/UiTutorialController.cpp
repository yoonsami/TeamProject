#include "pch.h"
#include "UiTutorialController.h" 

UiTutorialController::UiTutorialController()
{
}

HRESULT UiTutorialController::Init()
{
    if (m_pOwner.expired())
        return E_FAIL;

    if (true == m_bIsInit)
        return S_OK;

    m_bIsInit = true;

    auto& pResource = RESOURCES;
    m_pTexture.resize(15);
    for (_uint i = 0; i < 15; ++i)
    {
        wstring strKey = L"UI_Tutorial_R_" + to_wstring(i);
        m_pTexture[i] = pResource.Get<Texture>(strKey);
    }
    



    return S_OK;
}

void UiTutorialController::Tick()
{
	if (m_pOwner.expired())
		return;


}
