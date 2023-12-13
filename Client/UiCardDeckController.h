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

    //void Set_Render(_bool bValue);

    void Click_Deck_Select(wstring strObjName, _uint iIndex);
    void Click_Deck_Inven(_uint iIndex);
    void Click_Deck_X(wstring strObjName, _uint iIndex);
    void Click_Info_Set(_uint iIndex);

    void Remove_Info();

    void Create_Card_Deck();
    void Remove_Card_Deck();

private:
    void Set_Card();
    void Create_Switch_Complete(_bool bValue);

    void Create_Info(_uint iIndex);
    void Set_Font(_uint iIndex);
    void Remove_Font(_uint iIndex);

    void Create_Select_Mark();
    void Remove_Select_Mark();
    void Move_Select_Mark();

    void Check_Scroll();

private:
    weak_ptr<GameObject>            m_pCharChange;

    vector<weak_ptr<GameObject>>    m_vecCardDeckObj;
    vector<weak_ptr<GameObject>>    m_vecInvenObj;
    //vector<weak_ptr<GameObject>>    m_vecCardDeckX;
    //vector<weak_ptr<GameObject>>    m_vecFont;
    
    vector<weak_ptr<GameObject>>    m_vecAddedObj;
    vector<weak_ptr<GameObject>>    m_vecSelectMark;

    //_bool       m_bIsRender             = { false };
    _uint       m_iPickingIndex         = { 0 };

    _bool       m_bIsInfoCreate         = {};

    _bool       m_bIsClickSet           = {};
    _uint       m_iSetIndex             = {};

    _bool       m_bIsSelectMarkCreate   = {};
    
    _bool       m_bIsMoveDown           = {};
    _float      m_fCheckTime            = {};
    _float      m_fMaxTime              = {};
    _float      m_fSpeed                = {};

    // 
    weak_ptr<GameObject> m_pScroll;
    _bool       m_bIsCreated            = {};
    _float      m_fMaxPosY              = {};
};

