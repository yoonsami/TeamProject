#pragma once
#include "MonoBehaviour.h"
class HeroChangeScript : public MonoBehaviour
{
public:
    HeroChangeScript(shared_ptr<GameObject> pPlayer);
    ~HeroChangeScript();

public:
    virtual void Tick() override;

private:
    void Add_Character_Weapon(const wstring& weaponname);

private:
    weak_ptr<GameObject> m_pPlayer;
};

