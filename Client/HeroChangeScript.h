#pragma once
#include "MonoBehaviour.h"

class FSM;
class ModelAnimator;

class HeroChangeScript : public MonoBehaviour
{
public:
    HeroChangeScript();
    ~HeroChangeScript();

public:
    virtual HRESULT Init() override;
    virtual void Final_Tick() override;
    
    void Change_Hero(HERO eHero);

private:
    void Add_Character_Weapon(const wstring& weaponname);
    void Change_To_Player();
    void Change_To_Spear_Ace();
    void Change_To_Kyle();
    void Change_To_Yeopo();
    void Change_To_Dellons();
    void Change_To_Spike();
    void Change_To_Yeonhee();
    void Change_To_Shane();
    void Change_To_Input(HERO eHero);

};

