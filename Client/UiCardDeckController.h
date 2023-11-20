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
    void Render_Off();

private:

private:
    vector<weak_ptr<GameObject>> m_vecInvenObj;
    vector<weak_ptr<GameObject>> m_vecCardDeckObj;
    vector<weak_ptr<GameObject>> m_vecCardDeckBg;
    vector<weak_ptr<GameObject>> m_vecCardDeckElement;
    vector<weak_ptr<GameObject>> m_vecFont;
    
    weak_ptr<GameObject>         m_pUiCharChange;
    weak_ptr<GameObject>         m_pUiCardDeckSelect;

    _bool           m_bIsRender     = { false };
    _bool           m_bIsPicking    = { false };
    _uint           m_iPickingIndex = { 0 };
};

