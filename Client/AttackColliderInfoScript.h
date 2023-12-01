#pragma once
#include "MonoBehaviour.h"
class AttackColliderInfoScript : public MonoBehaviour
{

public:
    virtual void Tick() override;
	virtual shared_ptr<MonoBehaviour> Copy_Script()override { return make_shared<AttackColliderInfoScript>(); }


public:
    void Set_SkillName(const wstring& name) { m_strSkillName = name; }
    void Set_ColliderOwner(shared_ptr<GameObject> pColliderOwner);
    void Set_AttackDamage(_float fAttackDamage) { m_fAttackDamage = fAttackDamage; }

    const wstring& Get_SkillName() { return m_strSkillName; }
    _float Get_AttackDamage() { return m_fAttackDamage; }
    shared_ptr<GameObject> Get_ColliderOwner() { return m_pColliderOwner.lock(); }

private:
    wstring m_strSkillName = L"";
    _float m_fAttackDamage = 0.f;
    weak_ptr<GameObject> m_pColliderOwner;
};



