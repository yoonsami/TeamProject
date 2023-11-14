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
    void Change_To_Player();
    void Change_To_Spear_Ace();
    void Change_To_Kyle();
    void Change_To_Yeopo();
    void Change_To_Dellons();

};

