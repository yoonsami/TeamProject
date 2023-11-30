#pragma once
#include "MonoBehaviour.h"

class UiCardDeckController : public MonoBehaviour
{
public:
    UiCardDeckController();

public:
    virtual HRESULT Init() override;

    void Set_Render(_bool bValue);

    void Click_Deck_Select(wstring strObjName);
    void Click_Deck_Inven(wstring strInvenName);
    void Click_Deck_X(wstring strObjName);

private:
    void Set_Card();

private:
    vector<weak_ptr<GameObject>> m_vecCardDeckObj;
    vector<weak_ptr<GameObject>> m_vecInvenObj;
    vector<weak_ptr<GameObject>> m_vecCardDeckBg;
    vector<weak_ptr<GameObject>> m_vecCardDeckElement;
    vector<weak_ptr<GameObject>> m_vecFont;
    
    _bool           m_bIsRender     = { false };
    _uint           m_iPickingIndex = { 0 };
    _uint           m_iPreSize      = {};
};

