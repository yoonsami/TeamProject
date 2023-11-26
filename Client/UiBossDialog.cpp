#include "pch.h"
#include "UiBossDialog.h" 

UiBossDialog::UiBossDialog()
{
}

HRESULT UiBossDialog::Init()
{
    if (m_pOwner.expired())
        return E_FAIL;



    return S_OK;
}

void UiBossDialog::Tick()
{
	if (m_pOwner.expired())
		return;


}
