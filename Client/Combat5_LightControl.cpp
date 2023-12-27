#include "pch.h"
#include "Combat5_LightControl.h"
#include "Light.h"

void Combat5_LightControl::Tick()
{
	_float3 vPlayerPos = m_pPlayer.lock()->Get_Transform()->Get_State(Transform_State::POS).xyz();
	_float3 vMyPos = Get_Transform()->Get_State(Transform_State::POS).xyz();
	
	if (!CUR_SCENE->Get_Light())
		return;

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
		SWITCHMGR.Set_SwitchState(SWITCH_TYPE::COMBAT5_END, true);
	}
	else if ((vPlayerPos - vMyPos).LengthSquared() < 5.f * 5.f)
	{


		if (vPlayerPos.z < 60.f)
		{
			Color targetAmbientColor = Color(119.f, 30.f, 30.f, 255.f) / 255.f;
			Color targetDiffuseColor = Color(2355.f, 508.f, 508.f, 255.f) / 255.f;

			curAmbientColor = _float4::Lerp(curAmbientColor, targetAmbientColor, 3.f * fDT);
			curDiffuseColor = _float4::Lerp(curDiffuseColor, targetDiffuseColor, 3.f * fDT);


		}
		else
		{
			Color targetAmbientColor = Color(244.f, 244.f, 244.f, 255.f) / 255.f;
			Color targetDiffuseColor = Color(1180.f, 1180.f, 1180.f, 255.f) / 255.f;

			curAmbientColor = _float4::Lerp(curAmbientColor, targetAmbientColor, 3.f * fDT);
			curDiffuseColor = _float4::Lerp(curDiffuseColor, targetDiffuseColor, 3.f * fDT);


		}


	}

}
