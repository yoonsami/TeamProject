#pragma once

class DataMgr
{
    DECLARE_SINGLETON(DataMgr)

public:
    void Initialize();
    
    const HERODATA Get_Data(HERO eHero) { return m_HeroData[IDX(eHero)]; }

private:
    vector<HERODATA> m_HeroData;

};

