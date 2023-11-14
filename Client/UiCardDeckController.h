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
    void Set_Hero(HERO eHero);

private:
    void Render_Off();

private:
    vector<weak_ptr<GameObject>> m_vecInvenObj;
    vector<weak_ptr<GameObject>> m_vecCardDeckObj;
    vector<weak_ptr<GameObject>> m_vecCardDeckBg;
    vector<weak_ptr<GameObject>> m_vecCardDeckElement;


    _bool               m_bIsRender = { false };
};

