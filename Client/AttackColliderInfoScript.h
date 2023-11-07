#pragma once
#include "MonoBehaviour.h"
class AttackColliderInfoScript : public MonoBehaviour
{

public:
    virtual void Tick() override;

public:
    void Set_SkillName(const wstring& name) { m_strSkillName = name; }
    const wstring& Get_SkillName() { return m_strSkillName; }

private:
    wstring m_strSkillName;
};



