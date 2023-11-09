#pragma once
#include "MonoBehaviour.h"

class CoolTimeCheckScript : public MonoBehaviour
{
public:
    enum class HERO { ACE, KYLE, YEOPO, DELLONS, MAX};

public:
    CoolTimeCheckScript();

public:
    virtual HRESULT Init() override;
    virtual void Tick() override;

    void Set_Cur_Hero(HERO eType);
    void Set_Skill_End();
    _bool IsAvailable(_uint iSkillIndex);
    _bool GetIsWork() { return m_bIsSkillWork; }
    void Start_Attack_Button_Effect();

private:
    void Check_Cool_Time();
    void Change_Skills_Value();
    void Start_Effect(_uint iIndex);

private:
    vector<vector<COOLTIMEINFO>>    m_CoolTime;
    vector<vector<wstring>>         m_TextureKey;
    
    COOLTIMEINFO m_tagEvade     = {};

    HERO    m_eCurHero          = { HERO::MAX };
    _uint   m_iWorkSkillIndex   = { 0 };
    _bool   m_bIsSkillWork      = { false };
};

