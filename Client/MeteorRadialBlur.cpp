#include "pch.h"
#include "MeteorRadialBlur.h"
#include "Camera.h"
#include "Light.h"
#include "FSM.h"

MeteorRadialBlur::MeteorRadialBlur(_float fExploseTime, _float fBiggerSpeed, _float fMaintainTime)
	: m_fExploseTime(fExploseTime), m_fBiggerSpeed(fBiggerSpeed), m_fEndTime(fMaintainTime + fExploseTime)
{
	if (CUR_SCENE->Get_Light())
	{
		auto& lightColor = CUR_SCENE->Get_Light()->Get_Light()->Get_LightInfo().color;
		m_vOriginAmbientColor = lightColor.ambient;
		m_vOriginDiffuseColor = lightColor.diffuse;

		m_vTargetAmbientColor = m_vOriginAmbientColor * 0.1f;
		m_vTargetDiffuseColor = m_vOriginDiffuseColor * 0.1f;
	}
}

void MeteorRadialBlur::Tick()
{
	const _float4x4& matView = CUR_SCENE->Get_MainCamera()->Get_Camera()->Get_ViewMat();
	const _float4x4& matProj = CUR_SCENE->Get_MainCamera()->Get_Camera()->Get_ProjMat();

	_float3 vCenterPos = _float3::Transform(Get_Transform()->Get_State(Transform_State::POS).xyz(), matView * matProj);
	CUR_SCENE->g_RadialBlurData.g_vCenterPos = { vCenterPos.x,vCenterPos.y };

	m_fAcc += fDT;

	auto& lightColor = CUR_SCENE->Get_Light()->Get_Light()->Get_LightInfo().color;
	Color& curAmbientColor = lightColor.ambient;
	Color& curDiffuseColor = lightColor.diffuse;

	_float fTimeOffset = 1.f;

	if (m_fAcc <= m_fExploseTime - fTimeOffset)
	{
		Color targetAmbientColor = m_vTargetAmbientColor;
		Color targetDiffuseColor = m_vTargetDiffuseColor;

		curAmbientColor = _float4::Lerp(m_vOriginAmbientColor, targetAmbientColor, m_fAcc / (m_fExploseTime - fTimeOffset));
		curDiffuseColor = _float4::Lerp(m_vOriginDiffuseColor, targetDiffuseColor, m_fAcc / (m_fExploseTime - fTimeOffset));
		SWITCHMGR.Set_SwitchState(SWITCH_TYPE::YEONHEE_300100_LIGHT_OFF, true);
	}

	if (m_fAcc >= m_fExploseTime - 0.3f)
	{
		if (!m_bMeteorAttack)
		{
			m_bMeteorAttack = true;

			vector<shared_ptr<GameObject>> targetMonster;

			for (auto& obj : CUR_SCENE->Get_Objects())
			{
				if (obj->Get_ObjectGroup() != OBJ_MONSTER)
					continue;

				if (!obj->Get_FSM())
					continue;

				obj->Get_FSM()->Get_Hit(KNOCKDOWN_ATTACK, 99999.f, Get_Owner(), ElementType::LIGHT);
			}

			SOUND.Play_Sound(L"magic_explosion_n_08", CHANNELID::SOUND_EFFECT, 1.f * g_fCharacterEffectRatio, Get_Transform()->Get_State(Transform_State::POS).xyz(), 100.f);
		
			m_fCurBgmRatio = g_fBgmRatio;
			m_fCurEnvironmentRatio = g_fEnvironmentRatio;
			m_fCurCharacterVoiceRatio = g_fCharacterVoiceRatio;
			m_fCurCharacterEffectRatio = g_fCharacterEffectRatio;
			m_fCurMonsterVoiceRatio = g_fMonsterVoiceRatio;
			m_fCurMonsterEffectRatio = g_fMonsterEffectRatio;
			m_fCurSystemSoundRatio = g_fSystemSoundRatio;
		}
	}

	if (m_fAcc >= m_fExploseTime && m_fAcc <= m_fEndTime)
	{
		CUR_SCENE->g_RadialBlurData.g_bRadialBlurOn = true;
		CUR_SCENE->g_RadialBlurData.g_fNormalRadius = 0.f;
		CUR_SCENE->g_RadialBlurData.g_fRadialBlurStrength += 1.f / (m_fEndTime - m_fExploseTime) * fDT;
	
		g_fBgmRatio = SoundVolumeLerp(m_fCurBgmRatio, 0.f, m_fSoundRatio);
		g_fEnvironmentRatio = SoundVolumeLerp(m_fCurEnvironmentRatio, 0.f, m_fSoundRatio);
		g_fCharacterVoiceRatio = SoundVolumeLerp(m_fCurCharacterVoiceRatio, 0.f, m_fSoundRatio);
		g_fCharacterEffectRatio = SoundVolumeLerp(m_fCurCharacterEffectRatio, 0.f, m_fSoundRatio);
		g_fMonsterVoiceRatio = SoundVolumeLerp(m_fCurMonsterVoiceRatio, 0.f, m_fSoundRatio);
		g_fMonsterEffectRatio = SoundVolumeLerp(m_fCurMonsterEffectRatio, 0.f, m_fSoundRatio);
		g_fSystemSoundRatio = SoundVolumeLerp(m_fCurSystemSoundRatio, 0.f, m_fSoundRatio);
	
		m_fSoundRatio += fDT;

		if (m_fSoundRatio >= 1.f)
			m_fSoundRatio = 1.f;
	}
	else if (m_fAcc > m_fEndTime)
	{
		CUR_SCENE->g_RadialBlurData.g_fRadialBlurStrength -= 1.f / (m_fEndTime - m_fExploseTime) * fDT;
		curAmbientColor = _float4::Lerp(curAmbientColor, m_vOriginAmbientColor, 5.f * fDT);
		curDiffuseColor = _float4::Lerp(curDiffuseColor, m_vOriginDiffuseColor, 5.f * fDT);

		if (CUR_SCENE->g_RadialBlurData.g_fRadialBlurStrength < 0.f)
		{
			CUR_SCENE->g_RadialBlurData.g_fRadialBlurStrength = 0.f;
			CUR_SCENE->g_RadialBlurData.g_bRadialBlurOn = false;
			
			curAmbientColor = m_vOriginAmbientColor;
			curDiffuseColor = m_vOriginDiffuseColor;
			SWITCHMGR.Set_SwitchState(SWITCH_TYPE::YEONHEE_300100_LIGHT_OFF, false);
			
			EVENTMGR.Delete_Object(Get_Owner());
		}

		if (!m_bSoundRecovery)
		{
			m_bSoundRecovery = true;
			m_fSoundRatio = 0.f;
		}

		g_fBgmRatio = SoundVolumeLerp(0.f, m_fCurBgmRatio, m_fSoundRatio);
		g_fEnvironmentRatio = SoundVolumeLerp(0.f, m_fCurEnvironmentRatio, m_fSoundRatio);
		g_fCharacterVoiceRatio = SoundVolumeLerp(0.f, m_fCurCharacterVoiceRatio, m_fSoundRatio);
		g_fCharacterEffectRatio = SoundVolumeLerp(0.f, m_fCurCharacterEffectRatio, m_fSoundRatio);
		g_fMonsterVoiceRatio = SoundVolumeLerp(0.f, m_fCurMonsterVoiceRatio, m_fSoundRatio);
		g_fMonsterEffectRatio = SoundVolumeLerp(0.f, m_fCurMonsterEffectRatio, m_fSoundRatio);
		g_fSystemSoundRatio = SoundVolumeLerp(0.f, m_fCurSystemSoundRatio, m_fSoundRatio);

		m_fSoundRatio += fDT;

		if (m_fSoundRatio >= 1.f)
			m_fSoundRatio = 1.f;
	}
}

_float MeteorRadialBlur::SoundVolumeLerp(_float fStart, _float fEnd, _float fRatio)
{
	return fStart * (1.f - fRatio) + fEnd * fRatio;
}
