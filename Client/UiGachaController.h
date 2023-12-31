#pragma once
#include "MonoBehaviour.h"

class UiGachaController : public MonoBehaviour
{
public:
    UiGachaController();

public:
    virtual HRESULT Init() override;
    virtual void Tick() override;

    void Create_Gacha_Card();
    void Open_All_Card();
    void Delete_All();

private:
    void Delete_Gacha_Card();
    void Delete_Gacha_Bg();
    void Delete_Gacha_Button();
    
    void Start_All_Open();

private:
    vector<wstring>     m_vecObjTag;
    vector<wstring>     m_vecObjEffectTag;
    vector<wstring>     m_vecObjBgTag;
    vector<wstring>     m_vecObjButton;

    _bool   m_bIsCreate     = { false };
    _bool   m_bIsStartOpen  = { false };
    _uint   m_iSize         = {};
    _uint   m_iIndex        = {};
    _float  m_fCheckTime    = {};
    _float  m_fMaxTime      = {};
};

