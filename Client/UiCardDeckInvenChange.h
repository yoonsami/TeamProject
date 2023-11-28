#pragma once
#include "MonoBehaviour.h"

class UiCardDeckInvenChange : public MonoBehaviour
{
public:
    UiCardDeckInvenChange(_uint iType);

public:
    virtual HRESULT Init() override;
    virtual void Tick() override;

    HERO  Get_Hero()    { return m_eHero; }
    _bool Is_Set_Hero() { return m_bIsSetHero; }

    void Set_Hero(HERO eHero);
    void Set_Hero(DUMMY_HERO eHero);
    void Set_Empty();

private:


private:
    _uint   m_iType         = { 0 };
    _bool   m_bIsSetHero    = { false };
    HERO    m_eHero         = { HERO::MAX };
    DUMMY_HERO m_eDummy     = { DUMMY_HERO::MAX };
};

