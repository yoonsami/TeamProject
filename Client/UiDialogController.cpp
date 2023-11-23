#include "pch.h"
#include "UiDialogController.h" 

UiDialogController::UiDialogController()
{
}

HRESULT UiDialogController::Init()
{
    if (m_pOwner.expired())
        return E_FAIL;



    return S_OK;
}

void UiDialogController::Tick()
{
	if (m_pOwner.expired())
		return;


}
