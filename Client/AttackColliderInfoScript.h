#pragma once
#include "MonoBehaviour.h"
class AttackColliderInfoScript : public MonoBehaviour
{

public:
    virtual void Tick() override;


public:
    void Set_SkillName(const wstring& name) { m_strSkillName = name; }
    void Set_ColliderOwner(shared_ptr<GameObject> pColliderOwner);
    
    const wstring& Get_SkillName() { return m_strSkillName; }
    shared_ptr<GameObject> Get_ColliderOwner() { return m_pColliderOwner.lock(); }

private:
    wstring m_strSkillName;
    weak_ptr<GameObject> m_pColliderOwner;
};



