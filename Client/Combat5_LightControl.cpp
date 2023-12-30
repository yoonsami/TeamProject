#include "pch.h"
#include "Combat5_LightControl.h"
#include "Light.h"

void Combat5_LightControl::Tick()
{
	_float3 vPlayerPos = m_pPlayer.lock()->Get_Transform()->Get_State(Transform_State::POS).xyz();
	_float3 vMyPos = Get_Transform()->Get_State(Transform_State::POS).xyz();
	
	if (!CUR_SCENE->Get_Light())
		return;

	_float fBGMVolume = SOUND.Get_Volume(CHANNELID::SOUND_BGM) * 1.f/ g_fBgmRatio;
	_float fSubBGMVolume = SOUND.Get_Volume(CHANNELID::SOUND_BGM2) * 1.f / g_fBgmRatio;

	auto& lightColor = CUR_SCENE->Get_Light()->Get_Light()->Get_LightInfo().color;
	Color& curAmbientColor = lightColor.ambient;
	Color& curDiffuseColor = lightColor.diffuse;

	if (SWITCHMGR.Get_Score(SCORE_TYPE::COMBAT_5) >= SWITCHMGR.Get_Score(SCORE_TYPE::COMBAT_5_MONSTER_COUNT))
	{
		Color targetAmbientColor = Color(244.f, 244.f, 244.f, 255.f) / 255.f;
		Color targetDiffuseColor = Color(1180.f, 1180.f, 1180.f, 255.f) / 255.f;

		if(!SWITCHMGR.Get_SwitchState(SWITCH_TYPE::YEONHEE_300100_LIGHT_OFF))
		{
			curAmbientColor = _float4::Lerp(curAmbientColor, targetAmbientColor, 3.f * fDT);
			curDiffuseColor = _float4::Lerp(curDiffuseColor, targetDiffuseColor, 3.f * fDT);
		}

		fBGMVolume = _float2::Lerp(_float2(fBGMVolume), _float2(0.5f), 3.f * fDT).x;
		fSubBGMVolume = _float2::Lerp(_float2(fSubBGMVolume), _float2(0.f), 3.f * fDT).x;

		SOUND.Set_Volume(SOUND_BGM, fBGMVolume * g_fBgmRatio);
		SOUND.Set_Volume(SOUND_BGM2, fSubBGMVolume * g_fBgmRatio);

		SWITCHMGR.Set_SwitchState(SWITCH_TYPE::COMBAT5_END, true);
	}
	else if ((vPlayerPos - vMyPos).LengthSquared() < 5.f * 5.f)
	{
		SOUND.Play_Sound(L"BGM_FieldBattle1", CHANNELID::SOUND_BGM2, 0.5f * g_fBgmRatio);
		SWITCHMGR.Set_SwitchState(SWITCH_TYPE::IN_COMBAT5, true);
		SWITCHMGR.Set_SwitchState(SWITCH_TYPE::OUT_COMBAT5, false);
		if (vPlayerPos.z < 60.f)
		{
			Color targetAmbientColor = Color(119.f, 30.f, 30.f, 255.f) / 255.f;
			Color targetDiffuseColor = Color(2355.f, 508.f, 508.f, 255.f) / 255.f;

			curAmbientColor = _float4::Lerp(curAmbientColor, targetAmbientColor, 3.f * fDT);
			curDiffuseColor = _float4::Lerp(curDiffuseColor, targetDiffuseColor, 3.f * fDT);

			fBGMVolume = _float2::Lerp(_float2(fBGMVolume), _float2(0.f), 3.f * fDT).x;
			fSubBGMVolume = _float2::Lerp(_float2(fSubBGMVolume), _float2(0.5f), 3.f * fDT).x;

			SOUND.Set_Volume(SOUND_BGM, fBGMVolume * g_fBgmRatio);
			SOUND.Set_Volume(SOUND_BGM2, fSubBGMVolume * g_fBgmRatio);


		}
		else
		{
			SWITCHMGR.Set_SwitchState(SWITCH_TYPE::IN_COMBAT5, false);
			SWITCHMGR.Set_SwitchState(SWITCH_TYPE::OUT_COMBAT5, true);
			Color targetAmbientColor = Color(244.f, 244.f, 244.f, 255.f) / 255.f;
			Color targetDiffuseColor = Color(1180.f, 1180.f, 1180.f, 255.f) / 255.f;

			curAmbientColor = _float4::Lerp(curAmbientColor, targetAmbientColor, 3.f * fDT);
			curDiffuseColor = _float4::Lerp(curDiffuseColor, targetDiffuseColor, 3.f * fDT);

			fBGMVolume = _float2::Lerp(_float2(fBGMVolume), _float2(0.5f), 3.f * fDT).x;
			fSubBGMVolume = _float2::Lerp(_float2(fSubBGMVolume), _float2(0.f), 3.f * fDT).x;

			SOUND.Set_Volume(SOUND_BGM, fBGMVolume * g_fBgmRatio);
			SOUND.Set_Volume(SOUND_BGM2, fSubBGMVolume * g_fBgmRatio);
		}


	}

}
