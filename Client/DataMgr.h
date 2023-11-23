#pragma once

class DataMgr
{
    DECLARE_SINGLETON(DataMgr)

public:
    void Initialize();
    
    HERODATA    Get_Data(HERO eHero)                { return m_HeroData[IDX(eHero)]; }
    wstring     Get_Element(HERO eHero)             { return m_KeyElement[m_HeroData[IDX(eHero)].Element] + L"0"; }
    wstring     Get_Element_Line(HERO eHero)        { return m_KeyElement[m_HeroData[IDX(eHero)].Element] + L"1"; }

    BOSSDATA    Get_Data(BOSS eBoss)                { return m_BossData[IDX(eBoss)]; }
    wstring     Get_Element(BOSS eBoss)             { return m_KeyElement[m_BossData[IDX(eBoss)].Element] + L"0"; }
    wstring     Get_Element_Line(BOSS eBoss)        { return m_KeyElement[m_BossData[IDX(eBoss)].Element] + L"1"; }

private:
    vector<HERODATA>    m_HeroData;
    vector<wstring>     m_KeyElement;

    vector<BOSSDATA>    m_BossData;
};

