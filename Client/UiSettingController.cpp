#include "pch.h"
#include "UiSettingController.h" 

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
    m_pPBR_Attenuation  = pScene->Get_UI(L"UI_Setting_Star_PBR_Attenuation");
    m_pPBR_Ratio        = pScene->Get_UI(L"UI_Setting_Star_PBR_Ratio");
    m_pPBR_ShadowBius   = pScene->Get_UI(L"UI_Setting_Star_PBR_ShadowBius");
    m_pCheck_Bloom      = pScene->Get_UI(L"UI_Setting_Check_Bloom");
    m_pCheck_SSAO       = pScene->Get_UI(L"UI_Setting_Check_SSAO");
    m_pCheck_MotionBlur = pScene->Get_UI(L"UI_Setting_Check_MotionBlur");
    m_pCheck_LensFlare  = pScene->Get_UI(L"UI_Setting_Check_LensFlare");
    m_pCheck_Outline    = pScene->Get_UI(L"UI_Setting_Check_Outline");
    m_pCheck_FXAA       = pScene->Get_UI(L"UI_Setting_Check_FXAA");
    m_pCheck_PBR        = pScene->Get_UI(L"UI_Setting_Check_PBR");

    m_pTextureCheck     = RESOURCES.Get<Texture>(L"UI_Setting_Check");
    m_pTextureNoCheck   = RESOURCES.Get<Texture>(L"UI_Setting_No_Check");

    return S_OK;
}

void UiSettingController::Tick()
{
	if (m_pOwner.expired())
		return;


}
