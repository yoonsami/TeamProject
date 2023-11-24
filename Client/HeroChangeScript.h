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
    virtual void Tick() override;
    
    void Change_Hero(HERO eHero);

private:
    void Add_Character_Weapon(const wstring& weaponname);
    void Change_To_Input(HERO eHero);

};

