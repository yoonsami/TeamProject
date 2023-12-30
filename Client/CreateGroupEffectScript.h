#pragma once
#include "MonoBehaviour.h"
class CreateGroupEffectScript : public MonoBehaviour
{
public:
    virtual HRESULT Init() override;
    virtual void Tick() override;
    virtual shared_ptr<MonoBehaviour> Copy_Script() { return make_shared<CreateGroupEffectScript>(); }

    void    Add_GroupEffectOwner(const wstring& strSkilltag, _float3 vPosOffset, _bool usePosAs, shared_ptr<MonoBehaviour> pScript, _bool bTimeSlowed);

    void    Set_EffectTag(const wstring& wstrEffectTag) { m_wstrEffectTag = wstrEffectTag; }

private:
    wstring m_wstrEffectTag;
    _bool   m_bIsAvailable = true;
    _float  m_fHintEffectTimer = { 0.f };
};

