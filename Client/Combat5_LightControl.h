#pragma once
#include "MonoBehaviour.h"
class Combat5_LightControl :
    public MonoBehaviour
{
public:
    Combat5_LightControl(shared_ptr<GameObject> pPlayer) : m_pPlayer(pPlayer) {}


    virtual shared_ptr<MonoBehaviour> Copy_Script() override { return make_shared<Combat5_LightControl>(m_pPlayer.lock()); }

    virtual void Tick() override;


private:
    weak_ptr<GameObject> m_pPlayer;

};

