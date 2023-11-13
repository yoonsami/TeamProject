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
    vector<weak_ptr<GameObject>> m_vecInvenObj;
    vector<weak_ptr<GameObject>> m_vecCardDeckObj;
    _bool               m_bIsRender = { false };
};

