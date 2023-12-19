#pragma once
#include "MonoBehaviour.h"
#include "pch.h"

class MapObjectLoopEffectScript :
    public MonoBehaviour
{
public:
    MapObjectLoopEffectScript(_float _fLoopDuration, wstring _wstrEffectGroup);
    virtual ~MapObjectLoopEffectScript() = default;

public:
    virtual void Tick() override;
    virtual shared_ptr<MonoBehaviour> Copy_Script() { return nullptr; }

public:
    void Add_GroupEffectOwner(const wstring& strSkilltag, _float3 vPosOffset, _bool usePosAs, shared_ptr<MonoBehaviour> pScript);

private:
    _float m_fLoopDuration = 0.f;
    _float m_fLoopDelta = 0.f;
    wstring m_wstrEffectGroup;
};