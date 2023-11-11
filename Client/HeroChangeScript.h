#pragma once
#include "MonoBehaviour.h"
class HeroChangeScript : public MonoBehaviour
{
public:
    HeroChangeScript();
    ~HeroChangeScript();

public:
    virtual void Tick() override;

private:
    void Add_Character_Weapon(const wstring& weaponname);


};

