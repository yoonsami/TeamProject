#pragma once
#include "MonoBehaviour.h"
class PortalScript :
    public MonoBehaviour
{
public:
    PortalScript(SCENE_TYPE eDest);
    shared_ptr<MonoBehaviour> Copy_Script() override { return make_shared<PortalScript>(m_eDest); }

public:
    virtual void Tick() override;


private:
    SCENE_TYPE m_eDest = SCENE_TYPE::FIELD;

    _bool m_bClicked = false;

    _float m_fAcc = 0.f;

    weak_ptr<GameObject> m_pEffectObj;

};

