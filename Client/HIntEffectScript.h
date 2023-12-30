#pragma once
#include "MonoBehaviour.h"
class HIntEffectScript : public MonoBehaviour
{
public:
    virtual HRESULT Init() override;
    virtual void Tick() override;
    virtual shared_ptr<MonoBehaviour> Copy_Script() { return make_shared<HIntEffectScript>(); }

private:
    _float  m_fHintEffectTimer[21];
    _bool   m_bIsActivated[21];
};

