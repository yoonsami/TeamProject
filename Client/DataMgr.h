#pragma once

class DataMgr
{
    DECLARE_SINGLETON(DataMgr)

public:
    void Initialize();
    
    HERODATA    Get_Data(HERO eHero)                { return m_HeroData[IDX(eHero)]; }
    wstring     Get_Element(HERO eHero)             { return m_KeyElement[m_HeroData[IDX(eHero)].Element] + L"0"; }
    wstring     Get_Element_Line(HERO eHero)        { return m_KeyElement[m_HeroData[IDX(eHero)].Element] + L"1"; }

private:
    vector<HERODATA>    m_HeroData;
    vector<wstring>     m_KeyElement;
};

