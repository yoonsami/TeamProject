#pragma once

class DataMgr
{
    DECLARE_SINGLETON(DataMgr)

public:
    void Initialize();
    
    const HERODATA&     Get_Data(HERO eHero)                                { return m_HeroData[IDX(eHero)]; }
    wstring             Get_Element(HERO eHero)                             { return m_KeyElement[m_HeroData[IDX(eHero)].Element] + L"0"; }
    wstring             Get_Element_Line(HERO eHero)                        { return m_KeyElement[m_HeroData[IDX(eHero)].Element] + L"1"; }

    const BOSSDATA&     Get_Data(BOSS eBoss)                                { return m_BossData[IDX(eBoss)]; }
    wstring             Get_Element(BOSS eBoss)                             { return m_KeyElement[m_BossData[IDX(eBoss)].Element] + L"0"; }
    wstring             Get_Element_Line(BOSS eBoss)                        { return m_KeyElement[m_BossData[IDX(eBoss)].Element] + L"1"; }

    const wstring&      Get_Npc_Name(NPCTYPE eType)                         { return m_NpcName[IDX(eType)]; }
    
    const wstring&      Get_Dialog(QUESTINDEX eIndex, _bool bHaveQuest, _bool bIsClear, _uint iIndex);
    const QUESTDATA&    Get_Quest(QUESTINDEX eIndex)                        { return m_Quest[IDX(eIndex)]; }


private:
    vector<HERODATA>            m_HeroData;
    vector<wstring>             m_KeyElement;
    vector<BOSSDATA>            m_BossData;
    vector<wstring>             m_NpcName;

    vector<wstring>             m_DialogGameClear;
    vector<vector<wstring>>     m_DialogAccept;
    vector<vector<wstring>>     m_DialogNoClear;
    vector<vector<wstring>>     m_DialogClear;
    vector<QUESTDATA>           m_Quest;

    wstring                     strTemp;
};

