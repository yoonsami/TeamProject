#include "pch.h"
#include "UiSettingController.h" 

#include "BaseUI.h"

UiSettingController::UiSettingController()
{
}

HRESULT UiSettingController::Init()
{
    if (m_pOwner.expired())
        return E_FAIL;

    if (true == m_bIsInit)
        return S_OK;

    m_bIsInit = true;

    auto pScene = CUR_SCENE;
    m_pBg               = pScene->Get_UI(L"UI_Setting_Bg");
    m_pBrightness       = pScene->Get_UI(L"UI_Setting_Star_Brightness");
    m_pContrast         = pScene->Get_UI(L"UI_Setting_Star_Contrast");
    m_pSaturation       = pScene->Get_UI(L"UI_Setting_Star_Saturation");
    m_pBloom            = pScene->Get_UI(L"UI_Setting_Star_Bloom");
    m_pToneMapping      = pScene->Get_UI(L"UI_Setting_Star_ToneMapping");
    m_pToneMappingMax   = pScene->Get_UI(L"UI_Setting_Star_ToneMapping_Brightness");
    m_pSSAO_Radius      = pScene->Get_UI(L"UI_Setting_Star_SSAO_Radius");
    m_pSSAO_Start       = pScene->Get_UI(L"UI_Setting_Star_SSAO_Start");
    m_pSSAO_End         = pScene->Get_UI(L"UI_Setting_Star_SSAO_End");
    m_pMotionBlur       = pScene->Get_UI(L"UI_Setting_Star_MotionBlur");
    m_pPBR_Attenuation  = pScene->Get_UI(L"UI_Setting_Star__PBR_Attenuation");
    m_pPBR_Ratio        = pScene->Get_UI(L"UI_Setting_Star__PBR_Ratio");
    m_pPBR_ShadowBius   = pScene->Get_UI(L"UI_Setting_Star__PBR_ShadowBius");
    m_pCheck_Bloom      = pScene->Get_UI(L"UI_Setting_Check_Bloom");
    m_pCheck_SSAO       = pScene->Get_UI(L"UI_Setting_Check_SSAO");
    m_pCheck_MotionBlur = pScene->Get_UI(L"UI_Setting_Check_MotionBlur");
    m_pCheck_LensFlare  = pScene->Get_UI(L"UI_Setting_Check_LensFlare");
    m_pCheck_Outline    = pScene->Get_UI(L"UI_Setting_Check_Outline");
    m_pCheck_FXAA       = pScene->Get_UI(L"UI_Setting_Check_FXAA");
    m_pCheck_PBR        = pScene->Get_UI(L"UI_Setting_Check_PBR");

    m_pTextureCheck     = RESOURCES.Get<Texture>(L"UI_Setting_Check");
    m_pTextureNoCheck   = RESOURCES.Get<Texture>(L"UI_Setting_No_Check");

    if (true == Check_Expire())
        return E_FAIL;

    
    m_pBg.lock()->Get_Button()->AddOnClickedEvent([&]()
        {
            this->Set_Render(false);
        });




    return S_OK;
}

void UiSettingController::Tick()
{
	if (m_pOwner.expired())
		return;


}

void UiSettingController::Set_Render(_bool bValue)
{
    if (true == Check_Expire())
        return;

    m_pBg.lock()->Set_Render(bValue);
    m_pBrightness.lock()->Set_Render(bValue);
    m_pContrast.lock()->Set_Render(bValue);
    m_pSaturation.lock()->Set_Render(bValue);
    m_pBloom.lock()->Set_Render(bValue);
    m_pToneMapping.lock()->Set_Render(bValue);
    m_pToneMappingMax.lock()->Set_Render(bValue);
    m_pSSAO_Radius.lock()->Set_Render(bValue);
    m_pSSAO_Start.lock()->Set_Render(bValue);
    m_pSSAO_End.lock()->Set_Render(bValue);
    m_pMotionBlur.lock()->Set_Render(bValue);
    m_pPBR_Attenuation.lock()->Set_Render(bValue);
    m_pPBR_Ratio.lock()->Set_Render(bValue);
    m_pPBR_ShadowBius.lock()->Set_Render(bValue);
    m_pCheck_Bloom.lock()->Set_Render(bValue);
    m_pCheck_SSAO.lock()->Set_Render(bValue);
    m_pCheck_MotionBlur.lock()->Set_Render(bValue);
    m_pCheck_LensFlare.lock()->Set_Render(bValue);
    m_pCheck_Outline.lock()->Set_Render(bValue);
    m_pCheck_FXAA.lock()->Set_Render(bValue);
    m_pCheck_PBR.lock()->Set_Render(bValue);
}

_bool UiSettingController::Check_Expire()
{
    _bool bValue = false;

    bValue |= m_pBg.expired();
    bValue |= m_pBrightness.expired();
    bValue |= m_pContrast.expired();
    bValue |= m_pSaturation.expired();
    bValue |= m_pBloom.expired();
    bValue |= m_pToneMapping.expired();
    bValue |= m_pToneMappingMax.expired();
    bValue |= m_pSSAO_Radius.expired();
    bValue |= m_pSSAO_Start.expired();
    bValue |= m_pSSAO_End.expired();
    bValue |= m_pMotionBlur.expired();
    bValue |= m_pPBR_Attenuation.expired();
    bValue |= m_pPBR_Ratio.expired();
    bValue |= m_pPBR_ShadowBius.expired();
    bValue |= m_pCheck_Bloom.expired();
    bValue |= m_pCheck_SSAO.expired();
    bValue |= m_pCheck_MotionBlur.expired();
    bValue |= m_pCheck_LensFlare.expired();
    bValue |= m_pCheck_Outline.expired();
    bValue |= m_pCheck_FXAA.expired();
    bValue |= m_pCheck_PBR.expired();

    return bValue;
}
