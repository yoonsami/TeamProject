#pragma once
#include "MonoBehaviour.h"

class UiCardDeckController : public MonoBehaviour
{
public:
    UiCardDeckController();

public:
    virtual HRESULT Init() override;
    virtual void Tick() override;

    void Render_On();

private:
    void Render_Off();

private:
    vector<wstring>     m_vecObjTag;
    _bool               m_bIsRender = { false };
};

