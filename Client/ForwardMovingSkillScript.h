#pragma once
#include "MonoBehaviour.h"
class ForwardMovingSkillScript : public MonoBehaviour
{
public:
    ForwardMovingSkillScript(const _float4& vLook, _float fMoveSpeed, _float fLifeTime, _float fLimitDistance = 0.f);
    ~ForwardMovingSkillScript();

public:

    virtual HRESULT Init() override;
    virtual void Tick() override;

private:
    _float4 m_vLook = { 0.f,0.f,0.f,0.f };
    _float m_fMoveSpeed = 1.f;
    COOLTIMEINFO m_tLifeTime = { 2.f,0.f };
    _float m_fLimitDistance = 0.f;
    _float4 m_vFirstPos = { 0.f,0.f,0.f,1.f };

};

