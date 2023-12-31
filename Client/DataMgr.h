#pragma once

class DataMgr
{
    DECLARE_SINGLETON(DataMgr)

public:
    void Initialize();
    
    HERODATA    Get_Data(HERO eHero)            { return m_HeroData[IDX(eHero)]; }
    wstring     Get_Element(HERO eHero)         { return m_KeyElement[m_HeroData[IDX(eHero)].Element] + L"0"; }
    wstring     Get_Element_Line(HERO eHero)    { return m_KeyElement[m_HeroData[IDX(eHero)].Element] + L"1"; }

    BOSSDATA    Get_Data(BOSS eBoss)            { return m_BossData[IDX(eBoss)]; }
    wstring     Get_Element(BOSS eBoss)         { return m_KeyElement[m_BossData[IDX(eBoss)].Element] + L"0"; }
    wstring     Get_Element_Line(BOSS eBoss)    { return m_KeyElement[m_BossData[IDX(eBoss)].Element] + L"1"; }

    wstring     Get_Npc_Name(NPCTYPE eType)                         { return m_NpcName[IDX(eType)]; }
    _uint       Get_Quest_Dialog_Count(QUESTDIALOG eType)           { return IDX(m_QuestDialog.size()); }
    wstring     Get_Quest_Dialog(QUESTDIALOG eType, _uint iIndex)   { return m_QuestDialog[IDX(eType)][iIndex]; }

private:
    vector<HERODATA>            m_HeroData;
    vector<wstring>             m_KeyElement;
    vector<BOSSDATA>            m_BossData;
    vector<wstring>             m_NpcName;
    vector<vector<wstring>>     m_QuestDialog;
};

