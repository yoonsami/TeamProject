#pragma once
#include "MonoBehaviour.h"

class UiUseItemSlot : public MonoBehaviour
{
public:
    UiUseItemSlot();


public:
    virtual HRESULT Init() override;
    virtual shared_ptr<MonoBehaviour> Copy_Script() { return make_shared<UiUseItemSlot>(); }

    virtual void Tick() override;

    void Click_Slot();
    void Click_Inven(_uint iIndex);

    void Remove_Inven();

    void Use_Item();

private:
    void Create_Inven();

private:
    vector<weak_ptr<GameObject>> m_addedObj;

    weak_ptr<GameObject> m_pUseItem_Slot;
    weak_ptr<GameObject> m_pUseItem_Count;

    

    _bool   m_bIsSet = {};
    _bool   m_bIsCreated = {};

    _uint   m_iCount = {};
};

