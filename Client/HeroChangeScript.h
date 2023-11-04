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
    weak_ptr<GameObject> m_pPlayer;
};

