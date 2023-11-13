#pragma once
#include "MonoBehaviour.h"

class UiCardDeckInvenChange : public MonoBehaviour
{
public:
    UiCardDeckInvenChange();

public:
    virtual HRESULT Init() override;
    virtual void Tick() override;

    void Set_Hero(HERO eHero);
    void Set_Empty();

private:


private:
   

};

