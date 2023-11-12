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
    void Delete_Gacha_Card();
    void Delete_Gacha_Bg();

private:
    

private:
    vector<wstring>     m_vecObjTag;
    vector<wstring>     m_vecObjEffectTag;
    vector<wstring>     m_vecObjBgTag;
};

