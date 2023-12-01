#pragma once
#include "MonoBehaviour.h"

class UiCardDeckController : public MonoBehaviour
{
public:
    UiCardDeckController();

public:
    virtual HRESULT Init() override;
    virtual void Tick() override;
	virtual shared_ptr<MonoBehaviour> Copy_Script() { return make_shared<UiCardDeckController>(); }

    void Render_On();
    void Set_Hero(HERO eHero);
    void Set_Hero(DUMMY_HERO eHero);

    void Click_Deck_Select(wstring strObjName);
    void Click_Deck_Inven(wstring strInvenName);
    void Click_Deck_X(wstring strObjName);
    void Render_Off();

private:
    void Select_Down();
    void Select_Up();

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
    
    enum class SELECT_STATE { DOWN, UP};
    SELECT_STATE    m_eState        = { SELECT_STATE::DOWN };
    _float          m_fCheckTime    = {};
    _float          m_fMaxTime      = {};
    _float          m_fSpeed        = {};
};

