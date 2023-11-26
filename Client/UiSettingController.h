#pragma once
#include "MonoBehaviour.h"

class UiSettingController : public MonoBehaviour
{
public:
    UiSettingController();


public:
    virtual HRESULT Init() override;
    virtual void Tick() override;


private:


public:
    

private:
    weak_ptr<GameObject>    m_pBg;
    weak_ptr<GameObject>    m_pBrightness;
    weak_ptr<GameObject>    m_pContrast;
    weak_ptr<GameObject>    m_pSaturation;
    weak_ptr<GameObject>    m_pBloom;
    weak_ptr<GameObject>    m_pToneMapping;
    weak_ptr<GameObject>    m_pToneMappingMax;
    weak_ptr<GameObject>    m_pSSAO_Radius;
    weak_ptr<GameObject>    m_pSSAO_Start;
    weak_ptr<GameObject>    m_pSSAO_End;
    weak_ptr<GameObject>    m_pMotionBlur;
    weak_ptr<GameObject>    m_pPBR_Attenuation;
    weak_ptr<GameObject>    m_pPBR_Ratio;
    weak_ptr<GameObject>    m_pPBR_ShadowBius;

    weak_ptr<GameObject>    m_pCheck_Bloom;
    weak_ptr<GameObject>    m_pCheck_SSAO;
    weak_ptr<GameObject>    m_pCheck_MotionBlur;
    weak_ptr<GameObject>    m_pCheck_LensFlare;
    weak_ptr<GameObject>    m_pCheck_Outline;
    weak_ptr<GameObject>    m_pCheck_FXAA;
    weak_ptr<GameObject>    m_pCheck_PBR;

    weak_ptr<Texture>       m_pTextureCheck;
    weak_ptr<Texture>       m_pTextureNoCheck;

};

