#pragma once
#include "MonoBehaviour.h"

class UIShop : public MonoBehaviour
{
public:
    UIShop();

public:
    virtual HRESULT Init() override;
    virtual void Tick() override;
	virtual shared_ptr<MonoBehaviour> Copy_Script() { return make_shared<UIShop>(); }

    void Create_Shop();
    void Remove_Shop();

    void Click_Item(_uint iIndex);

    void Click_Buy_Yes();
    void Click_Buy_No();

private:
    void Create_Buy_Ui();
    void Remove_Buy_Ui();

    void Check_Scroll();

private:
    vector<weak_ptr<GameObject>>    m_addedObj;
    vector<weak_ptr<GameObject>>    m_ItemObj;
    vector<weak_ptr<GameObject>>    m_ItemName;
    vector<weak_ptr<GameObject>>    m_ItemPrice;
    vector<weak_ptr<GameObject>>    m_addedBuyUi;

    weak_ptr<GameObject>            m_pScroll;

    weak_ptr<GameObject> m_pMoneyValue;

    _bool           m_bIsCreated            = {};
    _bool           m_bIsCreatedBuyUi       = {};
                    
    _uint           m_iInteractionItemIndex = {};

    vector<_bool>   m_vecIsBuy;
    vector<_uint>   m_vecPrice;
    vector<wstring> m_vecName;

    _uint           m_iMoney = {};
    _float          m_fMaxPosX = {};
};

