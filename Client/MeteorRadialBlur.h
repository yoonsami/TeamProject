#pragma once
#include "MonoBehaviour.h"
class MeteorRadialBlur :
    public MonoBehaviour
{
public:
    MeteorRadialBlur(_float fExploseTime, _float fBiggerSpeed, _float fMaintainTime);
    shared_ptr<MonoBehaviour> Copy_Script() override { return make_shared<MeteorRadialBlur>(m_fExploseTime,m_fBiggerSpeed,m_fEndTime); }

    virtual void Tick() override;

private:
    _float m_fExploseTime = 0.f;
    _float m_fAcc = 0.f;

    _float m_fEndTime = 0.f;

    _float m_fBiggerSpeed = 0.f;

    _bool m_bMeteorAttack = false;
    _bool m_bSoundRecovery = false;

    Color m_vOriginAmbientColor =Color(0.f);
    Color m_vOriginDiffuseColor =Color(0.f);

	Color m_vTargetAmbientColor = Color(0.f);
	Color m_vTargetDiffuseColor = Color(0.f);

    //SoundInfo
    _float m_fCurBgmRatio = 0.f;
    _float m_fCurEnvironmentRatio = 0.f;
    _float m_fCurCharacterVoiceRatio = 0.f;
    _float m_fCurCharacterEffectRatio = 0.f;
    _float m_fCurMonsterVoiceRatio = 0.f;
    _float m_fCurMonsterEffectRatio = 0.f;
    _float m_fCurSystemSoundRatio = 0.f;

    _float m_fSoundRatio = 0.f;


private:
    _float SoundVolumeLerp(_float fStart, _float fEnd, _float fRatio);
};

