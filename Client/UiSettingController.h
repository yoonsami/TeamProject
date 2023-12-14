#pragma once
#include "MonoBehaviour.h"

class UiSettingController : public MonoBehaviour
{
public:
    UiSettingController();


public:
    virtual HRESULT Init() override;
    virtual void Tick() override;
	virtual shared_ptr<MonoBehaviour> Copy_Script()override { return make_shared<UiSettingController>(); }

    //void Set_Render(_bool bValue);
    
    void Change_Bloom();
    void Change_SSAO();
    void Change_MotionBlur();
    void Change_LensFlare();
    void Change_Outline();
    void Change_FXAA();
    void Change_PBR();
    void Change_FPS();

    void Create_Setting_Ui();
    void Remove_Setting_Ui();

private:
    _bool Check_Expire();
    void Change_All_Value();
    
    void Change_Value_Brightness();
    void Change_Value_Contrast();
    void Change_Value_Saturation();
    void Change_Value_Bloom();
    void Change_Value_ToneMappingIndex();
    void Change_Value_ToneMappingMaxWhite();
    void Change_Value_SSAO_Radius();
    void Change_Value_SSAO_Start();
    void Change_Value_SSAO_End();
    void Change_Value_MotionBlur();

    void Change_FPS_Font();
    
private:
    //_bool                   m_bIsRender         = {};
     
    //weak_ptr<GameObject>    m_pBg;
    //weak_ptr<GameObject>    m_pBrightness;
    //weak_ptr<GameObject>    m_pContrast;
    //weak_ptr<GameObject>    m_pSaturation;
    //weak_ptr<GameObject>    m_pBloom;
    //weak_ptr<GameObject>    m_pToneMapping;
    //weak_ptr<GameObject>    m_pToneMappingMax;
    //weak_ptr<GameObject>    m_pSSAO_Radius;
    //weak_ptr<GameObject>    m_pSSAO_Start;
    //weak_ptr<GameObject>    m_pSSAO_End;
    //weak_ptr<GameObject>    m_pMotionBlur;
    //
    //weak_ptr<GameObject>    m_pCheck_Bloom;
    //weak_ptr<GameObject>    m_pCheck_SSAO;
    //weak_ptr<GameObject>    m_pCheck_MotionBlur;
    //weak_ptr<GameObject>    m_pCheck_LensFlare;
    //weak_ptr<GameObject>    m_pCheck_Outline;
    //weak_ptr<GameObject>    m_pCheck_FXAA;
    //weak_ptr<GameObject>    m_pCheck_PBR;
    //weak_ptr<GameObject>    m_pCheck_FPS;

    weak_ptr<GameObject>    m_pFPS_Font;

    weak_ptr<Texture>       m_pTextureCheck;
    weak_ptr<Texture>       m_pTextureNoCheck;

    _bool                   m_bBloom            = {};
    _bool                   m_bSSAO             = {};
    _bool                   m_bMotionBlur       = {};
    _bool                   m_bLensFlare        = {};
    _bool                   m_bOutline          = {};
    _bool                   m_bFXAA             = {};
    _bool                   m_bPBR              = {};
    _bool                   m_bFPS              = {};

    pair<_float, _float>    m_Brightness        = {};
    pair<_float, _float>    m_Contrast          = {};
    pair<_float, _float>    m_Saturation        = {};
    pair<_float, _float>    m_Bloom             = {};
    pair<_uint, _uint>      m_ToneMappingIndex  = {};
    pair<_float, _float>    m_White             = {};
    pair<_float, _float>    m_SSAORadius        = {};
    pair<_float, _float>    m_SSAOStart         = {};
    pair<_float, _float>    m_SSAOEnd           = {};
    pair<_uint, _uint>      m_MotionBlurCount   = {};

    _float                  m_fMinPos           = {};

    _bool                   m_bIsCreated        = {};
    vector<weak_ptr<GameObject>> m_addedObj;

    _uint   m_iCheckBloomIndex                  = {};
    _uint   m_iCheckSSAOIndex                   = {};
    _uint   m_iCheckMotionBlurIndex             = {};
    _uint   m_iCheckLensFlareIndex              = {};
    _uint   m_iCheckOutlineIndex                = {};
    _uint   m_iCheckFXAAIndex                   = {};
    _uint   m_iCheckPBRIndex                    = {};
    _uint   m_iCheckFPSIndex                    = {};

    _uint   m_iStarBrightness                   = {};
    _uint   m_iStarContrast                     = {};
    _uint   m_iStarSaturation                   = {};
    _uint   m_iStarBloom                        = {};
    _uint   m_iStarToneMapping                  = {};
    _uint   m_iStarToneMappingMax               = {};
    _uint   m_iStarSSAO_Radius                  = {};
    _uint   m_iStarSSAO_Start                   = {};
    _uint   m_iStarSSAO_End                     = {};
    _uint   m_iStarMotionBlur                   = {};
};

