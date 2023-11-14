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

    void Click_Deck_Select(wstring strObjName);
    void Click_Deck_Inven(wstring strInvenName);
    void Click_Deck_X(wstring strObjName);

private:
    void Render_Off();

private:
    vector<weak_ptr<GameObject>> m_vecInvenObj;
    vector<weak_ptr<GameObject>> m_vecCardDeckObj;
    vector<weak_ptr<GameObject>> m_vecCardDeckBg;
    vector<weak_ptr<GameObject>> m_vecCardDeckElement;
    weak_ptr<GameObject>         m_pUiCharChange;

    _bool           m_bIsRender     = { false };
    _bool           m_bIsPicking    = { false };
    _uint           m_iPickingIndex = { 0 };
};

