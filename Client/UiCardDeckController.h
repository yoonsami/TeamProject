#pragma once
#include "MonoBehaviour.h"

class UiCardDeckController : public MonoBehaviour
{
public:
    UiCardDeckController();

public:
    virtual HRESULT Init() override;
    virtual void Tick() override;

    void Set_Render(_bool bValue);

    void Click_Deck_Select(wstring strObjName);
    void Click_Deck_Inven(wstring strInvenName);
    void Click_Deck_X(wstring strObjName);
    void Click_Info_Set(_uint iIndex);

    void Remove_Info();

private:
    void Set_Card();
    void Create_Switch_Complete(_bool bValue);

    void Create_Info(_uint iIndex);

private:
    weak_ptr<GameObject>            m_pCharChange;

    vector<weak_ptr<GameObject>>    m_vecCardDeckObj;
    vector<weak_ptr<GameObject>>    m_vecInvenObj;
    vector<weak_ptr<GameObject>>    m_vecCardDeckBg;
    vector<weak_ptr<GameObject>>    m_vecCardDeckElement;
    vector<weak_ptr<GameObject>>    m_vecFont;
    
    _bool           m_bIsRender     = { false };
    _uint           m_iPickingIndex = { 0 };
    _uint           m_iPreSize      = {};

    _bool           m_bIsInfoCreate = {};
    vector<weak_ptr<GameObject>> m_vecAddedObj;

    _bool           m_bIsClickSet = {};
    _uint           m_iSetIndex = {};
};

