#pragma once
#include "MonoBehaviour.h"
class PortalScript :
    public MonoBehaviour
{
public:
    PortalScript(SCENE_TYPE eDest,const _float3& vPos);
    shared_ptr<MonoBehaviour> Copy_Script() override { return make_shared<PortalScript>(m_eDest,m_vStartingPos); }

public:
    virtual void Tick() override;
    

private:
    SCENE_TYPE m_eDest = SCENE_TYPE::FIELD;

    _bool m_bClicked = false;

    _float m_fAcc = 0.f;

    weak_ptr<GameObject> m_pEffectObj;
    _float3 m_vStartingPos = _float3(0.f);

};

