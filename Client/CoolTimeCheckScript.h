#pragma once
#include "MonoBehaviour.h"

class CoolTimeCheckScript : public MonoBehaviour
{
public:
    CoolTimeCheckScript();

public:
    virtual HRESULT Init() override;
    virtual void Tick() override;
    
    void Start_Attack_Button_Effect();

    _bool GetIsWork() { return m_bIsSkillWork; }
    _bool IsAvailable(SkillType eSkillType);

    void Set_Skill_End();
    void Set_Cur_Hero(HERO eType);

private:
    void Check_Cool_Time();
    void Change_Skills_Value();
    void Start_Effect(SkillType eSkillType);
    void Start_ButtonEndEffect(_uint iIndex);

private:
    vector<vector<COOLTIMEINFO>>    m_CoolTime;
    //vector<vector<wstring>>         m_TextureKey;
    vector<weak_ptr<GameObject>>    m_pUiSkill;
    vector<weak_ptr<GameObject>>    m_pUiSkill_Effect;
    vector<weak_ptr<GameObject>>    m_pUi_Cool_End;

    COOLTIMEINFO m_tagEvade     = {};

    HERO    m_eCurHero          = { HERO::MAX };
    _uint   m_iWorkSkillIndex   = { 0 };
    _bool   m_bIsSkillWork      = { false };
};

